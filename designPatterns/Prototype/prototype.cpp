#include "prototype.h"
#include <string.h>


ResumeA::ResumeA(const char* str)
{
	if(str == NULL)
	{
		name = new char[1];
		name[0] = '\0';
	}
	else
	{
		name = new char(strlen(str)+1);
		strcpy(name,str);
	}
}

ResumeA::~ResumeA()
{
	delete[] name;
}

ResumeA::ResumeA(const ResumeA &r)
{
	name = new char[strlen(r.name)+1];
	strcpy(name,r.name);
}

ResumeA* ResumeA::Clone()
{
	return new ResumeA(*this);
}

void ResumeA::Show()
{
    cout << "ResumeA name: " << name << endl;
}



ResumeB::ResumeB(const char* str)
{
	if(str == NULL)
	{
		name = new char[1];
		name[0] = '\0';
	}
	else
	{
		name = new char(strlen(str)+1);
		strcpy(name,str);
	}
}

ResumeB::~ResumeB()
{
	delete[] name;
}

ResumeB::ResumeB(const ResumeB &r)
{
	name = new char[strlen(r.name)+1];
	strcpy(name,r.name);
}

ResumeB* ResumeB::Clone()
{
	return new ResumeB(*this);
}

void ResumeB::Show()
{
    cout << "ResumeA name: " << name << endl;
}

int main(int argc,char* argv[])
{
	Resume *r1 = new ResumeA("A");
	Resume *r2 = new ResumeB("B");
	Resume *r3 = r1->Clone();
	Resume *r4 = r2->Clone();
	delete r1;
	delete r2;
	r3->Show();
	r4->Show();
	delete r3;
	delete r4;
	return 0;
}


















