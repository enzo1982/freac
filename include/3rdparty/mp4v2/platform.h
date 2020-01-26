#ifndef MP4V2_PLATFORM_H
#define MP4V2_PLATFORM_H

/*****************************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#if defined( _WIN32 ) || defined( __MINGW32__ )
#   if defined( MP4V2_EXPORTS )
#       define MP4V2_EXPORT __declspec(dllexport)
#   elif defined( MP4V2_USE_DLL_IMPORT ) || !defined( MP4V2_USE_STATIC_LIB )
#       define MP4V2_EXPORT __declspec(dllimport)
#   else
#       define MP4V2_EXPORT
#   endif
#else
#   define MP4V2_EXPORT __attribute__((visibility("default")))
#endif

#if defined( __GNUC__ )
#   define MP4V2_DEPRECATED __attribute__((deprecated))
#else
#   define MP4V2_DEPRECATED
#endif

/******************************************************************************
 *
 * TODO-KB: cleanup -- absolutely no need for a C-API to fuss with reserved
 * C++ keywords. This will involve changing the public interface and current
 * plan of action:
 *
 *      typdef enum {
 *          mp4_false,
 *          mp4_true,
 *      } mp4_bool_t;
 *
 * followed by updating all public signatures and implementation.
 */

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#if !defined( __cplusplus )
#ifndef bool
#if SIZEOF_BOOL == 8
typedef uint64_t bool;
#else
#if SIZEOF_BOOL == 4
typedef uint32_t bool;
#else
#if SIZEOF_BOOL == 2
typedef uint16_t bool;
#else
typedef unsigned char bool;
#endif
#endif
#endif
#ifndef false
#define false FALSE
#endif
#ifndef true
#define true TRUE
#endif
#endif
#endif

/*****************************************************************************/

#endif /* MP4V2_PLATFORM_H */
