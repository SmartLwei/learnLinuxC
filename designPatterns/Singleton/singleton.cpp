#include "singleton.h"
#include <string.h>

Singleton* Singleton::singleton = NULL;
Singleton* Singleton::GetInstance(const char* name)
{
    if(singleton == NULL)
    {
        if(strcmp(name,"SingletonA") == 0)
        	singleton = new SingletonA;
        else if(strcmp(name,"SingletonB") == 0)
        	singleton = new SingletonB;
        else
        	singleton = new Singleton();
    }
    return singleton;
}
    void show() {cout << "Singleton";}

int main(int argc,char* argv[])
{
    Singleton *mySingleton = Singleton::GetInstance("SingletonB");
    mySingleton->show();
    
    return 0;
}
