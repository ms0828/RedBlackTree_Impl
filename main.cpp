#include <iostream>
#include <set>
#include <array>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <Windows.h>
#include "MyRBTree.h"

using namespace std;

vector<int> blackCntVector;
set<int> unique_numbers;

#define MAXNUMBER 500

void rbTest(CMyRBTree *rbTree, CMyRBTree::Node* curNode, CMyRBTree::ECOLOR prevColor, int blackCnt)
{
	// 1. ��Ʈ ���� ���̴�.
	// 2. ���� ���� ���̴�.
	// 3. ���� ����� �ڽ��� ���̴�.
	// 4. ��Ʈ ������ ���� ������ ��� ����� �� ����� ������ ����.

	if (curNode == nullptr)
	{
		cout << "curNode�� nullptr �Դϴ�.\n";
		exit(1);
	}

	// ��Ʈ ����� ������ Ȯ��
	if (curNode == rbTree->root && curNode->color == CMyRBTree::ECOLOR::RED)
	{
		cout << "��Ʈ ��尡 RED �Դϴ�.\n";
		exit(1);
	}

	// nil ��带 ������ blackCnt ����
	if (curNode == rbTree->nil)
	{
		if (curNode->color == CMyRBTree::ECOLOR::RED)
		{
			cout << "nil ��尡 RED �Դϴ�.\n";
			exit(1);
		}
		else
		{
			blackCntVector.push_back(blackCnt + 1);
			return;
		}
	}

	// ���� ����� ���� ������ ���� ��尡 ������ Ȯ��
	if (prevColor == CMyRBTree::ECOLOR::RED && curNode->color == CMyRBTree::ECOLOR::RED)
	{
		cout << "���尡 �������� ���Խ��ϴ�.\n";
		exit(1);
	}

	if (curNode->color == CMyRBTree::ECOLOR::BLACK)
	{
		rbTest(rbTree, curNode->left, CMyRBTree::ECOLOR::BLACK, blackCnt + 1);
		rbTest(rbTree, curNode->right, CMyRBTree::ECOLOR::BLACK, blackCnt + 1);
	}
	else
	{
		rbTest(rbTree, curNode->left, CMyRBTree::ECOLOR::RED, blackCnt);
		rbTest(rbTree, curNode->right, CMyRBTree::ECOLOR::RED, blackCnt);
	}
}

bool checkBlackCnt()
{
	for (size_t i = 1; i < blackCntVector.size(); ++i)
	{
		if (blackCntVector[i] != blackCntVector[0])
			return false;
	}
	return true;
}

void inorderTest(CMyRBTree* rbTree, vector<int>& result, CMyRBTree::Node* curNode)
{
	if (curNode == rbTree->nil)
		return;

	inorderTest(rbTree, result, curNode->left);
	result.push_back(curNode->key);
	inorderTest(rbTree, result, curNode->right);
}

void rbTest(CMyRBTree* rbTree, bool isInsert)
{
	//if(isInsert)
		//cout << "-------------����-------------\n";
	//else
		//cout << "-------------����-------------\n";

	// ���� �� Ʈ�� ��Ģ Ȯ��
	blackCntVector.clear();
	rbTest(rbTree, rbTree->root, CMyRBTree::ECOLOR::BLACK, 0);
	if (checkBlackCnt() == false)
	{
		cout << "Ư�� ��ο��� �� Cnt�� �ٸ��ϴ�. \n";
		exit(1);
	}
	//cout << "�� ����� ���� " << blackCntVector.size() << " �Դϴ�." << "\n";
	//cout << "blackCnt�� " << blackCntVector[0] << "�� ��� �����մϴ�.\n";
	
	
	// ---- �������� ��ġ Ȯ�� ----
	vector<int> btree_inorder;
	inorderTest(rbTree ,btree_inorder, rbTree->root);


	if (btree_inorder.size() >= 2)
	{
		for (int i = 1; i < btree_inorder.size(); i++)
		{
			if (btree_inorder[i - 1] >= btree_inorder[i])
			{
				cout << "�������� ������ �ƴմϴ�. \n";
				exit(1);
			}
		}
	}


	if (btree_inorder.size() == MAXNUMBER)
	{
		cout << "full insert : �������� �׽�Ʈ �Ϸ� -> ";
		for (int i = 0; i < 10; i++)
		{
			cout << btree_inorder[i] <<", ";
		}
		cout << "... \n";
		//Sleep(1000);
	}
	
	//cout << "-------------------------------\n";
}





int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));
	CMyRBTree rbTree;
	while (1)
	{
		unique_numbers.clear();
		while (unique_numbers.size() < MAXNUMBER)
		{
			int num = rand() % 3000; // 0~2999
			unique_numbers.insert(num);  // �ߺ��̸� ���Ե��� ����
		}

		// -----------���� �׽�Ʈ-------------
		vector<int> insertRandBuf(unique_numbers.begin(), unique_numbers.end());
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(insertRandBuf.begin(), insertRandBuf.end(), std::default_random_engine(seed));

		int insertCnt = 0;
		for (int num : insertRandBuf)
		{
			rbTree.insert(num);
			rbTest(&rbTree, true);  // *****
			insertCnt++;
		}
		if (insertCnt != MAXNUMBER)
		{
			cout << "insertCnt != MAXNUMBER\n";
			return 0;
		}



		// ------------ ���� �׽�Ʈ ------------
		vector<int> removeRandBuf(unique_numbers.begin(), unique_numbers.end());
		seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(removeRandBuf.begin(), removeRandBuf.end(), std::default_random_engine(seed));
		int removeCnt = 0;
		for (int num : removeRandBuf)
		{
			bool removeRet = rbTree.remove(num);
			if (removeRet == false)
			{
				cout << "remove fail\n";
				return 0;
			}
			if(rbTree.root != nullptr)
				rbTest(&rbTree, false);
			removeCnt++;
		}


		// ��� �����Ͽ����� Ȯ�� ��, ��� �����Ͽ��ٸ� ��� ���
		if (blackCntVector[0] != 2 || removeCnt != MAXNUMBER || rbTree.root != nullptr)
		{
			cout << "��� ���� ���� ����\n";
			return 0;
		}
		else
		{
			cout << "full remove : ���� �׽�Ʈ �Ϸ� -> ";
			for (int i = 0; i < 10; i++)
			{
				cout << removeRandBuf[i] << ", ";
			}
			cout << "... ������ ���� �Ͽ���\n";
			cout << "------------------------------\n";
			//Sleep(1000);
		}

	}
	return 0;
}