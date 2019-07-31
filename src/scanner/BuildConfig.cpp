#include "BuildConfig.h"

#define XSTRINGIFY(a) STRINGIFY(a)
#define STRINGIFY(a) #a

namespace Scanner {

#ifdef IBSCANNER_VERSION
const char *BuildConfig::VERSION = XSTRINGIFY(IBSCANNER_VERSION);
#else
const char *BuildConfig::VERSION = "v0.0.0";
#endif

#ifdef IBSCANNER_GIT_REV
const char *BuildConfig::GIT_REV = XSTRINGIFY(IBSCANNER_GIT_REV);
#else
const char *BuildConfig::GIT_REV = "unknown";
#endif

#ifdef IBSCANNER_BUILD_DATE
const char *BuildConfig::BUILD_DATE = XSTRINGIFY(IBSCANNER_BUILD_DATE);
#else
const char *BuildConfig::BUILD_DATE = "0000-00-00 00:00:00";
#endif

#ifdef IBSCANNER_GIT_BRANCH
const char *BuildConfig::GIT_BRANCH = XSTRINGIFY(IBSCANNER_GIT_BRANCH);
#else
const char *BuildConfig::GIT_BRANCH = "unknown";
#endif

}

#undef STRINGIFY
#undef XSTRINGIFY