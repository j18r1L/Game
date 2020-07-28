#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H
#include <iostream>
#include <HartEng.h>

#ifdef HE_PLATFORM_LINUX

extern HE::Application* HE::CreateApplication();

using namespace std;

int main(int argc, char *argv[])
{
    HE::Log::Init();
    HE_CORE_WARN("Initialized Log!");

    int a = 5;
    HE_INFO("Hello! Var={0}", a);
    auto app = HE::CreateApplication();


    app->Run();


    delete app;
    return 0;
}


#endif

#endif // ENTRYPOINT_H
