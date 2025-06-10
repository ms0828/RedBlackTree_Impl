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
	// 1. 루트 노드는 블랙이다.
	// 2. 리프 노드는 블랙이다.
	// 3. 레드 노드의 자식은 블랙이다.
	// 4. 루트 노드부터 리프 노드까지 모든 경로의 블랙 노드의 개수는 같다.

	if (curNode == nullptr)
	{
		cout << "curNode가 nullptr 입니다.\n";
		exit(1);
	}

	// 루트 노드라면 블랙인지 확인
	if (curNode == rbTree->root && curNode->color == CMyRBTree::ECOLOR::RED)
	{
		cout << "루트 노드가 RED 입니다.\n";
		exit(1);
	}

	// nil 노드를 만나면 blackCnt 종합
	if (curNode == rbTree->nil)
	{
		if (curNode->color == CMyRBTree::ECOLOR::RED)
		{
			cout << "nil 노드가 RED 입니다.\n";
			exit(1);
		}
		else
		{
			blackCntVector.push_back(blackCnt + 1);
			return;
		}
	}

	// 이전 노드의 색이 레드라면 현재 노드가 블랙인지 확인
	if (prevColor == CMyRBTree::ECOLOR::RED && curNode->color == CMyRBTree::ECOLOR::RED)
	{
		cout << "레드가 연속으로 나왔습니다.\n";
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
		//cout << "-------------삽입-------------\n";
	//else
		//cout << "-------------삭제-------------\n";

	// 레드 블랙 트리 규칙 확인
	blackCntVector.clear();
	rbTest(rbTree, rbTree->root, CMyRBTree::ECOLOR::BLACK, 0);
	if (checkBlackCnt() == false)
	{
		cout << "특정 경로에서 블랙 Cnt가 다릅니다. \n";
		exit(1);
	}
	//cout << "총 경로의 수는 " << blackCntVector.size() << " 입니다." << "\n";
	//cout << "blackCnt가 " << blackCntVector[0] << "로 모두 동일합니다.\n";
	
	
	// ---- 오름차순 일치 확인 ----
	vector<int> btree_inorder;
	inorderTest(rbTree ,btree_inorder, rbTree->root);


	if (btree_inorder.size() >= 2)
	{
		for (int i = 1; i < btree_inorder.size(); i++)
		{
			if (btree_inorder[i - 1] >= btree_inorder[i])
			{
				cout << "오름차순 정렬이 아닙니다. \n";
				exit(1);
			}
		}
	}


	if (btree_inorder.size() == MAXNUMBER)
	{
		cout << "full insert : 오름차순 테스트 완료 -> ";
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
			unique_numbers.insert(num);  // 중복이면 삽입되지 않음
		}

		// -----------삽입 테스트-------------
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



		// ------------ 삭제 테스트 ------------
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


		// 모두 삭제하였는지 확인 후, 모두 삭제하였다면 결과 출력
		if (blackCntVector[0] != 2 || removeCnt != MAXNUMBER || rbTree.root != nullptr)
		{
			cout << "모두 삭제 되지 않음\n";
			return 0;
		}
		else
		{
			cout << "full remove : 삭제 테스트 완료 -> ";
			for (int i = 0; i < 10; i++)
			{
				cout << removeRandBuf[i] << ", ";
			}
			cout << "... 순으로 삭제 하였음\n";
			cout << "------------------------------\n";
			//Sleep(1000);
		}

	}
	return 0;
}