#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H
#include <iostream>
#include <HartEng.h>

#ifdef HE_PLATFORM_LINUX

extern HE::Application* HE::CreateApplication();

using namespace std;

int main(int argc, char *argv[])
{
    cout << "hello" << endl;
    auto app = HE::CreateApplication();
    app->Run();
    delete app;
    return 0;
}


#endif

#endif // ENTRYPOINT_H
