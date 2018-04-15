/**
* 简单工厂模式
*/
#include "factory1.h"

int main(int argc, char* argv[])
{
	//新建工厂实例
    factory myfactory;
    //工厂的产品
    product* myProduct = NULL;
    
    //创建产品A
    myProduct = myfactory.createProduct(A);
    myProduct->show();
    delete myProduct;
    
    //创建产品B
    myProduct = myfactory.createProduct(B);
    myProduct->show();
    delete myProduct;
    
    return 0;
}
