/**
* 工厂方法模式
*/
#include "factory2.h"

int main(int argc, char* argv[])
{
    //工厂的产品
    product* myProduct = NULL;
    //工厂
    factoryA myFactoryA;
    factoryB myFactoryB; 
    
    //利用工厂A生产A产品
    myProduct = myFactoryA.createProduct();
    myProduct->show();
    delete myProduct;
    
    //利用工厂B生产B产品
    myProduct = myFactoryB.createProduct();
    myProduct->show();
    delete myProduct;
    
    return 0;
}
