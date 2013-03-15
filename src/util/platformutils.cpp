#include "util/platformutils.h"

using namespace Caelum;

const char* PlatformUtils::getFileSeparator() {
    static const char* winSeparator = "\\", linuxSeparator = "/";
    #if ENGINE_PLATFORM == ENGINE_PLATFORM_WIN32
    return winSeparator;
    #elif ENGINE_PLATFORM == ENGINE_PLATFORM_LINUX
    return linuxSeparator;
    #endif
}

const char* PlatformUtils::formatPath(const char *path) {
    String tmp = path;
    tmp->replace("/", getFileSeparator());
    return tmp.c_str();
}
