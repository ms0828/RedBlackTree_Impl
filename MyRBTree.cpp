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

	// 삽입할 노드의 부모 노드 탐색 
	Node* curNode = root;
	Node* parentNode = root;
	while (curNode != nil)
	{
		// 중복 삽입 허용 X
		if (insertKey == curNode->key)
			return false;

		parentNode = curNode;
		if (insertKey < curNode->key)
			curNode = curNode->left;
		else
			curNode = curNode->right;
	}

	// insertKey < parentNode 이면 왼쪽 자식에 삽입
	// insertKey > parentNode 이면 오른쪽 자식에 삽입
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
	// curNode가 루트라면 블랙으로 바꾼다.
	//  -> 2번 규칙 - 루트 노드는 블랙이어야 한다.
	if (curNode == root)
	{
		curNode->color = ECOLOR::BLACK;
		return;
	}

	// curNode의 부모 노드가 블랙이라면 규칙 성립
	if (curNode->parent->color == ECOLOR::BLACK)
		return;



	// ----------- 부모 노드가 레드 -> 4번 규칙 위반 ------------
	// 1. 삼촌이 블랙
	//	1-1. 삽입한 위치의 노드부터 할아버지까지의 경로가 일직선인 상태
	//  1-2. 삽입한 위치의 노드부터 할아버지까지의 경로가 꺾여있는 상태
	// 2. 삼촌이 레드
	// ----------------------------------------------------------

	// 할아버지, 부모, 삼촌 지정
	Node* grand = curNode->parent->parent;
	Node* parent = curNode->parent;
	Node* uncle = nullptr;
	if (grand->left == parent)
		uncle = grand->right;
	else
		uncle = grand->left;

	
	// 1. 삼촌이 블랙
	if (uncle->color == ECOLOR::BLACK)
	{
		// 1-1. 삽입한 위치의 노드부터 할아버지까지의 경로가 일직선인 상태
		//		- 부모와 할아버지의 색상을 교환
		//		- 할아버지 기준 반대편으로 회전
		//		- 자식은 삼촌의 자식으로
		if (parent == grand->left && curNode == parent->left) // 경로가 왼쪽으로 일직선
		{
			grand->color = ECOLOR::RED;
			parent->color = ECOLOR::BLACK;
			rightRotate(grand);
			return;
		}
		else if(parent == grand->right && curNode == parent->right) // 경로가 오른쪽으로 일직선
		{
			grand->color = ECOLOR::RED;
			parent->color = ECOLOR::BLACK;
			leftRotate(grand);
			return;
		}

		
		// 1-2. 삽입한 위치의 노드부터 할아버지까지의 경로가 꺾여있는 상태
		//		- 부모를 기준으로 삽입한 위치의 반대로 회전하여 1-1 case 만들기
		//		- 1-1 case 로직 그대로 수행 
		if (parent == grand->left && curNode == parent->right)
		{
			leftRotate(parent);
			grand->color = ECOLOR::RED;
			curNode->color = ECOLOR::BLACK; // (회전 후 curNode가 기존의 부모 위치에 서게 됨)  
			rightRotate(grand);
			return;
		}
		else if (parent == grand->right && curNode == parent->left)
		{
			rightRotate(parent);
			grand->color = ECOLOR::RED;
			curNode->color = ECOLOR::BLACK; // (회전 후 curNode가 기존의 부모 위치에 서게 됨)  
			leftRotate(grand);
			return;
		}
	}
	else
	{
		// 2. 삼촌이 레드
		//		- 레드를 반대편으로 넘길 수 없다.
		//		-> 부모, 삼촌과 할아버지의 색을 교환
		//		-> 할아버지 기준으로 다시 밸런싱 함수 수행
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

	if (removeNode->left == nil && removeNode->right == nil) // 삭제할 노드가 자식이 없을 때
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
		
		// 레드 블랙 트리 밸런싱 수행
		if (removeNode->color == ECOLOR::BLACK)
		{
			nil->parent = parentNode;
			deleteExtraBlack(nil);
			nil->parent = nullptr;
		}
		delete removeNode;
		return true;
	}
	else if (removeNode->left != nil && removeNode->right == nil) // 삭제할 노드가 왼쪽 자식만 가질 때
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
		
		// 레드 블랙 트리 밸런싱 수행
		if (removeNode->color == ECOLOR::BLACK)
			deleteExtraBlack(leftChild);
		
		delete removeNode;
		return true;
	}
	else if (removeNode->left == nil && removeNode->right != nil) // 삭제할 노드가 오른쪽 자식만 가질 때
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

		// 레드 블랙 트리 밸런싱 수행
		if (removeNode->color == ECOLOR::BLACK)
			deleteExtraBlack(rightChild);

		delete removeNode;
		return true;
	}
	else // 삭제할 노드가 자식이 2개 일 때
	{
		// 교체 후보자 찾기
		// -> 삭제할 노드의 오른쪽 자식 중에서 가장 작은 노드
		Node* curNode = removeNode->right;
		Node* successor = removeNode->right;
		while (curNode != nil)
		{
			successor = curNode;
			curNode = curNode->left;
		}

		// 교체 후보자와 삭제할 노드를 교체
		removeNode->key = successor->key;

		if (successor->right == nil) // 교체할 노드의 자식이 없을 때
		{
			if (successor == successor->parent->left) // 교체할 노드가 부모의 왼쪽 자식이면
				successor->parent->left = nil;
			else
				successor->parent->right = nil; // 교체할 노드가 부모의 오른쪽 자식이면

			// 레드 블랙 트리 밸런싱 수행
			if (successor->color == ECOLOR::BLACK)
			{
				nil->parent = successor->parent;
				deleteExtraBlack(nil);
				nil->parent = nullptr;
			}
		}
		else // 교체할 노드의 오른쪽 자식이 있을 때
		{
			if (successor == successor->parent->left) // 교체할 노드가 부모의 왼쪽 자식이면
				successor->parent->left = successor->right;
			else
				successor->parent->right = successor->right; // 교체할 노드가 부모의 오른쪽 자식이면

			successor->right->parent = successor->parent;

			// 레드 블랙 트리 밸런싱 수행
			if (successor->color == ECOLOR::BLACK)
				deleteExtraBlack(successor->right);
		}
		delete successor;
		return true;
	}
}


void CMyRBTree::deleteExtraBlack(Node* ebNode)
{
	// 1-1. ebNode가 루트 일 때
	// - ebNode가 레드라면 블랙으로 바꾼다.
	// - ebNode가 블랙이라면 곧바로 eb를 없앤다.
	// 1-2. ebNode가 RED일 때
	// - ebNode가 레드라면 블랙으로 바꾼다.
	if (ebNode == root || ebNode->color == ECOLOR::RED)
	{
		ebNode->color = ECOLOR::BLACK;
		return;
	}


	// 아래 부터는 ebNode가 루트가 아니며 블랙인 경우
	Node* parent = ebNode->parent;
	Node* sibling;
	if (ebNode == parent->left) // ebNode가 부모의 왼쪽 자식인 경우
		sibling = parent->right;
	else
		sibling = parent->left; // ebNode가 부모의 오른쪽 자식인 경우

	// 2. ebNode의 형제가 레드인 경우 (ebNode와 부모는 반드시 블랙)
	// - 형제를 블랙으로 유도한다.
	// - 부모와 형제의 색을 바꾼 후, 부모 기준으로 ebNode 방향으로 회전한다. 
	// - 깊이가 한 단계 내려간 ebNode의 형제가 블랙이 되었으므로 다시 밸런싱을 수행한다.
	if (sibling->color == ECOLOR::RED)
	{
		parent->color = ECOLOR::RED;
		sibling->color = ECOLOR::BLACK;
		if (ebNode == parent->left) // ebNode가 부모의 왼쪽 자식인 경우
		{
			leftRotate(parent);
			sibling = parent->right; // 3 or 4 or 5번 case로 이어서 갈 것이므로 형제를 다시 세팅
		}
		else // ebNode가 부모의 오른쪽 자식인 경우
		{
			rightRotate(parent);
			sibling = parent->left; // 3 or 4 or 5번 case로 이어서 갈 것이므로 형제를 다시 세팅
		}
	}

	// 3. ebNode의 형제가 블랙이고 형제의 양쪽 자식이 블랙
	// - ebNode의 eb와 형제의 black을 부모로 넘기고 부모를 기준으로 다시 밸런싱을 수행
	// - 이 때는 위로 올라가며 검사하는 로직이 수행된다.
	if (sibling->left->color == ECOLOR::BLACK && sibling->right->color == ECOLOR::BLACK)
	{
		sibling->color = ECOLOR::RED;
		deleteExtraBlack(parent);
		return;
	}

	// 4. ebNode의 형제가 블랙이고, 형제의 자식이 ebNode가 존재하는 방향으로 레드로 존재하는 경우 (반대 방향 자식은 블랙으로 존재)
	// - 형제를 기준으로 반대로 회전하여 5번 case로 만든다.
	//		 - 5번 case : 형제의 자식이 ebNode가 존재하는 방향과 반대로 존재하며 레드인 경우
	if (ebNode == parent->left && sibling->right->color == ECOLOR::BLACK && sibling->left->color == ECOLOR::RED) // ebNode가 부모의 왼쪽 자식인 경우
	{
		sibling->color = ECOLOR::RED;
		sibling->left->color = ECOLOR::BLACK;
		rightRotate(sibling); 
		sibling = parent->right; // 5번 case로 이어서 갈 것이므로 형제를 다시 세팅
	}
	else if (ebNode == parent->right && sibling->left->color == ECOLOR::BLACK && sibling->right->color == ECOLOR::RED) // ebNode가 부모의 오른쪽 자식인 경우
	{
		sibling->color = ECOLOR::RED;
		sibling->right->color = ECOLOR::BLACK;
		leftRotate(sibling);
		sibling = parent->left; // 5번 case로 이어서 갈 것이므로 형제를 다시 세팅
	}


	// 5. ebNode의 형제가 블랙이고, 형제의 자식이 ebNode가 존재하는 반대편 방향으로 레드로 존재하는 경우 (형제의 자식이 둘 다 레드일 때도 포함)
	// - 형제의 색을 부모의 색으로 바꾼다.
	// - 부모와 형제의 반대편 방향 자식을 블랙으로 바꾼다.
	// - 부모 기준으로 ebNode 방향으로 회전을 수행한다.
	if (ebNode == parent->left && sibling->right->color == ECOLOR::RED) // ebNode가 부모의 왼쪽 자식인 경우
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
		if (node == node->parent->left)			// 회전의 기준 노드가 부모의 왼쪽 자식일 경우 
			node->parent->left = right;
		else									// 회전의 기준 노드가 부모의 오른쪽 자식일 경우
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
		if (node == node->parent->left)			// 회전의 기준 노드가 부모의 왼쪽 자식일 경우 
			node->parent->left = left;
		else									// 회전의 기준 노드가 부모의 오른쪽 자식일 경우
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
