#ifndef COMMON_H_
#define COMMON_H_
#include <iostream>
#include <vector>
using namespace std;

struct BinaryTreeNode
{
	int 				m_nValue;
	BinaryTreeNode*		m_pLeft;
	BinaryTreeNode*		m_pRight;
	BinaryTreeNode(int x=0, BinaryTreeNode* left=NULL, BinaryTreeNode* right=NULL):
		m_nValue(x), m_pLeft(left), m_pRight(right) {}
};

//打印二叉树
inline void PrintBinaryTreeNode(BinaryTreeNode* node)
{
	if(node == NULL)
		return;
	//广度优先遍历
	vector<BinaryTreeNode*> currLayer;
	vector<BinaryTreeNode*> nextLayer;
	currLayer.push_back(node);
	while(!currLayer.empty())
	{
		for(int i=0; i<currLayer.size();i++)
		{
			cout << currLayer[i]->m_nValue << "\t";
			if(currLayer[i]->m_pLeft != NULL)
				nextLayer.push_back(currLayer[i]->m_pLeft);
			else
				cout << "NULL\t";
			if(currLayer[i]->m_pRight != NULL)
				nextLayer.push_back(currLayer[i]->m_pRight);
			else
				cout << "NULL\t";
		}
		currLayer.swap(nextLayer);
		nextLayer.clear();
	}
}






























#endif
