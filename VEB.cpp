#include<bits/stdc++.h>
#define ll long long

using namespace std;



struct veb_node
{
	int data;									//type can be changed as per need- for modification
	int parent;
	int left;
	int right;
	bool dummy;
};



vector<int> inp_array;

veb_node arr_veb[1048576];					// You could declare these three arrays according to the memory you have
int binary[1048576];							// I declared them as 2^18 due to memory constraints.

int index_veb;

class VEB
{
public:
	int nodes_tree;
	int levels;
	int ind, num;
	int cnt;


	VEB()
	{
		nodes_tree = 1048576;num=0;						// change as per the height of the tree you want
		levels = 20;									// nodes_tree = 2^levels
		veb_init();
	}


	void veb_init()
	{
		ind = 1;
		veb_assign(1, nodes_tree, levels, 1, levels);
	}

	void veb_assign(int start, int end, int height, int curht, int maxht)				// fucntion to define the VEB layout for a given height
	{
		int mid = (start+end)/2;

		if(height == 1)
		{
			binary[mid] = ind;
			arr_veb[ind].data = INT_MIN;
			arr_veb[ind].dummy = true;
			arr_veb[ind].left = 0;
			arr_veb[ind].right = 0;
			ind++;
		}
		else
		{
			int half = height/2;
			int secondhalf=height-half;
			int count=0;

			int nextht=pow(2,half);
			int mx=pow(2,maxht-half)-1;
			int x=0;
			veb_assign( start, end, half, curht, maxht);

			for(int i=0;i<nextht;i++)
			{
				veb_assign( start, start+mx-1, secondhalf, curht+half, maxht-half );

				if(count%2)
				{	
					arr_veb[binary[start-1]].right = binary[((2*start)+mx-1)/2];			
					arr_veb[binary[((2*start)+mx-1)/2]].parent = binary[start-1];
				}
				else
				{
					arr_veb[binary[start+mx]].left = binary[((2*start)+mx-1)/2];		
					arr_veb[binary[((2*start)+mx-1)/2]].parent = binary[start+mx];
				}
				count++;
				start=start+mx+1;
			}
		}
	}

	void insert(int* arr, int size)
	{
		ind = 1;
//		veb_assign(1, nodes_tree, levels, 1, levels);
		num = 0;
		sort(arr,arr+size);
		veb_insert_new(arr,0,size-1);
	}

	void insert_util(int val, int key)									// for insert- change type of val as per the dataset
	{
		if(key == 0)
			return;
		else if(arr_veb[key].dummy == true)
		{
			arr_veb[key].dummy = false;
			arr_veb[key].data = val;
		}
		else if(arr_veb[key].data > val)
		{
			insert_util(val, arr_veb[key].left);
		}
		else
		{
			insert_util(val, arr_veb[key].right);
		}
	}

	void veb_insert_new(int* arr,int s, int e)
	{
		if(s > e)
			return;
		int mid = (s+e)/2;
		insert_util(arr[mid],1);
		veb_insert_new(arr,s,mid-1);
		veb_insert_new(arr,mid+1,e);
	}



	int search(int val)													// for search- change type of val as per the dataset
	{
		int x = search_util(val,1);
		return x;
	}

	int search_util(int val, int key)										// for search- change type of val as per the dataset
	{
		if(key == 0)
			return -1;
		else if(arr_veb[key].dummy == true)
			return -1;
		else if(arr_veb[key].data == val)
			return 1;
		else if(arr_veb[key].data > val)
			return search_util(val, arr_veb[key].left);
		else
			return search_util(val, arr_veb[key].right);
	}

	void traverse(int key)
	{
		if(key == 0)
			return;
		if(arr_veb[key].dummy == true)
			return;
		else
		{
			printf("%d\n",arr_veb[key].data);							//for printing- change type %d as per dataset
			traverse(arr_veb[key].left);
			traverse(arr_veb[key].right);
		}
	}

};


int ar1[1048579];
int main()
{

	ifstream myfile;
	myfile.open("database.txt");				//Input file containing the keys to be entered

	for(int i=0;i<1048576;i++)
	{
		myfile>>ar1[i];
	//	printf("%d\n", ar1[i]);
	}
	myfile.close();

/*	for(int i=0;i<1048576;i++)
		ar1[i] = i+1;
*/	srand(time(NULL));

	clock_t startTime1 = clock();
	VEB v;

	v.insert(ar1,1048576);						// Insert takes the input array to be insert and the size of the array as arguements
	float secsElapsed1 = (float)(clock() - startTime1)/CLOCKS_PER_SEC;
	printf("%f\n", secsElapsed1);	

	int query = pow(2,20),randno;
	ifstream myfile1;
	myfile1.open("Input9.txt");					//Input file containing queries


	for(int i=0;i<query;i++)					
	{
		myfile1>>randno;
		v.search(randno);						// For searching
	}

	return 0;
}



/* Using this code:

	In line 38, 39
	specify the next power of 2 of the no. of nodes you will have in the tree. Also specify the levels
	nodes_tree should be equal to 2^levels.
	Ensure that the array sizes in line 28,29 are >= the no. of nodes in the tree.
	for insert, make an array. insert(int*, int)--------> int* = pointer to the array to be inserted. int = no. of elements in the array
	search: just specify the key to be inserted
	traverse: change to inorder, preoder or postorder as per needs: on line no. 162,163,164
	for traverse, always call tree_name.traverse(1)
*/