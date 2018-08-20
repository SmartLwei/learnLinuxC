/**
* 抽象工厂模式
*/
#include "factory3.h"

int main(int argc, char* argv[])
{
    //工厂的产品
    productSeries1* myProductSeries1 = NULL;
    productSeries2* myProductSeries2 = NULL;
    //工厂
    factoryA myFactoryA;
    factoryB myFactoryB; 
    
    //利用工厂A生产系列1中的A产品
    myProductSeries1 = myFactoryA.createSeries1();
    myProductSeries1->show();
    delete myProductSeries1;
    //利用工厂A生产系列2中的A产品
    myProductSeries2 = myFactoryA.createSeries2();
    myProductSeries2->show();
    delete myProductSeries2;
    
    //利用工厂B生产系列1中的A产品
    myProductSeries1 = myFactoryB.createSeries1();
    myProductSeries1->show();
    delete myProductSeries1;
    //利用工厂B生产系列2中的A产品
    myProductSeries2 = myFactoryB.createSeries2();
    myProductSeries2->show();
    delete myProductSeries2;
    
    return 0;
}





















