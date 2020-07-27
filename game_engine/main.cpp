

#include <HartEng.h>


class SandBox: public HE::Application
{
public:
    SandBox() {}

    ~SandBox() {}
};


HE::Application* HE::CreateApplication()
{
    return new SandBox();
}
