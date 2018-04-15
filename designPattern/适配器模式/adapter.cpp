#include "adapter.h"

int main(int argc, char* argv[])
{
    Sequence *s1 = new Stack();
    Sequence *s2 = new Queue();
    s1->push(1);
    s1->pop();
    s2->push(1);
    s2->pop();
    delete s1;
    return 0;
}
