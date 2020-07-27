#ifndef CORE_H
#define CORE_H

#ifdef HE_PLATFORM_WINDOWS
    #ifdef HE_BUILD_DLL
        #define HARTENG_API __declspec(dllexport)
    #else
        #define HARTENG_API __declspec(dll import)
    #endif
#else
    //Do linux things
#endif

#endif // CORE_H
