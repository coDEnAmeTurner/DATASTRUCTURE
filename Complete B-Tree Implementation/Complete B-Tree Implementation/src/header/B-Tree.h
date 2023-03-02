#include <iostream>
using namespace std;

struct node
{
	int* key;
	node** child;
	bool leaf;
	int n;

	node(int deg)
	{
		leaf = true; //value when "this" node is dedicated for a NULL root
		key = new int[deg] {INT_MIN}; //degree is max no. of children (order)
		child = new node * [deg + 1];
		for (int i = 0; i <= deg; i++)
		{
			child[i] = NULL;
		}
		n = 0;
	}
	void findNode(int data, node* temp, node*& temp2, int t);
	void splitChild(node* temp, int t);
	void printLevel(int lv);
	void printInOrder();
	void deleteKey(int data, node* parent, node*& root, int t);
	bool borrowLeft(node* parent, int t);
	bool borrowRight(node* parent, int t);
	void merge(node* c, int t);
	int findMax(node*& hasMax);
	int findMin(node*& hasMin);

	friend class BTree;
};

class BTree
{
private:
	node* root;
	int t; //the max degree
public:
	BTree(int deg)
	{
		root = NULL;
		t = deg;
	}	  
	void Insert(int data)
	{
		if (root == NULL)
		{
			root = new node(t); //we fully aware that max no. of keys is t - 1, but we set size of the array as t 
			//since we want an extra slot for easier analysis
			root->key[0] = data;
			root->n++;
		}
		else
		{
			root->findNode(data, root, root, t); 
		}
	}
	void deleteKey(int data)
	{
		if (root == NULL)
			return;
		root->deleteKey(data, root, root, t);
	}
	void printLevel(int lv)
	{
		root->printLevel(lv);
	}
	void printInOrder()
	{
		if (root == NULL)
		{
			return;
		}
		root->printInOrder();
	}
	int findMax()
	{
		node* clone;
		return root->findMax(clone);
	}
	int findMin()
	{
		node* clone;
		return root->findMin(clone);
	}
	bool isEmpty()
	{
		if (root == NULL)
		{
			return true;
		}
		return false;
	}
	void deleteAll()
	{
		if (root == NULL)
		{
			return;
		}
		while (!isEmpty())
		{
			root->deleteKey(root->key[0], root, root, t);
		}
	}
	~BTree()
	{
		if (root != NULL)
		{
			while (!isEmpty())
			{
				root->deleteKey(root->key[0], root, root, t);
			}

		}
	}
};

//find the suitable node to insert data
//this function will do both finding and insertion
//the node calling can be the suitable node if it is a leaf
//node_active is the current node
//node* root will store root's address at whatever recursion 
void node::findNode(int data, node* node_active, node*& root, int t) 
{	
	if (this == NULL)
	{
		return;
	}
	if (leaf)
	{
		int i = n;
		while (i > 0 && key[i - 1] > data) //wherever i ends up after the interation, it is the suitable one
		{
			key[i] = key[i - 1];
			i--; 
		}
		key[i] = data;
		n++;
	}
	else //we want to find the suitable CHILD
	{
		int i = 0; 
		while (i < n && key[i] < data)
		{
			i++;
		}

		//no need to check if the loop terminates at the right child
		//we also want to keep track of child[i]'s parent, which is THIS node, so we will pass "this" to node_active, whose name is exactly what "this" is about.
		//So at the point of this recursion, we are aware what the child is (the calling node), what the child's parent is (node_active)
		child[i]->findNode(data, this, root, t); 

	}

	//we split "this" node when its n == t
	// n reaches t and we split, meaning n can go as far as t - 1 => B Tree properties preserved though the size of key array is t.
	//we put this algo seperately since anything can be split either this or its ancestors
	if (n == t)
	{
		if (this == root) //if this current node is root, and the root is full
		{
			node* s = new node(t);
			s->leaf = false;
			s->child[0] = this;
			s->splitChild(this, t);//because "this" is the root, and we want to split the root
			root = s;
		}
		else
		{
			//here's where node_active kicks in. Remember when we save the parent in node_active? 
			//Now we'll use that parent aka node_active to split "this", since "this" is node_active's child

			node_active->splitChild(this, t);
		}
	}
}

//fullNode is the child that is full
void node::splitChild(node* fullNode, int t)
{
	//first create the right half of the child which is to be splitted
	node* right = new node(t);
	int move_up = (t - 1) / 2; //the middle key to be moved up to the parent

	//we will move every key to the right of middle key, to the right node:
	int nBefore = fullNode->n;
	for (int i = move_up + 1; i < nBefore; i++)
	{
		//directly use the current right's n value, since it perfectly indicates the index that we will add fullNode's key[i] to.
		right->key[right->n] = fullNode->key[i]; 
		right->n++;
		fullNode->n--; //the key we just add to right, no longer exist in fullNode
	}

	//if fullNode has children, we will split the children
	//if it has children, it must have max no. of children = t + 1
	if (fullNode->leaf == false)
	{
		int child_i = 0;
		//t / 2 since the no. of children is t + 1, the middle child will be [(t + 1) - 1]/ 2
		for (int walk_child = t / 2; walk_child <= t; walk_child++) 
		{
			right->child[child_i] = fullNode->child[walk_child];
			fullNode->child[walk_child] = NULL;	
			child_i++;	
		}

		//right node is at the same level as full node, so right's leaf = fullnode's leaf
		right->leaf = fullNode->leaf;
	}

	//now we make the right node a child to the parent 
	//since the parent might have other nodes too, so we must make space for this new right node
	int full_index = n + 1;
	while (child[full_index] != fullNode)
	{
		child[full_index] = child[full_index - 1];
		full_index--;
	}
	child[full_index + 1] = right;

	//now add the middle key of fullnode to the parent
	int middle_i = n;
	while (middle_i > 0 && key[middle_i - 1] > fullNode->key[move_up])
	{
		key[middle_i] = key[middle_i - 1];
		middle_i--;
	}
	key[middle_i] = fullNode->key[move_up];
	n++;
	fullNode->n--; 
}

void node::printLevel(int lv)	
{
	if (lv > 0 && leaf)
	{
		cout << "Empty" << endl;
	}
	else if (lv == 0)
	{
		for (int i = 0; i < n; i++)
		{
			cout << key[i] << " ";
		}
	}
	else
	{
		for (int i = 0; i < n + 1; i++)
		{
			child[i]->printLevel(lv - 1);
		}
	}
}

void node::printInOrder()
{
	if (this == NULL)
	{
		return;
	}
	for (int i = 0; i < n; i++)
	{
		child[i]->printInOrder();
		cout << key[i] << " ";
	}
	child[n]->printInOrder();
}

void node::deleteKey(int data, node* parent, node*& root, int t)
{
	if (this == NULL)
	{
		return;
	}
	int i = 0;
	while (i < n && key[i] < data) {
		i++;
	}
	bool isRoot = (this == root) ? true : false; //to confirm at this line, "this" is root

	if (i != n)
	{
		if (data == key[i])
		{
			if (leaf == true)
			{
				//delete the key in the array
				if (i == n - 1)
				{
					n--;
					if (this == root)
					{
						delete root;
						root = NULL;
					}
				}
				else
				{
					for (int move_left = i; move_left < n - 1; move_left++)
					{
						key[move_left] = key[move_left + 1];
					}
					n--;
				}
			}
			else
			{
				node* hasMax = NULL;
				int max = child[i]->findMax(hasMax);
				if (hasMax->n <= ceil(t / 2) - 1)
				{
					node* hasMin = NULL;
					int min = child[i + 1]->findMin(hasMin);
					if (hasMin->n <= ceil(t / 2) - 1)
					{
						key[i] = max;
						child[i]->deleteKey(max, this, root, t);
					}
					else
					{
						key[i] = min;
						child[i + 1]->deleteKey(min, this, root, t);
					}
				}
				else
				{
					key[i] = max;
					child[i]->deleteKey(max, this, root, t);
				}

			}
		}
		else
		{
			child[i]->deleteKey(data, this, root, t);
		}
	}
	else
	{
		child[i]->deleteKey(data, this, root, t);
	}

	//after the above execution, "this" might not be root anymore, root is shifted a child lower
	//we have to check if before "this" is not root and NOW THIS IS ALSO NOT ROOT, to continue execution
	if (isRoot == false && this != root) 
	{
		if (n < ceil(t / 2) - 1) //since t is max degree (order), min no. of keys possible is ceil(t / 2) - 1 => B Tree properties preserved
		{
			if (borrowLeft(parent, t) == false)
			{
				if (borrowRight(parent, t) == false)
				{
					parent->merge(this, t);

					if (parent == root && parent->n < 1)
					{
						node* prevRoot = root;
						root = root->child[0]; //this is obvious, since the root's n = 0 so it only has 1 child, here's the child: child[0]
						delete prevRoot;
						prevRoot = NULL;
					}
				}
			}
		}

	}

}

int node::findMax(node*& hasMax)
{
	if (this->leaf == true)
	{
		hasMax = this;
		return key[n - 1];
	}
	
	return child[n]->findMax(hasMax);
}

int node::findMin(node*& hasMin)
{
	if (this->leaf == true)
	{
		hasMin = this;
		return key[0];
	}

	return child[0]->findMin(hasMin);
}

bool node::borrowLeft(node* parent, int t)
{
	//"this" can't be NULL
	if (this == NULL)
	{
		return false;
	}
	//find "this" in parent
	int indexOfThis = INT_MIN;
	for (int i = 0; i <= parent->n; i++)
	{
		if (parent->child[i] == this)
		{
			indexOfThis = i;
			break;
		}
	}

	if (indexOfThis > 0)
	{
		//borrow child to the left of "this"
		int idxLeftOfThis = indexOfThis - 1;

		if (parent->child[idxLeftOfThis]->n <= ceil(t / 2) - 1) //if the left of this is lacking key too
		{
			return false;
		}
		
		//no need to search for where to insert the parent's key, since the index to be inserted is always at the head
		//make space for the borrowed key on the left
		for (int i = n; i > 0; i--)
		{
			key[i] = key[i - 1];
		}
		key[0] = parent->key[idxLeftOfThis]; n++;
		parent->key[idxLeftOfThis] = parent->child[idxLeftOfThis]->key[parent->child[idxLeftOfThis]->n - 1]; //here is the key on the left to be borrowed
		if (leaf == false) //if it is not leaf node, shift the child following the borrowed key
		{
			for (int i = n + 1; i > 0; i--)
			{
				child[i] = child[i - 1];
			}
			child[0] = parent->child[idxLeftOfThis]->child[parent->child[idxLeftOfThis]->n];
			parent->child[idxLeftOfThis]->child[parent->child[idxLeftOfThis]->n] = NULL;
		}

		//delete both the borrowed key on the left and the child following
		parent->child[idxLeftOfThis]->n--;

		return true;
	}
	return false;
}

bool node::borrowRight(node* parent, int t)
{
	//"this" can't be NULL
	if (this == NULL)
	{
		return false;
	}
	//find "this" in parent
	int indexOfThis = INT_MIN;
	for (int i = 0; i <= parent->n; i++)
	{
		if (parent->child[i] == this)
		{
			indexOfThis = i;
			break;
		}
	}

	if (indexOfThis == parent->n)
	{
		return false;
	}

	if (indexOfThis != INT_MIN)
	{
		//borrow child to the right of "this"
		int idxRightOfThis = indexOfThis + 1;
		if (parent->child[idxRightOfThis]->n <= ceil(t / 2) - 1) //if the right of this is lacking key too
		{
			return false;
		}

		//int insertHere = n;
		//while (key[insertHere - 1] > parent->key[indexOfThis]) //find a place in "this" to put the parent's key in
		//{
		//	key[insertHere] = key[insertHere - 1];	
		//	insertHere--;
		//}
		
		//no need to make space, since the index to be inserted is always at the tail 
		key[n] = parent->key[indexOfThis]; n++;
		parent->key[indexOfThis] = parent->child[idxRightOfThis]->key[0]; //here is the key on the right to be borrowed

		if (leaf == false) //if it is not leaf node, shift the child following the borrowed key
		{
			child[n] = parent->child[idxRightOfThis]->child[0];

			//delete the child following the key to be borrowed from the right
			for (int i = 0; i < parent->child[idxRightOfThis]->n; i++)
			{
				parent->child[idxRightOfThis]->child[i] = parent->child[idxRightOfThis]->child[i + 1];
			}
			parent->child[idxRightOfThis]->child[parent->child[idxRightOfThis]->n] = NULL;
		}

		//delete the key to be borrowed from the right
		for (int i = 0; i < parent->child[idxRightOfThis]->n - 1; i++) 
		{
			parent->child[idxRightOfThis]->key[i] = parent->child[idxRightOfThis]->key[i + 1];
		}
		parent->child[idxRightOfThis]->n--;

		return true;
	}
	return false;

}

void node::merge(node* c, int t)
{
	//find child in parent
	int iChild;
	for (iChild = 0; iChild <= n; iChild++)
	{
		if (child[iChild] == c)
		{
			break;
		}
	}

	//find the parent's key to be merged
	int iParent = (iChild == 0) ? iChild : iChild - 1;

	//find the adjacent child to be merged
	int iAdjacent = (iChild == 0) ? iChild + 1 : iChild - 1;

	//assign either child or adjacent to either left or right
	int iLeft = (iChild < iAdjacent) ? iChild : iAdjacent;
	int iRight = (iChild < iAdjacent) ? iAdjacent : iChild;

	//copy iParent key to iLeft
	child[iLeft]->key[child[iLeft]->n] = key[iParent];
	child[iLeft]->n++;
	//delete iParent
	for (int i = iParent; i < n - 1; i++)
	{
		key[i] = key[i + 1];
	}

	//copy key and the child (of right child) from the right child to the left child
	int prevRightN = child[iRight]->n;
	for (int i = 0; i < prevRightN; i++)
	{
		child[iLeft]->key[child[iLeft]->n] = child[iRight]->key[i];
		child[iLeft]->child[child[iLeft]->n] = child[iRight]->child[i];
		child[iLeft]->n++;
		child[iRight]->n--;
		if (i == prevRightN - 1)
		{
			child[iLeft]->child[child[iLeft]->n] = child[iRight]->child[prevRightN];
		}
	}

	//delete the right child
	delete child[iRight];
	child[iRight] = NULL;
	for (int i = iRight; i < n; i++) //must delete the right child from parent's child array
	{
		child[i] = child[i + 1];
	}
	n--; //no. of keys from parent drops, bring no. of chilren drop relatively

}
