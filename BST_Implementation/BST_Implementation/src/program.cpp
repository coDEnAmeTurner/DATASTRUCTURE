#include <iostream>
#include <queue>

struct Node
{
	int data;
	Node* left;
	Node* right;
	~Node()
	{
		delete this->left, this->right;
	}
};
Node* getNewNode (int data)
{
	Node* root = new Node();
	root->data = data;
	root->left = root->right = NULL;
	return root;
}
Node* Insert (Node* root, int data)
{
	if (root == NULL)
		root = getNewNode(data);
	else if (data <= root->data)
		root->left = Insert(root->left, data);
	else
		root->right = Insert(root->right, data);
	return root;
}
int findMin(Node* root)
{
	while (root->left != NULL)
	{
		root = root->left;
	}
	return root->data;
}
int findMax(Node* root)
{
	while (root->right != NULL)
	{
		root = root->right;
	}
	return root->data;
}
int maxInTwo(int a, int b)
{
	if (a > b)
		return a;
	return b;
}
int findHeight(Node* root)
{
	if (root == NULL)
		return -1;
	return maxInTwo(findHeight(root->left), findHeight(root->right)) + 1;
}
void levelOrder (Node* root)
{
	if (root == NULL)
		std::cout << "There's no node in the Tree.\n";
	else
	{
		std::queue<Node*> Q;
		Q.push(root);
		while (!Q.empty())
		{
			std::cout << Q.front()->data << " ";
			if (Q.front()->left != NULL) Q.push(Q.front()->left);
			if (Q.front()->right != NULL) Q.push(Q.front()->right);
			Q.pop();

		}
		std::cout << "\n";
	}
}
void preOrder (Node* root)
{
	if (root == NULL)
		return;
	std::cout << root->data << " ";
	preOrder(root->left);
	preOrder(root->right);
}

bool BSTutil(Node* root, int max, int min)
{
	if (root == NULL)
		return true;
	if (root->data < max && root->data >= min && BSTutil(root->left, root->data, min) && BSTutil(root->right, max, root->data))
		return true;
	return false;
}
bool isBinarySearchTree(Node* root)
{
	return BSTutil(root, INT_MAX, INT_MIN);
}
Node* root = NULL;

int main()
{
	root = Insert(root, 10);
	root = Insert(root, 5);
	root = Insert(root, 15);
	root = Insert(root, 20);
	root = Insert(root, 25);
	std::cout << isBinarySearchTree(root);
	delete root;

	std::cin.get();
}