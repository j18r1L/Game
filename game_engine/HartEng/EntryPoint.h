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


    auto app = HE::CreateApplication();
    app->Run();
    delete app;


    return 0;
}


#endif // ENTRYPOINT_H
