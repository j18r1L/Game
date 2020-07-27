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


/*
m for members
c for constants/readonlys
p for pointer (and pp for pointer to pointer)
v for volatile
s for static
i for indexes and iterators
e for events
*/
