#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <algorithm>

using namespace std;

class node;

class key
{
    public:
        int data;
        node* leftChild; // Seperator for Index Nodes 

        key()
        {
            leftChild = nullptr;
        }
};

class node
{
    public:
        vector<key *> values;
        bool isDataNode;
        node* rightMostChild; // For Index Nodes 
        node* parent; 

        node()
        {
            rightMostChild = nullptr;
            parent = nullptr;
        }
};

// Used for sorting a node 
bool ascOrder(key *a, key*b){
   return a->data < b->data ;
}

class bPlusTree
{
    public:
        node* root;
        int dataNodeOrder;
        int indexNodeOrder;

        bPlusTree(int d, int t)
        {
            root = nullptr;
            this->dataNodeOrder = d;
            this->indexNodeOrder = t;
        }

        void insert(int val)
        {
            // First Value in B+ Tree 
            if(root == nullptr)
            {
                node* x = new node();
                x->isDataNode = true;
                key* y = new key();
                y->data = val;
                x->values.push_back(y);
                root = x;
                return;
            }

            // No Index Node yet 
            if(root->isDataNode == true)
            {
                // If root doesn't reach max capacity
                if(root->values.size() < 2*dataNodeOrder)
                {
                    key* n = new key();
                    n->data = val;
                    root->values.push_back(n);
                    sort(root->values.begin(),root->values.end(),ascOrder);
                }
                else
                {
                    key *n = new key();
                    n->data = val;
                    root->values.push_back(n);
                    sort(root->values.begin(), root->values.end(), ascOrder);

                    // Creating a right node
                    node* new_node = new node();
                    new_node->isDataNode = true;

                    // Sending d+1 keys to right node and deleting them from earlier node
                    for(int i = dataNodeOrder; i < root->values.size(); i++)
                    {
                        key* n = new key();
                        n->data = root->values[i]->data;
                        new_node->values.push_back(n);
                    } 

                    for(int i = 0; i <= dataNodeOrder ; i++){
                        root->values.pop_back();
                    }   
                    
                    // Making a new root i.e. first index node and storing seperator (first of right child) in the new index node 
                    node* newRoot = new node();
                    newRoot->isDataNode = false;

                    key* newSeperator = new key();
                    newSeperator->data = new_node->values[0]->data;
                    // Left child which was previous root; now becomes left child of seperator key
                    newSeperator->leftChild = root;

                    newRoot->values.push_back(newSeperator);
                    newRoot->rightMostChild = new_node;

                    root = newRoot;
                    root->values[0]->leftChild->parent = root;
                    root->rightMostChild->parent = root;
                }
                return;
            }
        
        // If there are 1 or more index nodes; we need to find our required data key

        node* currPtr = root;
        while(!(currPtr->isDataNode))
        {
            bool check = false; // If seperator not found traverse through the rightmost path
            for(int i=0;i< currPtr->values.size();i++)
            {
                if(val < currPtr->values[i]->data) 
                {
                    check = true;
                    currPtr = currPtr->values[i]->leftChild;
                    break;
                }
            }

            if(!check)
            {
                currPtr = currPtr->rightMostChild;
            }
        }
        
        // Check if Data node searched is not full
        if(currPtr->values.size() < 2*dataNodeOrder)
        {
            key* n = new key();
            n->data = val;
            currPtr->values.push_back(n);
            sort(currPtr->values.begin(),currPtr->values.end(),ascOrder);
            return;
        }

        key *n = new key();
        n->data = val;
        currPtr->values.push_back(n);
        sort(currPtr->values.begin(), currPtr->values.end(), ascOrder);

        // Split the overflowed data node
        node *new_node = new node();
        new_node->isDataNode = true;

        // Sending d+1 keys to right node and deleting them from earlier node
        for (int i = dataNodeOrder; i < currPtr->values.size(); i++)
        {
            key *n = new key();
            n->data = currPtr->values[i]->data;
            new_node->values.push_back(n);
        }

        for (int i = 0; i <= dataNodeOrder; i++)
        {
            currPtr->values.pop_back();
        }

        // Sending seperator key to parent index node
        key *newSeperator = new key();
        newSeperator->data = new_node->values[0]->data;
        newSeperator->leftChild = currPtr;
        new_node->parent = currPtr->parent;
        
        // If new Seperator is the largest value in parent index node then task is easy otherwise pointers need to be modified
        if( newSeperator->data > currPtr->parent->values[currPtr->parent->values.size()-1]->data)
        {
            currPtr->parent->values.push_back(newSeperator);
            currPtr->parent->rightMostChild = new_node;
        }
        else
        {
            currPtr->parent->values.push_back(newSeperator);
            node* currParent = currPtr->parent;
            sort(currParent->values.begin(),currParent->values.end(),ascOrder);
            // Finding the key in parent which is equal to seperator key after sorting
            int j=0;
            for( j=0; j< currParent->values.size();j++ )
            {
                if(currParent->values[j]->data == newSeperator->data)
                {
                    break;
                }
            }
            currParent->values[j+1]->leftChild = new_node;
        }

        // After we have inserted in the parent node(index) we must check for overflows till root

        currPtr = currPtr->parent;
        
        if(currPtr == nullptr)
        {
            return;
        }
        
        while( currPtr->values.size() > 2*indexNodeOrder + 1)
        {
            // If Parent was root; we cover that case seperately 

            if((currPtr == root) or (currPtr == nullptr))
            {
                break;
            }

            // Splitting the overflowed index node till we reach the root

            node* rightNode = new node();
            rightNode->isDataNode = false;    

            // Node divided in t and 1 and t+1
            int sendUp = currPtr->values[indexNodeOrder]->data;
            for(int i = indexNodeOrder + 1; i<currPtr->values.size();i++)
            {
                key* n = new key();
                n->data = currPtr->values[i]->data;
                n->leftChild = currPtr->values[i]->leftChild;
                rightNode->values.push_back(n);
            }

            rightNode->parent = currPtr->parent;
            rightNode->rightMostChild = currPtr->rightMostChild;
            currPtr->rightMostChild = currPtr->values[indexNodeOrder]->leftChild;

            for(int i = 0;i <= indexNodeOrder+1 ;i++)
            {
                currPtr->values.pop_back();
            }

            for(auto x: rightNode->values)
            {
                x->leftChild->parent = rightNode;
            }
            rightNode->rightMostChild->parent = rightNode;

            // Sending middle value upwards to parent similar to one we had done earlier
            key* n = new key();
            n->data = sendUp;
            n->leftChild = currPtr;

            if(n->data > currPtr->parent->values[currPtr->parent->values.size()-1]->data)
            {
                currPtr->parent->values.push_back(n);
                currPtr->parent->rightMostChild = rightNode;
            }
            else
            {
                currPtr->parent->values.push_back(n);
                node* currParent = currPtr->parent;
                sort(currParent->values.begin(),currParent->values.end(),ascOrder);
                int j = 0;
                for(j=0;j<currParent->values.size();j++)
                {
                    if(currParent->values[j]->data == n->data)
                    {
                        break;
                    }
                }
                currParent->values[j+1]->leftChild = rightNode;
            }
            currPtr= currPtr->parent;
        }

        // Now we handle the case if root is overflowed

        if((currPtr == root) and (currPtr->values.size() > 2*indexNodeOrder +1))
        {
            node* rightNode = new node();
            rightNode->isDataNode = false;
            int sendUp = currPtr->values[indexNodeOrder]->data;
            
            for(int i= indexNodeOrder +1; i< root->values.size();i++)
            {
                key* n = new key();
                n->data = root->values[i]->data;
                n->leftChild = root->values[i]->leftChild;
                rightNode->values.push_back(n);
            }

            rightNode->rightMostChild = currPtr->rightMostChild;
            currPtr->rightMostChild = currPtr->values[indexNodeOrder]->leftChild;

            for(int i=0;i<= indexNodeOrder +1;i++)
            {
                currPtr->values.pop_back();
            }

            for (auto x : rightNode->values)
            {
                x->leftChild->parent = rightNode;
            }
            rightNode->rightMostChild->parent= rightNode;

            //Making new root
            node* newRoot = new node();
            newRoot->isDataNode = false;

            n = new key();
            n->data = sendUp;
            n->leftChild = root;

            newRoot->rightMostChild = rightNode;
            newRoot->values.push_back(n);

            root = newRoot;
            root->values[0]->leftChild->parent = root;
            root->rightMostChild->parent = root;
        }
    }

    int indexNodes(node* z)
    {
        if (z==nullptr) 
            return 0;
        
        if(z->isDataNode)
            return 0;
        
        int ans = 1;
        for (auto x : z->values)
        {
            ans += indexNodes(x->leftChild);
        }
        ans += indexNodes(z->rightMostChild);

        return ans;
    }

    int  dataNodes(node* z)
    {
        if(z == nullptr)
            return 0;

        if(z->isDataNode)
            return 1;
        
        int ans = 0;
        for (auto x : z->values)
        {
            ans += dataNodes(x->leftChild);
        }
        ans += dataNodes(z->rightMostChild);

        return ans;
    }

    void display()
    {
        if(root == nullptr)
            return;
        
        cout << indexNodes(root) << " " << dataNodes(root) << " ";
        for (auto x : root->values)
        {
            cout << x->data << " ";
        }
        cout<<endl;
        return;
    }
};



int main(){
    int d, t;
    cin >> d >> t;
    bPlusTree tree(d, t);
    int choice;
    cin >> choice;
    while (choice != 3)
    {
        if (choice == 1)
        {
            int val;
            cin >> val;
            tree.insert(val);
        }
        else if (choice == 2)
        {
            tree.display();
        }
        else
        {
            cout << " Invalid Choice Code " << endl;
        }
        cin >> choice;
    }
    return 0;
}