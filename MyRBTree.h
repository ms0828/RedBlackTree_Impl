#pragma once
#include <iostream>

class CMyRBTree
{
public:
	enum ECOLOR
	{
		BLACK = 0,
		RED
	};

	class Node
	{
		friend class CMyRBTree;

	private:
		// ----- 기본 생성자 -----
		// nil 노드 생성 전용 
		// -----------------------
		Node()
		{
			parent = nullptr;
			left = nullptr;
			right = nullptr;
			color = ECOLOR::BLACK;
			key = 0;
		}

		// ----- 노드 생성 -----
		// 매개 변수 : 
		// - RBTree 객체(owner) 포인터
		// - 부모 노드
		// - 데이터
		// ---------------------
		Node(CMyRBTree* owner, Node* _parent, int _key)
		{
			parent = _parent;
			left = owner->nil;
			right = owner->nil;
			color = ECOLOR::RED;
			key = _key;
		}

	public:
		Node* parent;
		Node* left;
		Node* right;
		ECOLOR color;
		int key;
	};

public:
	
	CMyRBTree()
	{
		nil = new Node();
		root = nullptr;
	}

	~CMyRBTree()
	{
		if (root != nullptr)
			postorder_deleteNode(root);

		delete nil;
	}

	Node* createNode(Node* _parent, int _key);

	bool insert(int _key);

	bool remove(int _key);

	void balancingInsert(Node* curNode);

	void deleteExtraBlack(Node* curNode);

	void leftRotate(Node* node);

	void rightRotate(Node* node);

	Node* findNode(int findKey);
	

	// 중위 순회 출력 테스트
	void inorder(Node* curNode)
	{
		if (curNode == nil)
			return;
		inorder(curNode->left);
		std::cout << curNode->key << ", ";
		inorder(curNode->right);
	}

	// 후위 순회 (메모리 해제)
	// -> 아래 노드부터 메모리 해제
	void postorder_deleteNode(Node* curNode)
	{
		if (curNode == nil)
			return;
		postorder_deleteNode(curNode->left);
		postorder_deleteNode(curNode->right);
		delete curNode;
	}

public:
	Node* nil;
	Node* root;
};


