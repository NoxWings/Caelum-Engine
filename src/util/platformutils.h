#ifndef PLATFORMUTILS_H
#define PLATFORMUTILS_H

#include "EnginePrerequisites.h"

namespace Caelum {

#define ENGINE_PLATFORM_WIN32 1
#define ENGINE_PLATFORM_LINUX 2
#define ENGINE_PLATFORM_APPLE 3
#define ENGINE_PLATFORM_APPLE_IOS 4
#define ENGINE_PLATFORM_ANDROID 5

#define ENGINE_ENDIAN_LITTLE 1
#define ENGINE_ENDIAN_BIG 2

#define ENGINE_ARCH_32 1
#define ENGINE_ARCH_64 2


/* Find platform */

// WIN32
#if defined( __WIN32__ ) || defined( _WIN32 )
#   define ENGINE_PLATFORM ENGINE_PLATFORM_WIN32
// APPLE & IOS
#elif defined( __APPLE_CC__)
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
#       define ENGINE_PLATFORM ENGINE_PLATFORM_APPLE_IOS
#   else
#       define ENGINE_PLATFORM ENGINE_PLATFORM_APPLE
#   endif
// ANDROID
#elif defined(__ANDROID__)
#   define ENGINE_PLATFORM ENGINE_PLATFORM_ANDROID
// LINUX
#else
#   define ENGINE_PLATFORM ENGINE_PLATFORM_LINUX
#endif


/* Find Endian */


class PlatformUtils {
  public:

    /** Returns the current operating system file separator
     *
     * @brief getFileSeparator
     * @return the file separator for the current OS
     */
    static const char* getFileSeparator();

    /** Transforms a path to the current OS route naming
     *
     * @brief formatPath
     * @param path the path to format
     * @return the formated path for the current OS
     */
    static const char* formatPath(const char* path);

  private:
    PlatformUtils() {}
    ~PlatformUtils() {}
};

}

#endif // PLATFORMUTILS_H
