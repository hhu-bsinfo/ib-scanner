#ifndef PROJECT_BUILDCONFIG_H
#define PROJECT_BUILDCONFIG_H


namespace IbPerfMon {

/**
 * Holds information about the build cofiguration and program version.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date October 2018
 */
class BuildConfig {

public:

    static const char *VERSION;
    static const char *GIT_REV;
    static const char *BUILD_DATE;

};

}


#endif
