#include<bits/stdc++.h>
#define SI(n) scanf("%d",&n)
#define SII(a,b) scanf("%d%d",&a,&b)
#define SIII(a,b,c) scanf("%d%d%d",&a,&b,&c)
#define SLL(n) scanf("%lld",&n)
#define SC(r) scanf("%c",&r)
#define SS(r) scanf("%s",r)
#define SD(r) scanf("%lf",&r)
#define REPA(i,a,n) for(int i=a;i<n;i++)
#define REP(i,n) for(int i=0;i<n;i++)
#define PI(n) printf("%d\n",n)
#define PII(a,b) printf("%d %d\n",a,b)
#define PLL(n) printf("%lld\n",n)
#define PC(n) printf("%c\n",n)
#define PS(n) printf("%s\n",n)
#define PD(n) printf("%lf\n",n)
#define ll long long
#define pb push_back
#define mp make_pair
#define M 1000000007

using namespace std;

struct node_path
{
	int v_l;
	int v_r;
	int val;
	int key;
	int p_l;
	int p_r;
};

struct path_wrapper
{
	node_path parr[25];
};

struct node_veb
{
	int p;
	int l;
	int r;
	int val;
	int key;
	bool leaf;
};

struct cache_node
{
	int val;
	int key;
	int pl,pr;
	int pathno_l;
	int pathno_r;
	int c_ind_l;
	int c_ind_r;
	int parent;
	int true_arr;
};

void veb_initialize(node_veb*,int,int,int,int,int);
int binarysearch_veb(node_veb*,int,int);
//void insert(node_veb*, node_array*, int);
void level_order_veb(node_veb*,int);
void traverse(node_veb*,node_path*, int,int);
void merge(node_veb*);
void make_path_array(node_veb*, int, int);
void split(node_veb* arr_veb, int path_no, int dir, int side, int key, int value);
int find_node(int c_index, int value);
void right_rotate(int n1);
void left_rotate(int n1);
void rotate(int num);
void split_main(node_veb* arr_veb, int value);
void initialize_path(path_wrapper* path);
void initialize_cache(cache_node* c);
int search(int r_num, node_veb* arr_veb, int value);

int ht,n,pst=0;									//ht = ht of veb tree = no. of nodes in one node of tree bounds = ht ans 2ht-1
int maxnum=1;
int leaves=1048576;

bool pl=false,pr=false,pl1=false,pr1=false;

node_veb layer1[2097152];									//veb with max height 20


//For packed array densities
double Tmin=0.5, Tmax=1;									//min and max for maximum densities
double Dmin=0.125, Dmax=0.25;									//min and max for minimum densities
double blocks=log2(leaves);

//required for vEb initialize
int binary[2097152],leaf_tree[1048576];
int ind=1;
int cnt;

//reqd for splits
int splits=0,lc_p;
int path_start=0, path_end=0,max_path = 1000, path_len=20;
path_wrapper pathl[84000],pathr[84000];

cache_node cache[8401],root;
int root_index=-1;
bool cache_full = false;
int cache_start=0,cache_end=0,cache_size=6000,path_size=80000;
//int max_cache[4]={2,4,16,256},curlevel_cache=0,cache_start[4],cache_end[4],ele_cache[4];

int time_st = 0;
int cnt_init = 0;



/* For insertion */
void init_veb(node_veb* arr_veb,int key)
{
//	PI(key);
	if(arr_veb[key].leaf == true)
	{
		arr_veb[key].val = leaf_tree[cnt_init++];
	}
	else
	{
		init_veb(arr_veb, arr_veb[key].l);
//		arr_veb[key].val = leaf_tree[cnt_init++];
		init_veb(arr_veb, arr_veb[key].r);
		arr_veb[key].val = min(arr_veb[arr_veb[key].l].val,arr_veb[arr_veb[key].r].val);

	}
}

/*Function to convert binary order to veb to assign children and parent to each node;

  height = height of the current subtree in picture
  curht = height of the current root node from the top of the tree
  maxht = height of the of the biggest possible subtree this node can be root of
*/

void veb_initialize(node_veb* arr_veb, int start, int end, int height, int curht, int maxht)
{	
	pst++;
	int mid=(start+end)/2;

	if(height == 1)
	{
		binary[mid]=ind++;
		arr_veb[ind-1].val=INT_MAX;
		arr_veb[ind-1].leaf=false;
		arr_veb[ind-1].key=0;
		if(curht==ht)
		{	arr_veb[ind-1].leaf = true;
		}
	}
	else
	{
		int half=height/2;
		int secondhalf=height-half;

		int count=0;

		int nextht=pow(2,half);
		int mx=pow(2,maxht-half)-1;

		veb_initialize(arr_veb,start,end,half,curht,maxht);
	
		REP(i,nextht)
		{
			veb_initialize(arr_veb,start,start+mx-1,secondhalf,curht+half,maxht-half);

			if(count%2)
			{	
				arr_veb[binary[start-1]].r = binary[((2*start)+mx-1)/2];				//right child
				arr_veb[binary[((2*start)+mx-1)/2]].p = binary[start-1];
			}
			else
			{
				arr_veb[binary[start+mx]].l = binary[((2*start)+mx-1)/2];			//left child
				arr_veb[binary[((2*start)+mx-1)/2]].p = binary[start+mx];
			}
			count++;
			start=start+mx+1;
		}
	}
}

/* Binary search in VeB to find the node which has the largest value that is smaller than value*/

int binarysearch_veb(node_veb* arr_veb,int key, int value)
{
	if(key>0)
	{
		if(arr_veb[key].leaf == true)
			return key;
		else if(arr_veb[key].leaf == false && arr_veb[arr_veb[key].r].val <= value)
			return binarysearch_veb(arr_veb,arr_veb[key].r,value);
		else
			return binarysearch_veb(arr_veb,arr_veb[key].l,value);
	}
	return -1;
}

int binarysearch_new(node_veb* arr_veb, node_path* path, int loc_v, int loc_p,int value)
{
	int ans;
	pathl[path_end].parr[lc_p].v_l = -1;
	pathl[path_end].parr[lc_p].v_r = -1;
	pathl[path_end].parr[lc_p].p_l = -1;
	pathl[path_end].parr[lc_p].p_r = -1;
	pathl[path_end].parr[lc_p].val = INT_MAX;
	pathl[path_end].parr[lc_p].key = 1;

	pathr[path_end].parr[lc_p].v_l = -1;
	pathr[path_end].parr[lc_p].v_r = -1;
	pathr[path_end].parr[lc_p].p_l = -1;
	pathr[path_end].parr[lc_p].p_r = -1;
	pathr[path_end].parr[lc_p].val = INT_MAX;
	pathr[path_end].parr[lc_p].key = 1;

	if(loc_v>-1)
	{

		if(arr_veb[loc_v].leaf == true && arr_veb[loc_v].val == value)
		{
			arr_veb[loc_v].key = 1;
			return loc_v;
		}
		else if(arr_veb[loc_v].leaf == false && (arr_veb[arr_veb[loc_v].r].val != INT_MAX && arr_veb[arr_veb[loc_v].r].val <= value))
		{	
			pathl[path_end].parr[lc_p].p_r = lc_p+1;
			pathr[path_end].parr[lc_p].p_r = lc_p+1;
			pathl[path_end].parr[lc_p].v_l = arr_veb[loc_v].l;
			lc_p++;
			ans =  binarysearch_new(arr_veb, path, arr_veb[loc_v].r, loc_p, value);
			lc_p--;
			pathl[path_end].parr[lc_p].val = min(arr_veb[arr_veb[loc_v].l].val,pathl[path_end].parr[lc_p+1].val);
			pathr[path_end].parr[lc_p].val = pathr[path_end].parr[lc_p+1].val;
			return ans;
		}
		else if(arr_veb[loc_v].leaf == false && (arr_veb[arr_veb[loc_v].l].val == INT_MAX || arr_veb[arr_veb[loc_v].l].val <= value))
		{
			pathl[path_end].parr[lc_p].p_l = lc_p+1;
			pathr[path_end].parr[lc_p].p_l = lc_p+1;
			pathr[path_end].parr[lc_p].v_r = arr_veb[loc_v].r;
			lc_p++;

			ans = binarysearch_new(arr_veb, path, arr_veb[loc_v].l, loc_p, value);
			lc_p--;
			pathr[path_end].parr[lc_p].val = min(arr_veb[arr_veb[loc_v].r].val,pathr[path_end].parr[lc_p+1].val);
			pathl[path_end].parr[lc_p].val = pathl[path_end].parr[lc_p+1].val;
			return ans;
		}
	}
	else if(loc_p>-1)
	{
		if((path[loc_p].p_r != -1) && path[path[loc_p].p_r].val <= value)
		{
			pathl[path_end].parr[lc_p].p_r = lc_p+1;
			pathr[path_end].parr[lc_p].p_r = lc_p+1;
			pathl[path_end].parr[lc_p].v_l = path[loc_p].v_l;
			lc_p++;
			ans = binarysearch_new(arr_veb, path, -1, path[loc_p].p_r, value);
			lc_p--;
			pathl[path_end].parr[lc_p].val = min(arr_veb[arr_veb[loc_v].l].val,pathl[path_end].parr[lc_p+1].val);
			pathr[path_end].parr[lc_p].val = pathr[path_end].parr[lc_p+1].val;
			return ans;

		}
		else if((path[loc_p].v_r != -1) && arr_veb[path[loc_p].v_r].val <= value)
		{
			pathl[path_end].parr[lc_p].p_r = lc_p+1;
			pathr[path_end].parr[lc_p].p_r = lc_p+1;
			pathl[path_end].parr[lc_p].v_l = path[loc_p].v_l;
			lc_p++;

			ans = binarysearch_new(arr_veb, path, path[loc_p].v_r, -1, value);
			lc_p--;
			pathl[path_end].parr[lc_p].val = min(arr_veb[arr_veb[loc_v].l].val,pathl[path_end].parr[lc_p+1].val);
			pathr[path_end].parr[lc_p].val = pathr[path_end].parr[lc_p+1].val;
			return ans;

		}
		else if((path[loc_p].p_l != -1) && path[path[loc_p].p_l].val <= value)
		{
			pathl[path_end].parr[lc_p].p_l = lc_p+1;
			pathr[path_end].parr[lc_p].p_l = lc_p+1;
			pathr[path_end].parr[lc_p].v_r = path[loc_p].v_r;
			lc_p++;
			ans = binarysearch_new(arr_veb, path, -1, path[loc_p].p_l, value);
			lc_p--;
			pathr[path_end].parr[lc_p].val = min(arr_veb[arr_veb[loc_v].r].val,pathr[path_end].parr[lc_p+1].val);
			pathl[path_end].parr[lc_p].val = pathl[path_end].parr[lc_p+1].val;
			return ans;
		}
		else if((path[loc_p].v_l != -1) && arr_veb[path[loc_p].v_l].val <= value)
		{
			pathl[path_end].parr[lc_p].p_l = lc_p+1;
			pathr[path_end].parr[lc_p].p_l = lc_p+1;
			pathr[path_end].parr[lc_p].v_r = path[loc_p].v_r;
			lc_p++;

			ans = binarysearch_new(arr_veb, path, path[loc_p].v_l, -1, value);
			lc_p--;
			pathr[path_end].parr[lc_p].val = min(arr_veb[arr_veb[loc_v].r].val,pathr[path_end].parr[lc_p+1].val);
			pathl[path_end].parr[lc_p].val = pathl[path_end].parr[lc_p+1].val;
			return ans;

		}
	}
	return -1;
}

/*Functions required for Inserting in the data structure*/

/*Insertion ends*/

/*Traversal of different layers of  the structure*/

void level_order_veb(node_veb* tmp,int key)
{
	printf("\n\n############################LEVEL ORDER TRAVERSAL OF LAYER 1########################################\n\n");
	queue< pair<int,int> > q;
	
	int curlevel_veb=1;
	q.push(mp(key,1));
	while(!q.empty())
	{
		if(q.front().second > curlevel_veb)
		{
			curlevel_veb++;
			cout<<endl;
		}
		printf("%d ",tmp[q.front().first].val);

		if(tmp[q.front().first].leaf == false)
		{
			q.push(mp(tmp[q.front().first].l,curlevel_veb+1));
			q.push(mp(tmp[q.front().first].r,curlevel_veb+1));
		}
		q.pop();
	}	
	printf("\n\n\n\n");
}


void initialize_path(path_wrapper* path)
{
	REP(i,50)
	{
		(*path).parr[i].v_l = -1;
		(*path).parr[i].v_r = -1;
		(*path).parr[i].p_l = -1;
		(*path).parr[i].p_r = -1;
		(*path).parr[i].val = INT_MAX;
	}
}


void initialize_cache(cache_node* c)
{
	(*c).val = INT_MAX;
	(*c).key = 0;
	(*c).pathno_l = -1;
	(*c).pathno_r = -1;
	(*c).c_ind_l = -1;
	(*c).c_ind_r = -1;
	(*c).parent = -1;
	(*c).true_arr = -1;

}

void rotate(int num)							//For splaying
{
	while(cache[num].parent != -1)
	{
		if(cache[num].parent == root_index)
		{
			if(cache[cache[num].parent].c_ind_l == num)
				right_rotate(cache[num].parent);
			else
				left_rotate(cache[num].parent);
		}
		else if((cache[cache[num].parent].c_ind_l == num) && (cache[num].parent == cache[cache[cache[num].parent].parent].c_ind_l))
		{
	//		right_rotate(cache[cache[num].parent].parent);
			right_rotate(cache[num].parent);
			right_rotate(cache[num].parent);
		}
		else if((cache[cache[num].parent].c_ind_r == num) && (cache[num].parent == cache[cache[cache[num].parent].parent].c_ind_r))
		{
//			left_rotate(cache[cache[num].parent].parent);
			left_rotate(cache[num].parent);
			left_rotate(cache[num].parent);
		}
		else if((cache[cache[num].parent].c_ind_l == num) && (cache[num].parent == cache[cache[cache[num].parent].parent].c_ind_r))
		{
			right_rotate(cache[num].parent);
			left_rotate(cache[num].parent);
		}
		else
		{
			left_rotate(cache[num].parent);
			right_rotate(cache[num].parent);
		}
	}
	root = cache[num];
	root_index = num;
}

void right_rotate(int n1)
{
	int n2 = cache[n1].c_ind_l;
	cache[n1].c_ind_l = cache[n2].c_ind_r;

	if(cache[n2].c_ind_r != -1)
		cache[cache[n2].c_ind_r].parent = n1;
	cache[n1].pathno_l = cache[n2].pathno_r;
	cache[n1].pl = cache[n2].pr;
	cache[n2].parent = cache[n1].parent;
	cache[n2].c_ind_r = -1;
	cache[n2].pathno_r = -1;

	cache[n2].c_ind_r = n1;

	if(cache[n2].parent != -1)
	{
		if(cache[cache[n1].parent].c_ind_l == n1)
			cache[cache[n1].parent].c_ind_l = n2;
		else
			cache[cache[n1].parent].c_ind_r = n2;
	}
	cache[n1].parent = n2;
}


void left_rotate(int n1)
{
	int n2 = cache[n1].c_ind_r;
	cache[n1].c_ind_r = cache[n2].c_ind_l;

	if(cache[n2].c_ind_l != -1)
		cache[cache[n2].c_ind_l].parent = n1;

	cache[n1].pathno_r = cache[n2].pathno_l;
	cache[n2].c_ind_l = -1;
	cache[n2].pathno_l = -1;
	cache[n1].pr = cache[n2].pl;

	cache[n2].parent = cache[n1].parent;

	cache[n2].c_ind_l = n1;

	if(cache[n2].parent != -1)
	{
		if(cache[cache[n1].parent].c_ind_l == n1)
			cache[cache[n1].parent].c_ind_l = n2;
		else
			cache[cache[n1].parent].c_ind_r = n2;
	}
	cache[n1].parent = n2;
}


int find_node(int c_index, int value)
{
	if( cache[c_index].val < value && cache[c_index].c_ind_r != -1 )
		return find_node(cache[c_index].c_ind_r, value );
	else if( cache[c_index].val < value && cache[c_index].pathno_r != -1 )
		return c_index;
	else if( cache[c_index].val > value && cache[c_index].c_ind_l != -1 )
		return find_node( cache[c_index].c_ind_l, value );
	else if( cache[c_index].val > value && cache[c_index].pathno_l != -1 )
		return c_index;
}

void split_main(node_veb* arr_veb, int value)					//For splitting
{
	int key = binarysearch_new(arr_veb, NULL, 1, -1, value);
	if(key == -1)
	{
		printf("The value doesn't exist\n");
		return;
	}

	if(splits == 0)
	{
		initialize_cache(&cache[0]);
		cache[0].val = value;
		cache[0].pathno_l = path_end;
		cache[0].pathno_r = path_end;
		cache[0].true_arr = path_end;
		cache[0].key = key;
		cache[0].pl = 0;
		cache[0].pr = 1;
		root = cache[0];
		root_index = 0;

//		make_path_array(arr_veb,key,  value);
		path_end = (path_end+1)%path_size;
		cache_end++;
		splits++;
	}
	else
	{
		if(!cache_full)
		{
			initialize_cache(&cache[cache_end]);
			int node_no = find_node(root_index,value);
			if(cache[node_no].val < value)
			{
				cache[node_no].c_ind_r = cache_end;
				cache[node_no].pathno_r = -1;
			}
			else{
				cache[node_no].c_ind_l = cache_end;
				cache[node_no].pathno_l = -1;
			}

			cache[cache_end].val = value;
			cache[cache_end].pathno_l = path_end;
			cache[cache_end].pathno_r = path_end;
			cache[cache_end].true_arr = path_end;
			cache[cache_end].key = key;
			cache[cache_end].parent = node_no;
			cache[cache_end].pl = 0;
			cache[cache_end].pr = 1;
			rotate(cache_end);

			path_end=(path_end+1)%path_size;
			cache_end=(cache_end+1)%cache_size;

			if((cache_end == cache_start-1) || (cache_start == 0 && cache_end == cache_size -1))
				cache_full = true;
			splits++;
		}
		else
		{
			merge(arr_veb);
			initialize_cache(&cache[cache_end]);
			int node_no = find_node(root_index,value);
			if(cache[node_no].val < value)
			{
//				make_path_array(arr_veb,key,  value);

				cache[node_no].c_ind_r = cache_end;
				cache[node_no].pathno_r = -1;
			}
			else{
//				make_path_array(arr_veb,key,  value);
				cache[node_no].c_ind_l = cache_end;
				cache[node_no].pathno_l = -1;
			}

			cache[cache_end].val = value;
			cache[cache_end].pathno_l = path_end;
			cache[cache_end].pathno_r = path_end;
			cache[cache_end].true_arr = path_end;
			cache[cache_end].key = key;
			cache[cache_end].parent = node_no;
			cache[cache_end].pl = 0;
			cache[cache_end].pr = 1;
			rotate(cache_end);
	
			path_end=(path_end+1)%path_size;
			splits++;
			cache_end=(cache_end+1)%cache_size;
		}
	}
}
void merge(node_veb* arr_veb)									//For merging
{
	int par = cache[cache_start].parent;

	if(par != -1)
	{
		if(cache[par].c_ind_l == cache_start)
		{
			cache[par].c_ind_l = -1;
			cache[par].pathno_l = cache[par].true_arr;
			cache[par].pl = 0;
		}
		else
		{
			cache[par].c_ind_r = -1;
			cache[par].pathno_r = cache[par].true_arr;
			cache[par].pr = 1;		
		}
		splits--;
	}
	cache_start=(cache_start+1)%cache_size;
}


int search(int r_num, node_veb* arr_veb, int value)					//For searching
{

	if(r_num != -1)
	{
		if(cache[r_num].val == value){
			return 1;
		}
		else if(cache[r_num].val < value)
		{

			if(cache[r_num].c_ind_r != -1)
				return search(cache[r_num].c_ind_r, arr_veb, value);
			else if(cache[r_num].pathno_r != -1)
			{
				int ans;
				if(cache[r_num].pr == 0){
					ans = binarysearch_new(arr_veb, pathl[cache[r_num].pathno_r].parr,-1,0,value);
				}
				else{
					ans = binarysearch_new(arr_veb, pathr[cache[r_num].pathno_r].parr,-1,0,value);
				}
				if(ans != -1)
				{
					split_main(arr_veb, value);
					return 1;
				}
			}
		}
		else
		{
			if(cache[r_num].c_ind_l != -1)
				return search(cache[r_num].c_ind_l, arr_veb, value);
			else if(cache[r_num].pathno_l != -1)
			{
				int ans;
				if(cache[r_num].pl == 0)
					ans = binarysearch_new(arr_veb, pathl[cache[r_num].pathno_l].parr,-1,0,value);
				else
					ans = binarysearch_new(arr_veb, pathr[cache[r_num].pathno_l].parr,-1,0,value);

				if(ans != -1){
					split_main(arr_veb, value);
					return 1;
				}
			} 
		}
	}
	else
	{
		int key = binarysearch_new(arr_veb, NULL, 1, -1, value);
		if(key == -1)
			return -1;
		else{
			split_main(arr_veb, value);
			return 1;
		}
	}
	return -1;
}

void traverse(node_veb* arr_veb, node_path* path, int loc_v, int loc_p)				//Traversing bulk node and underlyig veb
{
	if(loc_v>-1)
	{
		if(arr_veb[loc_v].leaf == true)
		{	
			printf("veb: %d\n", arr_veb[loc_v].val);
		}
		else if(arr_veb[loc_v].leaf == false)
		{	
			traverse(arr_veb,NULL,arr_veb[loc_v].l,-1);
				printf("veb: %d\n", arr_veb[loc_v].val);
			traverse(arr_veb,NULL,arr_veb[loc_v].r,-1);
		}
	}
	else if(loc_p>-1)
	{
		if((path[loc_p].p_l != -1))
			traverse(arr_veb,path,-1,path[loc_p].p_l);
		else if(path[loc_p].v_l != -1)
			traverse(arr_veb,NULL,path[loc_p].v_l,-1);
		printf("path: %d\n",path[loc_p].val);
		if((path[loc_p].p_r != -1))
			traverse(arr_veb,path,-1,path[loc_p].p_r);
		else if(path[loc_p].v_r != -1 )
			traverse(arr_veb,NULL,path[loc_p].v_r,-1);
	}
}


void traverse_c(node_veb* arr_veb, int r_num)							//Traversing root block
{
	if(r_num != -1)
	{
		if(cache[r_num].c_ind_l != -1)
			traverse_c(arr_veb,cache[r_num].c_ind_l);
		else if(cache[r_num].pathno_l != -1)
		{
			if(cache[r_num].pl)
				traverse(arr_veb, pathr[cache[r_num].pathno_l].parr, -1, 0);
			else
				traverse(arr_veb, pathl[cache[r_num].pathno_l].parr, -1, 0);
		}

		printf("cache: %d\n", cache[r_num].val);
	
		if(cache[r_num].c_ind_r != -1)
			traverse_c(arr_veb,cache[r_num].c_ind_r);
		else if(cache[r_num].pathno_r != -1)
		{
			if(cache[r_num].pr)
				traverse(arr_veb, pathr[cache[r_num].pathno_r].parr, -1, 0);
			else
				traverse(arr_veb, pathl[cache[r_num].pathno_r].parr, -1, 0);
		}

	}
}

int main(int argc, char* argv[])
{
//	ht=20;
	cnt=0;
//	leaves=524288;
	int query;

	ht = 21;
	leaves = pow(2,ht-1);

	if(argc <2)
	{
		printf("Enter filename!!!\n");
		return 0;
	}

	ifstream myfile;
	myfile.open("database.txt");				//Input file for keys

	REP(i,leaves)
		myfile>>leaf_tree[i];
	myfile.close();
	sort(leaf_tree,leaf_tree+leaves);

	veb_initialize(layer1, 1,2*leaves,ht,1,ht);

	init_veb(layer1,1);

	query = pow(2,21);
	ifstream myfile1;
	myfile1.open(argv[1]);						//Input file for search queries
	srand(time(NULL));
	int randno;
	clock_t startTime = clock();

	REP(i,query)
	{
		lc_p=0;
		myfile1>>randno;
		search(root_index, layer1,randno);
	}
	float secsElapsed = (float)(clock() - startTime)/CLOCKS_PER_SEC;

	printf("%f\n", secsElapsed);
	myfile.close();
}	
