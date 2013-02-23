#if !defined(CHUCHO_EXPORT_HPP__)
#define CHUCHO_EXPORT_HPP__

#if defined(chucho_EXPORTS)

#if defined(_MSC_VER)
#define CHUCHO_EXPORT __declspec(dllexport)
#elif defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 4)
#define CHUCHO_EXPORT __attribute__ ((visibility ("default")))
#elif defined(__SUNPRO_CC)
#define CHUCHO_EXPORT __symbolic
#else
#define CHUCHO_EXPORT
#endif

#else

#if defined(_MSC_VER)
#define CHUCHO_EXPORT __declspec(dllimport)
#elif defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 4)
#define CHUCHO_EXPORT __attribute__ ((visibility ("default")))
#elif defined(__SUNPRO_CC)
#define CHUCHO_EXPORT __hidden
#else
#define CHUCHO_EXPORT
#endif

#endif

#if defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 4)
#define CHUCHO_NO_EXPORT __attribute__ ((visibility ("hidden")))
#elif defined(__SUNPRO_CC)
#define CHUCHO_NO_EXPORT __hidden
#else
#define CHUCHO_NO_EXPORT
#endif

#endif
