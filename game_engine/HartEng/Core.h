#ifndef CORE_H
#define CORE_H

#ifdef HE_PLATFORM_WINDOWS
    #define DEBUG_BREAK __debugbreak()

    #ifdef HE_BUILD_DLL
        #define HARTENG_API __declspec(dllexport)
    #else
        #define HARTENG_API __declspec(dll import)
    #endif
#else
    #define DEBUG_BREAK
    //Do linux things
#endif

#ifdef HE_ENABLE_ASSERTS
    #define HE_ASSERT(x, ...) { if(!(x)) {HE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK; } }
    #define HE_CORE_ASSERT(x, ...) { if(!(x)) {HE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK; } }
#else
    #define HE_ASSERT(x, ...)
    #define HE_CORE_ASSERT(x, ...)
#endif
//
#define BIT(x) (1 << x)

#endif // CORE_H
