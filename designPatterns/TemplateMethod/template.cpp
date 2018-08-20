#include "template.h"

int main(int argc,char* argv[])
{
    Resume *r1;
    r1 = new ResumeA();
    r1->FillResume();
    delete r1;
    
    r1 = new ResumeB();
    r1->FillResume();
    delete r1;
    
    return 0;
}
