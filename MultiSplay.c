//CODE BY-
// ANANY DHAMIJA 2021MCB1231
// ASHMIT GUPTA 2021MCB1232
// TANMYA POTDAR 2021MCB1252
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

/* A node is created*/
struct node
{
    int key; // Value
    int depth; // 
    int mindepth;
    bool isRoot; // boolean variable that indicates if the edge to its parent is dashed
    struct node* leftChild; // points to left child of the node
    struct node* rightChild; // points to right child of the node
    struct node* parent; // points to parent of the node
};

// Function to find the minimum of 2 numbers
int min(int a,int b)
{
    if(a<b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

// Creating a root node as a global variable
struct node* root;

// Function which displays all the information about the nodes in the tree
void Display(struct node* root)
{
    // Prints value of key at that node
    printf("Value: %d \nisRoot: ",root->key);

    // Tells whether the node is a root or not
    if(root->isRoot)
    {
        printf("True \n"); 
    }
    else
    {
        printf("False \n");
    }

    // Displays the key of the parent node
    if(root->parent==NULL)
    {
        printf("Parent: NULL \n\n");
    }
    else
    {
        printf("Parent: %d \n\n",root->parent->key);
    }

    // We Recursively call display function for the left and right child of the tree
    if(root->leftChild!=NULL)
    {
        Display(root->leftChild);
    }
    if(root->rightChild!=NULL)
    {
        Display(root->rightChild);
    }   
}

//CreateNode function which creates a new node 
struct node* CreateNode(int key, int depth, int minDepth, bool isRoot)
{	
    //Allocates memory to the node
    struct node* ptr = (struct node*)malloc(sizeof(struct node));
    
    // Assigns value to various parameters of the node
    ptr->key=key;
    ptr->depth=depth;
    ptr->mindepth=minDepth;
    ptr->isRoot=isRoot;
    return ptr;
}


//BuildTree Function which creates the tree via recursion
struct node* BuildTree(int start, int end, int depth, bool isRoot)
{

		// Terminating condition for this recursive function
    if(start==end)
    {
        return NULL;
    }
    
    // We Create a mid node, and then we recursively call this Function to create its left subtree and its right subtree
    int mid = (start+end)/2;
    struct node* temp = CreateNode(mid,depth,depth,isRoot);
    temp->leftChild = BuildTree(start,mid,depth+1,false);
    temp->rightChild = BuildTree(mid+1,end,depth+1,true);
    if(temp->leftChild!=NULL)
    {
        temp->leftChild->parent=temp;
    }
    if(temp->rightChild!=NULL)
    {
        temp->rightChild->parent=temp;
    }
    return temp;
}

// Rotate function which first finds out what kind of rotation to make on a certain node (zig-zig, zig-zag, zag-zag, zag-zig) and then performs that rotation
void Rotate(struct node * ptr)
{
    struct node* temp = ptr->parent;
    if(temp->isRoot)
    {
        temp->isRoot=false;
        ptr->isRoot=true;
    }
    if(root==temp)
    {
        root=ptr;
    }
    if(temp->parent!=NULL)
    {
        if(temp->parent->leftChild==temp)
        {
            temp->parent->leftChild=ptr;
        }
        else
        {
            temp->parent->rightChild=ptr;
        }
    }
    ptr->parent=temp->parent;
    if(temp->rightChild==ptr)
    {
        temp->rightChild=ptr->leftChild;
        if(ptr->leftChild!=NULL)
        {
            ptr->leftChild->parent=temp;
        }
        ptr->leftChild=temp;
        temp->parent=ptr;
    }
    else
    {
        temp->leftChild=ptr->rightChild;
        if(ptr->rightChild!=NULL)
        {
            ptr->rightChild->parent=temp;
        }
        ptr->rightChild=temp;
        temp->parent=ptr;
    }
    if(temp->leftChild!=NULL)
    {
        temp->mindepth = min(temp->mindepth, temp->leftChild->mindepth);
    }
    if(temp->rightChild!=NULL)
    {
        temp->mindepth = min(temp->mindepth,temp->rightChild->mindepth);
    }
}

void splay(struct node* ptr,struct node* top)
{
    struct node * temp;
    struct node* temp2;
    // this function splays the required element and makes it reach the root or the top element as provided in question
    while(!(ptr->isRoot || ptr->parent==top))
    {
    
    		// this checks and uses the rotate function to shift some pointers and help the required element reach the top
        temp=ptr->parent;
        if(!(temp->isRoot || temp->parent==top))
        {
            temp2 = temp->parent;
            if((temp2->leftChild==temp && temp->leftChild==ptr) || (temp2->rightChild==temp && temp->rightChild==ptr))
            {
                Rotate(temp);
            }
            else
            {
                Rotate(ptr);
            }
        }
        Rotate(ptr);
    }
}
struct node* refParent(struct node* ptr,bool flag)
{
    struct node* temp = NULL;
    if(!flag)
    {
        temp = ptr->leftChild;
    }
    else
    {
        temp = ptr->rightChild;
    }
    while(1)
    {
        if(!flag)
        {
            if(temp->rightChild!=NULL && temp->rightChild->mindepth < ptr->depth) 
            {
                temp = temp->rightChild;
            }
            else if(temp->leftChild!=NULL && temp->leftChild->mindepth < ptr->depth)
            {      
                temp = temp->leftChild;
            }
            else
            {
                break;
            }
        }
        else
        {
            if(temp->leftChild && temp->leftChild->mindepth < ptr->depth)
            {
                temp = temp->leftChild;
            }
            else if(temp->rightChild && temp->rightChild->mindepth < ptr->depth)
            {
                temp = temp->rightChild;
            }
            else
            {
                break;
            }
        }
    }
    return temp;
}

void SwitchPath(struct node * ptr)
{

		// This Function just changes the preferred child to unpreferred and vice versa, it helps in bringing the Splay-Subtree up towards the root
    struct node* temp;
    if(ptr->leftChild!=NULL)
    {
        if(ptr->leftChild->mindepth > ptr->depth)
        {
            ptr->leftChild->isRoot = !(ptr->leftChild->isRoot);
        }
        else
        {
            temp = refParent(ptr,false);
            splay(temp,ptr);
            if(temp->rightChild!=NULL)
            {
                temp->rightChild->isRoot = !(temp->rightChild->isRoot);
            }
        }
    }
    if(ptr->rightChild!=NULL)
    {
        if(ptr->rightChild->mindepth > ptr->depth)
        {
            ptr->rightChild->isRoot = !(ptr->rightChild->isRoot);
        }
        else
        {
            temp = refParent(ptr,1);
            splay(temp,ptr);
            if(temp->leftChild!=NULL)
            {
                temp->leftChild->isRoot = !(temp->leftChild->isRoot);
            }
        }
    }
}


void MultiSplayAlgo(struct node* ptr)
{
		//this is the main function that uses the above coded splay function and the switch path function
    
    // this function basically splays the node to top of the MST as well as splays the whole SubTree to the top of the MST
    
    struct node* temp = ptr;
    while(temp->parent!=NULL)
    {
        struct node* temp2 = temp->parent;
        if(temp->isRoot)
        {
            splay(temp2,NULL);
            SwitchPath(temp2);
        }
        temp = temp2;
    }
    splay(ptr,NULL);
}

//Fucntion which searches a key in the tree
void Search(int key)
{
		// Create 2 nodes equal to root node
    struct node* ptr = root;
    struct node* prev = root;
    
    // Running loop till ptr becomes NULL or the key is found
    while(ptr!=NULL && ptr->key!=key)
    {
    		
        prev = ptr;
        
        // Searching for key as per Binary search tree
        if(key<ptr->key)
        {
            ptr = ptr->leftChild;
        }
        else
        {
            ptr = ptr->rightChild;
        }
    }
    if(ptr==NULL)
    {
    		// If key is not found in the tree we apply MultiSplayAlgo to the nearest node of the tree where key would have been present 
        MultiSplayAlgo(prev);
    }
    else
    {
    		// If the key is found we apply MultiSplayAlgo to that node (ptr) where key is found
        MultiSplayAlgo(ptr);
    }
}



int main()
{
    printf("Enter the number of nodes you want in the tree: ");
    int n;
    scanf("%d",&n);
    printf("\nMultiSplayTree is Created !!!!\n\n");
    root = BuildTree(1,n+1,0,true);
    int s=0;
    while(s!=3)
    {
        printf("1 -> Display the Tree\n2 -> Search an Element\n3 -> Exit the Code\n\nEnter Your Choice: ");
        scanf("%d",&s);
        if(s==1)
        {
            Display(root);
        }
        else if(s==2)
        {
            int d;
            printf("Enter element to Search: \n");
            scanf("%d",&d);
            Search(d);
        }
        else if(s==3)
        {
            break;
        }
        else
        {
            printf("Enter a valid Number Choice\n");
        }
    }

}