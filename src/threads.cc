#include <dlfcn.h>
#include <cstddef>
#include <cstdlib>

#include <iostream>

#include "lsst/base/threads.h"

#ifndef RTLD_DEEPBIND // Non-POSIX flag, so it may not exist
#define RTLD_DEEPBIND 0 // Will be ignored
#endif

namespace lsst {
namespace base {

namespace {

typedef int (*Getter)(void);
typedef void (*Setter)(int);

Getter getOpenBlasThreads = NULL;
Setter setOpenBlasThreads = NULL;
Getter getMklThreads = NULL;
Setter setMklThreads = NULL;

bool loadOpenBlas() {
    if (haveOpenBlas) {
        return true;
    }

    void* openblas = dlopen("libopenblas.so", RTLD_LAZY | RTLD_LOCAL | RTLD_DEEPBIND);
    if (!openblas) return false;

    // Believe it or not, the function which returns the number of threads that OpenBLAS will actually use is
    // called "goto_get_num_procs". The number returned by THIS function, and not "openblas_get_num_threads"
    // nor "openblas_get_num_procs", is modified by calls to "openblas_set_num_threads". Confused? Me too.
    (void*&)getOpenBlasThreads = dlsym(openblas, "goto_get_num_procs");
    (void*&)setOpenBlasThreads = dlsym(openblas, "openblas_set_num_threads");
    return getOpenBlasThreads && setOpenBlasThreads;
}

bool loadMkl() {
    if (haveMkl) {
        return true;
    }

    if (!dlopen("libmkl_core.so", RTLD_LAZY | RTLD_GLOBAL)) {
        return false;
    }

    // We will set the number of threads through OMP because that's easier.
    // (There are multiple mkl libraries, and each has a function to set the number of threads.)
    void* omp = dlopen("libiomp5.so", RTLD_LAZY | RTLD_GLOBAL);
    if (!omp) {
        return false;
    }

    (void*&)getMklThreads = dlsym(omp, "omp_get_max_threads");
    (void*&)setMklThreads = dlsym(omp, "omp_set_num_threads");
    return getMklThreads && setMklThreads;
}

bool disableImplicitThreadingImpl(
    std::string const& package,                    // Name of package
    std::initializer_list<std::string const> envvars, // Environment variables to check
    Getter getter,                      // Function to get number of threads
    Setter setter                       // Function to set number of threads
    )
{
    for (auto&& ss : envvars) {
        if (std::getenv(ss.c_str())) {
            return false;               // User is being explicit
        }
    }
    unsigned int numThreads = getter();
    if (numThreads <= 1) {
        return false;
    }

    std::cerr << "WARNING: You are using " << package << " with multiple threads (" << numThreads <<
        "), but have not\n" <<
        "specified the number of threads using one of the " << package <<
        " environment variables:\n";

    // Join list of environment variables. An alternative is
    //     boost::algorithm::join(envvars, ", ")
    // but we are trying to control the proliferation of Boost usage.
    std::cerr << *envvars.begin();
    for (auto iter = envvars.begin() + 1; iter != envvars.end(); ++iter) {
        std::cerr << ", " << *iter;
    }

    std::cerr << ".\n" <<
        "This may indicate that you are unintentionally using multiple threads, which may\n"
        "cause problems. WE HAVE THEREFORE DISABLED " << package << " THREADING. If you know\n" <<
        "what you are doing and want threads enabled implicitly, set the environment\n" <<
        "variable " << allowEnvvar << ".\n";
    setter(1);
    return true;
}


} // anonymous namespace


extern bool const haveOpenBlas = loadOpenBlas();
extern bool const haveMkl = loadMkl();

void setNumThreads(unsigned int numThreads)
{
    if (!haveOpenBlas && !haveMkl && numThreads != 0 && numThreads != 1) {
        throw NoThreadsException();
    }
    if (haveOpenBlas) {
        setOpenBlasThreads(numThreads);
    }
    if (haveMkl) {
        setMklThreads(numThreads);
    }
}

unsigned int getNumThreads()
{
    unsigned int numThreads = 0;
    if (haveOpenBlas) {
        numThreads = std::max(numThreads, static_cast<unsigned int>(getOpenBlasThreads()));
    }
    if (haveMkl) {
        numThreads = std::max(numThreads, static_cast<unsigned int>(getMklThreads()));
    }
    return numThreads;
}

bool disableImplicitThreading()
{
    if (std::getenv(allowEnvvar.c_str())) {
        return false; // The user knows what he's doing; no intervention performed
    }

    bool intervened = false;            // Did we intervene on behalf of the user?
    if (haveOpenBlas) {
        intervened |= disableImplicitThreadingImpl(
            "OpenBLAS",
            {"OPENBLAS_NUM_THREADS", "GOTO_NUM_THREADS", "OMP_NUM_THREADS"},
            getOpenBlasThreads,
            setOpenBlasThreads
            );
    }
    if (haveMkl) {
        intervened |= disableImplicitThreadingImpl(
            "MKL",
            {"MKL_NUM_THREADS", "MKL_DOMAIN_NUM_THREADS", "OMP_NUM_THREADS"},
            getMklThreads,
            setMklThreads
            );
    }
    return intervened;
}

}} // namespace lsst::base
