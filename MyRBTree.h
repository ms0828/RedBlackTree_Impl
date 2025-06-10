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
		// ----- �⺻ ������ -----
		// nil ��� ���� ���� 
		// -----------------------
		Node()
		{
			parent = nullptr;
			left = nullptr;
			right = nullptr;
			color = ECOLOR::BLACK;
			key = 0;
		}

		// ----- ��� ���� -----
		// �Ű� ���� : 
		// - RBTree ��ü(owner) ������
		// - �θ� ���
		// - ������
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
	

	// ���� ��ȸ ��� �׽�Ʈ
	void inorder(Node* curNode)
	{
		if (curNode == nil)
			return;
		inorder(curNode->left);
		std::cout << curNode->key << ", ";
		inorder(curNode->right);
	}

	// ���� ��ȸ (�޸� ����)
	// -> �Ʒ� ������ �޸� ����
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


