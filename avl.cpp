#include<bits/stdc++.h>

using namespace std;
// An AVL tree node
struct node
{
	int key;
	struct node *left;
	struct node *right;
	int height;
};

// A utility function to get maximum of two integers
int max(int a, int b);

// A utility function to get height of the tree
int height(struct node *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

// A utility function to get maximum of two integers
int max(int a, int b)
{
	return (a > b)? a : b;
}

/* Helper function that allocates a new node with the given key and
   NULL left and right pointers. */
struct node* newNode(int key)
{
	struct node* node = (struct node*)
		malloc(sizeof(struct node));
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->height = 1; // new node is initially added at leaf
	return(node);
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct node *rightRotate(struct node *y)
{
	struct node *x = y->left;
	struct node *T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height(y->left), height(y->right))+1;
	x->height = max(height(x->left), height(x->right))+1;

	// Return new root
	return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct node *leftRotate(struct node *x)
{
	struct node *y = x->right;
	struct node *T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height(x->left), height(x->right))+1;
	y->height = max(height(y->left), height(y->right))+1;

	// Return new root
	return y;
}

// Get Balance factor of node N
int getBalance(struct node *N)
{
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}

struct node* insert(struct node* node, int key)
{
	/* 1. Perform the normal BST rotation */
	if (node == NULL)
		return(newNode(key));

	if (key < node->key)
		node->left = insert(node->left, key);
	else
		node->right = insert(node->right, key);

	/* 2. Update height of this ancestor node */
	node->height = max(height(node->left), height(node->right)) + 1;

	/* 3. Get the balance factor of this ancestor node to check whether
	   this node became unbalanced */
	int balance = getBalance(node);

	// If this node becomes unbalanced, then there are 4 cases

	// Left Left Case
	if (balance > 1 && key < node->left->key)
		return rightRotate(node);

	// Right Right Case
	if (balance < -1 && key > node->right->key)
		return leftRotate(node);

	// Left Right Case
	if (balance > 1 && key > node->left->key)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && key < node->right->key)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

// A utility function to print preorder traversal of the tree.
// The function also prints height of every node
void preOrder(struct node *root)
{
	if(root != NULL)
	{
		printf("%d ", root->key);
		preOrder(root->left);
		preOrder(root->right);
	}
}

int search(node* head, int val)
{
	if(head == NULL)
		return -1;
	else if(head->key>val)
		return search(head->left,val);
	else if(head->key<val)
		return search(head->right,val);
	else
		return 1;

}
/* Drier program to test above function*/
int main(int argc, char* argv[])
{
	if(argc <2)
	{
		printf("Enter filename!!!\n");
		return 0;
	}

	struct node* root = NULL;

	int arr[2000016];
	srand(time(NULL));
	int k=0,ans;
	int randno;


	ifstream myfile;
	myfile.open("database.txt");				//Input file for keys

	for(int i=0;i<1048576;i++)
	{
		myfile>>arr[i];
	}
	myfile.close();

	for(int i=0;i<1048576;i++)
	{
		root = insert(root,arr[i]);
	}
	int query = pow(2,21);
	ifstream myfile1;
	myfile1.open(argv[1]);						//Input file for search queries

	clock_t startTime = clock();
	for(int i=0;i<query;i++)
	{
//		scanf("%d\n",&randno);
//		randno = rand()%1048576;
		myfile1>>randno;
		ans = search(root,randno);
	}
	float secsElapsed = (float)(clock() - startTime)/CLOCKS_PER_SEC;
	printf("%f\n",secsElapsed);
	myfile.close();
	return 0;
}
