#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "HartEng/pch.h"
#include "HartEng.h"


extern HE::Application* HE::CreateApplication();

using namespace std;

int main(int argc, char *argv[])
{
    HE::Log::Init();
    HE_CORE_WARN("Initialized Log!");
    HE_PROFILE_BEGIN_SESSION("StartUp", "HE_Profile_StartUp.json");
    auto app = HE::CreateApplication();
    HE_PROFILE_END_SESSION();
    HE_PROFILE_BEGIN_SESSION("RunTime", "HE_Profile_RunTime.json");
    app->Run();
    HE_PROFILE_END_SESSION();
    HE_PROFILE_BEGIN_SESSION("ShutDown", "HE_Profile_ShutDown.json");
    delete app;
    HE_PROFILE_END_SESSION();


    return 0;
}


#endif // ENTRYPOINT_H
