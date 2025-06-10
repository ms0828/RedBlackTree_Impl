#include "MyRBTree.h"

CMyRBTree::Node* CMyRBTree::createNode(Node* _parent, int _key)
{
	Node* newNode = new Node(this, _parent, _key);
	return newNode;
}

bool CMyRBTree::insert(int insertKey)
{
	if (root == nullptr)
	{
		root = createNode(nullptr, insertKey);
		balancingInsert(root);
		return true;
	}

	// ������ ����� �θ� ��� Ž�� 
	Node* curNode = root;
	Node* parentNode = root;
	while (curNode != nil)
	{
		// �ߺ� ���� ��� X
		if (insertKey == curNode->key)
			return false;

		parentNode = curNode;
		if (insertKey < curNode->key)
			curNode = curNode->left;
		else
			curNode = curNode->right;
	}

	// insertKey < parentNode �̸� ���� �ڽĿ� ����
	// insertKey > parentNode �̸� ������ �ڽĿ� ����
	Node* insertNode = createNode(parentNode, insertKey);
	if (insertKey < parentNode->key)
		parentNode->left = insertNode;
	else
		parentNode->right = insertNode;

	balancingInsert(insertNode);

	return true;
}

void CMyRBTree::balancingInsert(Node* curNode)
{	
	// curNode�� ��Ʈ��� ������ �ٲ۴�.
	//  -> 2�� ��Ģ - ��Ʈ ���� ���̾�� �Ѵ�.
	if (curNode == root)
	{
		curNode->color = ECOLOR::BLACK;
		return;
	}

	// curNode�� �θ� ��尡 ���̶�� ��Ģ ����
	if (curNode->parent->color == ECOLOR::BLACK)
		return;



	// ----------- �θ� ��尡 ���� -> 4�� ��Ģ ���� ------------
	// 1. ������ ��
	//	1-1. ������ ��ġ�� ������ �Ҿƹ��������� ��ΰ� �������� ����
	//  1-2. ������ ��ġ�� ������ �Ҿƹ��������� ��ΰ� �����ִ� ����
	// 2. ������ ����
	// ----------------------------------------------------------

	// �Ҿƹ���, �θ�, ���� ����
	Node* grand = curNode->parent->parent;
	Node* parent = curNode->parent;
	Node* uncle = nullptr;
	if (grand->left == parent)
		uncle = grand->right;
	else
		uncle = grand->left;

	
	// 1. ������ ��
	if (uncle->color == ECOLOR::BLACK)
	{
		// 1-1. ������ ��ġ�� ������ �Ҿƹ��������� ��ΰ� �������� ����
		//		- �θ�� �Ҿƹ����� ������ ��ȯ
		//		- �Ҿƹ��� ���� �ݴ������� ȸ��
		//		- �ڽ��� ������ �ڽ�����
		if (parent == grand->left && curNode == parent->left) // ��ΰ� �������� ������
		{
			grand->color = ECOLOR::RED;
			parent->color = ECOLOR::BLACK;
			rightRotate(grand);
			return;
		}
		else if(parent == grand->right && curNode == parent->right) // ��ΰ� ���������� ������
		{
			grand->color = ECOLOR::RED;
			parent->color = ECOLOR::BLACK;
			leftRotate(grand);
			return;
		}

		
		// 1-2. ������ ��ġ�� ������ �Ҿƹ��������� ��ΰ� �����ִ� ����
		//		- �θ� �������� ������ ��ġ�� �ݴ�� ȸ���Ͽ� 1-1 case �����
		//		- 1-1 case ���� �״�� ���� 
		if (parent == grand->left && curNode == parent->right)
		{
			leftRotate(parent);
			grand->color = ECOLOR::RED;
			curNode->color = ECOLOR::BLACK; // (ȸ�� �� curNode�� ������ �θ� ��ġ�� ���� ��)  
			rightRotate(grand);
			return;
		}
		else if (parent == grand->right && curNode == parent->left)
		{
			rightRotate(parent);
			grand->color = ECOLOR::RED;
			curNode->color = ECOLOR::BLACK; // (ȸ�� �� curNode�� ������ �θ� ��ġ�� ���� ��)  
			leftRotate(grand);
			return;
		}
	}
	else
	{
		// 2. ������ ����
		//		- ���带 �ݴ������� �ѱ� �� ����.
		//		-> �θ�, ���̰� �Ҿƹ����� ���� ��ȯ
		//		-> �Ҿƹ��� �������� �ٽ� �뷱�� �Լ� ����
		parent->color = ECOLOR::BLACK;
		uncle->color = ECOLOR::BLACK;
		grand->color = ECOLOR::RED;
		balancingInsert(grand);
		return;
	}
	return;
}


bool CMyRBTree::remove(int _key)
{
	Node* removeNode = findNode(_key);
	if (removeNode == nullptr)
		return false;

	if (removeNode->left == nil && removeNode->right == nil) // ������ ��尡 �ڽ��� ���� ��
	{
		if (removeNode == root)
		{
			delete removeNode;
			root = nullptr;
			return true;
		}

		Node* parentNode = removeNode->parent;
		if (removeNode == parentNode->left)
			parentNode->left = nil;
		else
			parentNode->right = nil;
		
		// ���� �� Ʈ�� �뷱�� ����
		if (removeNode->color == ECOLOR::BLACK)
		{
			nil->parent = parentNode;
			deleteExtraBlack(nil);
			nil->parent = nullptr;
		}
		delete removeNode;
		return true;
	}
	else if (removeNode->left != nil && removeNode->right == nil) // ������ ��尡 ���� �ڽĸ� ���� ��
	{
		Node* leftChild = removeNode->left;
		if (removeNode == root)
		{
			root = leftChild;
			leftChild->parent = nullptr;
			deleteExtraBlack(root);
			delete removeNode;
			return true;
		}

		Node* parentNode = removeNode->parent;
		if (removeNode == parentNode->left)
			parentNode->left = leftChild;
		else
			parentNode->right = leftChild;

		leftChild->parent = parentNode;
		
		// ���� �� Ʈ�� �뷱�� ����
		if (removeNode->color == ECOLOR::BLACK)
			deleteExtraBlack(leftChild);
		
		delete removeNode;
		return true;
	}
	else if (removeNode->left == nil && removeNode->right != nil) // ������ ��尡 ������ �ڽĸ� ���� ��
	{
		Node* rightChild = removeNode->right;
		if (removeNode == root)
		{
			root = rightChild;
			rightChild->parent = nullptr;
			deleteExtraBlack(root);
			delete removeNode;
			return true;
		}

		Node* parentNode = removeNode->parent;
		if (removeNode == parentNode->left)
			parentNode->left = rightChild;
		else
			parentNode->right = rightChild;

		rightChild->parent = parentNode;

		// ���� �� Ʈ�� �뷱�� ����
		if (removeNode->color == ECOLOR::BLACK)
			deleteExtraBlack(rightChild);

		delete removeNode;
		return true;
	}
	else // ������ ��尡 �ڽ��� 2�� �� ��
	{
		// ��ü �ĺ��� ã��
		// -> ������ ����� ������ �ڽ� �߿��� ���� ���� ���
		Node* curNode = removeNode->right;
		Node* successor = removeNode->right;
		while (curNode != nil)
		{
			successor = curNode;
			curNode = curNode->left;
		}

		// ��ü �ĺ��ڿ� ������ ��带 ��ü
		removeNode->key = successor->key;

		if (successor->right == nil) // ��ü�� ����� �ڽ��� ���� ��
		{
			if (successor == successor->parent->left) // ��ü�� ��尡 �θ��� ���� �ڽ��̸�
				successor->parent->left = nil;
			else
				successor->parent->right = nil; // ��ü�� ��尡 �θ��� ������ �ڽ��̸�

			// ���� �� Ʈ�� �뷱�� ����
			if (successor->color == ECOLOR::BLACK)
			{
				nil->parent = successor->parent;
				deleteExtraBlack(nil);
				nil->parent = nullptr;
			}
		}
		else // ��ü�� ����� ������ �ڽ��� ���� ��
		{
			if (successor == successor->parent->left) // ��ü�� ��尡 �θ��� ���� �ڽ��̸�
				successor->parent->left = successor->right;
			else
				successor->parent->right = successor->right; // ��ü�� ��尡 �θ��� ������ �ڽ��̸�

			successor->right->parent = successor->parent;

			// ���� �� Ʈ�� �뷱�� ����
			if (successor->color == ECOLOR::BLACK)
				deleteExtraBlack(successor->right);
		}
		delete successor;
		return true;
	}
}


void CMyRBTree::deleteExtraBlack(Node* ebNode)
{
	// 1-1. ebNode�� ��Ʈ �� ��
	// - ebNode�� ������ ������ �ٲ۴�.
	// - ebNode�� ���̶�� ��ٷ� eb�� ���ش�.
	// 1-2. ebNode�� RED�� ��
	// - ebNode�� ������ ������ �ٲ۴�.
	if (ebNode == root || ebNode->color == ECOLOR::RED)
	{
		ebNode->color = ECOLOR::BLACK;
		return;
	}


	// �Ʒ� ���ʹ� ebNode�� ��Ʈ�� �ƴϸ� ���� ���
	Node* parent = ebNode->parent;
	Node* sibling;
	if (ebNode == parent->left) // ebNode�� �θ��� ���� �ڽ��� ���
		sibling = parent->right;
	else
		sibling = parent->left; // ebNode�� �θ��� ������ �ڽ��� ���

	// 2. ebNode�� ������ ������ ��� (ebNode�� �θ�� �ݵ�� ��)
	// - ������ ������ �����Ѵ�.
	// - �θ�� ������ ���� �ٲ� ��, �θ� �������� ebNode �������� ȸ���Ѵ�. 
	// - ���̰� �� �ܰ� ������ ebNode�� ������ ���� �Ǿ����Ƿ� �ٽ� �뷱���� �����Ѵ�.
	if (sibling->color == ECOLOR::RED)
	{
		parent->color = ECOLOR::RED;
		sibling->color = ECOLOR::BLACK;
		if (ebNode == parent->left) // ebNode�� �θ��� ���� �ڽ��� ���
		{
			leftRotate(parent);
			sibling = parent->right; // 3 or 4 or 5�� case�� �̾ �� ���̹Ƿ� ������ �ٽ� ����
		}
		else // ebNode�� �θ��� ������ �ڽ��� ���
		{
			rightRotate(parent);
			sibling = parent->left; // 3 or 4 or 5�� case�� �̾ �� ���̹Ƿ� ������ �ٽ� ����
		}
	}

	// 3. ebNode�� ������ ���̰� ������ ���� �ڽ��� ��
	// - ebNode�� eb�� ������ black�� �θ�� �ѱ�� �θ� �������� �ٽ� �뷱���� ����
	// - �� ���� ���� �ö󰡸� �˻��ϴ� ������ ����ȴ�.
	if (sibling->left->color == ECOLOR::BLACK && sibling->right->color == ECOLOR::BLACK)
	{
		sibling->color = ECOLOR::RED;
		deleteExtraBlack(parent);
		return;
	}

	// 4. ebNode�� ������ ���̰�, ������ �ڽ��� ebNode�� �����ϴ� �������� ����� �����ϴ� ��� (�ݴ� ���� �ڽ��� ������ ����)
	// - ������ �������� �ݴ�� ȸ���Ͽ� 5�� case�� �����.
	//		 - 5�� case : ������ �ڽ��� ebNode�� �����ϴ� ����� �ݴ�� �����ϸ� ������ ���
	if (ebNode == parent->left && sibling->right->color == ECOLOR::BLACK && sibling->left->color == ECOLOR::RED) // ebNode�� �θ��� ���� �ڽ��� ���
	{
		sibling->color = ECOLOR::RED;
		sibling->left->color = ECOLOR::BLACK;
		rightRotate(sibling); 
		sibling = parent->right; // 5�� case�� �̾ �� ���̹Ƿ� ������ �ٽ� ����
	}
	else if (ebNode == parent->right && sibling->left->color == ECOLOR::BLACK && sibling->right->color == ECOLOR::RED) // ebNode�� �θ��� ������ �ڽ��� ���
	{
		sibling->color = ECOLOR::RED;
		sibling->right->color = ECOLOR::BLACK;
		leftRotate(sibling);
		sibling = parent->left; // 5�� case�� �̾ �� ���̹Ƿ� ������ �ٽ� ����
	}


	// 5. ebNode�� ������ ���̰�, ������ �ڽ��� ebNode�� �����ϴ� �ݴ��� �������� ����� �����ϴ� ��� (������ �ڽ��� �� �� ������ ���� ����)
	// - ������ ���� �θ��� ������ �ٲ۴�.
	// - �θ�� ������ �ݴ��� ���� �ڽ��� ������ �ٲ۴�.
	// - �θ� �������� ebNode �������� ȸ���� �����Ѵ�.
	if (ebNode == parent->left && sibling->right->color == ECOLOR::RED) // ebNode�� �θ��� ���� �ڽ��� ���
	{
		sibling->color = parent->color;
		parent->color = ECOLOR::BLACK;
		sibling->right->color = ECOLOR::BLACK;
		leftRotate(parent);
		return;
	}
	else if(ebNode == parent->right && sibling->left->color == ECOLOR::RED)
	{
		sibling->color = parent->color;
		parent->color = ECOLOR::BLACK;
		sibling->left->color = ECOLOR::BLACK;
		rightRotate(parent);
		return;
	}
	return;
}


void CMyRBTree::leftRotate(Node* node)
{
	Node* left = node->left;
	Node* right = node->right;
	if (node->right == nil)
		return;

	if (node == root)
		root = right;
	
	right->parent = node->parent;
	if (node->parent != nullptr)
	{
		if (node == node->parent->left)			// ȸ���� ���� ��尡 �θ��� ���� �ڽ��� ��� 
			node->parent->left = right;
		else									// ȸ���� ���� ��尡 �θ��� ������ �ڽ��� ���
			node->parent->right = right;
	}
	node->parent = right;

	right->left->parent = node;
	node->right = right->left;

	right->left = node;

	return;
}

void CMyRBTree::rightRotate(Node* node)
{
	Node* left = node->left;
	Node* right = node->right;
	if (node->left == nil)
		return;

	if (node == root)
		root = left;
	
	left->parent = node->parent;
	if(node->parent != nullptr)
	{
		if (node == node->parent->left)			// ȸ���� ���� ��尡 �θ��� ���� �ڽ��� ��� 
			node->parent->left = left;
		else									// ȸ���� ���� ��尡 �θ��� ������ �ڽ��� ���
			node->parent->right = left;
	}
	node->parent = left;

	left->right->parent = node;
	node->left = left->right;

	left->right = node;

	return;
}

CMyRBTree::Node* CMyRBTree::findNode(int findKey)
{
	if (root == nullptr)
		return nullptr;

	Node* searchNode = root;
	while (searchNode != nil)
	{
		if (searchNode->key > findKey)
			searchNode = searchNode->left;
		else if (searchNode->key < findKey)
			searchNode = searchNode->right;
		else
			return searchNode;
	}
	return nullptr;
}
