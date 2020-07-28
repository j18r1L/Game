#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "HartEng/pch.h"
#include "HartEng.h"

#ifdef HE_PLATFORM_LINUX

extern HE::Application* HE::CreateApplication();

using namespace std;

int main(int argc, char *argv[])
{
    HE::Log::Init();
    HE_CORE_WARN("Initialized Log!");

    int a = 5;
    HE_INFO("Hello! Var={0}", a);

    int b = 1;
    HE_INFO("Hello! Var={0}", b);

    HE_INFO("Hello! Var={0}", b);


    auto app = HE::CreateApplication();
    app->Run();
    delete app;


    return 0;
}


#endif

#endif // ENTRYPOINT_H
