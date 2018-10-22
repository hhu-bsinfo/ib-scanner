#include "BuildConfig.h"

#define XSTRINGIFY(a) STRINGIFY(a)
#define STRINGIFY(a) #a

namespace IbPerfMon {

#ifdef IBPERFMON_VERSION
const char *BuildConfig::VERSION = XSTRINGIFY(IBPERFMON_VERSION);
#else
const char *BuildConfig::VERSION = "v0.0.0";
#endif

#ifdef IBPERFMON_GIT_REV
const char *BuildConfig::GIT_REV = XSTRINGIFY(IBPERFMON_GIT_REV);
#else
const char *BuildConfig::GIT_REV = "unknown";
#endif

#ifdef IBPERFMON_BUILD_DATE
const char *BuildConfig::BUILD_DATE = XSTRINGIFY(IBPERFMON_BUILD_DATE);
#else
const char *BuildConfig::BUILD_DATE = "0000-00-00 00:00:00";
#endif

}

#undef STRINGIFY
#undef XSTRINGIFY