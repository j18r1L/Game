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
    //Do linux things
#endif

#ifdef HE_DEBUG

    #ifdef HE_PLATFORM_WINDOWS
        #ifdef HE_SILENT_ASSERTS
            #define HE_DEBUG_BREAK()
        #elif HE_VISUAL_STUDIO
            #define HE_DEBUG_BREAK() __debugbreak()
        #elif HE_QT_CREATOR
            #define HE_DEBUG_BREAK() Q_ASSERT
        #else
            #define HE_DEBUG_BREAK() { HE_CORE_ERROR("Other IDE than VISUAL_STUDIO or QT_CREATOR is not correctly supported for debug breaks (no call trace), define HE_SILENT_ASSERTS to mute error, but the message still will be logged"); __asm("int $3\n" : : );}
        #endif
    #elif HE_PLATFORM_LINUX
        #include <signal.h>
        #define HE_DEBUG_BREAK() raise(SIGTRAP)
    #endif

    #define HE_ASSERT(x, ...) { if(!(x)) {HE_ERROR("Assertion Failed: {0}", __VA_ARGS__); HE_DEBUG_BREAK(); } }
    #define HE_CORE_ASSERT(x, ...) { if(!(x)) {HE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); HE_DEBUG_BREAK(); } }
#else
    #define HE_ASSERT(x, ...)
    #define HE_CORE_ASSERT(x, ...)
#endif
//
#define BIT(x) (1 << x)

// Для биндинга эвентов
#define HE_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
// TODO
//#define HE_BIND_EVENT_FN(x) [this](auto&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...);}

#endif // CORE_H
