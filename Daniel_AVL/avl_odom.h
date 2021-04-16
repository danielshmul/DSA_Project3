#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "odom.h"

using namespace std;

class Node{
public:
    // string name;

    Odom* data;
    int time;

    int balance;
    Node* left;
    Node* right;
    Node() : time(0), name(""), balance(0), left(nullptr), right(nullptr) {}
    Node(Odom* data, int time) : 
    data(data), time(time), balance(0), left(nullptr), right(nullptr) {}
    Node(Odom* data, int time, int balance, Node* left, Node* right) : 
    data(data), time(time), balance(balance), left(left), right(right) {}
};

class AvlTree{
    Node* root;
    
public:
    AvlTree() : root(nullptr) {}
    Node* getRoot() {return root;}
    bool isAVL(Node* root);

    bool insert(int, Odom*);
    Node* insert(Node*, string, int);
    bool isValtime(string, string);
    void updateBalance(Node*);
    int calculateBalance(Node*);
    int getHeight(Node*);
    Node* balanceTree(Node*);
    Node* rotation(Node*);
    Node* rotateLeft(Node*);
    Node* rotateRight(Node*);
    Node* rotateLeftRight(Node*);
    Node* rotateRightLeft(Node*);

    bool remove(int);
    Node* remove(Node*, int);
    void inorderSuccessors(vector<Node*>&, Node*);

    vector<int> search(int);
    void search(Node*, vector<int>&, int);
    string search(int);
    Node* search(Node*, int);

    string traversePreorder(Node* node);
    string traverseInorder(Node* node);
    string traversePostorder(Node* node);

    // int countLevels();

    // bool removeInorder(int);
    // void inorderVector(vector<Node*>&, Node*);
};

// Insert node containing name and time (and maybe more) into tree
// Return true if insertion successful (False if constraints broken)
/* Insertion: 
 * Insert same as BST
 * timeentify deepest node that breaks the balance factor and rotate (traversal)
 */
bool AvlTree::insert(int time, Odom* data)
{
    // // check constraints (time 8 digit int, Name only alpha and spaces, unique time)
    // if (!isValtime(name, time_str)) return false;
    // int time = stol(time_str);
    // if (search(root, time)) return false;                

    root = insert(root, data, time);
    updateBalance(root); 
    root = balanceTree(root);
    
    return true;
}

// Insert overload for recursive functionality
// Self cite: stepik
Node* AvlTree::insert(Node* root, Odom* data, int time)
{
    if (!root) return new Node(data, time);
    if (time < root->time)
        root->left = insert(root->left, data, time);
    else 
        root->right = insert(root->right, data, time);
    return root;
}

// // checks if Name and time meet constraints
// bool AvlTree::isValid(string name, string time)
// {
//     // "Name must include only alphabet from [a-z, A-Z, and spaces]"
//     for(auto it = name.begin(); it != name.end(); it++)
//          if(!isalpha(*it) && !isspace(*it)) return false;

//     // "UFtimes are strictly 8 digtis int"
//     if (time.size() != 8) return false;

//     return true;    
// }

// Update balance for search path
void AvlTree::updateBalance(Node* node)
{
    if (!node) return;
    updateBalance(node->left);
    node->balance = calculateBalance(node);
    updateBalance(node->right);
}

// Calculates balance for updateBalance 
int AvlTree::calculateBalance(Node* node)
{
    if (!node) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Return node height to calculate balance
// Self cite stepik
int AvlTree::getHeight(Node* node)
{
    if (!node) return 0;
    return 1 + max(getHeight(node->left), getHeight(node->right));
}

// post order traversal, !balance -> rotations and then update balance for path
Node* AvlTree::balanceTree(Node* node)       
{
    if (!node) return nullptr;
    node->left = balanceTree(node->left);
    node->right = balanceTree(node->right);
    if (abs(node->balance) > 1)
    {
        node = rotation(node);
        updateBalance(root); 
    }
    return node; 
}

// Complete corresponding rotation
Node* AvlTree::rotation(Node* node)
{
    // left left case (right rotation)                     
    if(node->balance == 2 && node->left->balance == 1)
        node = rotateRight(node);
    // right right case (left rotation)                     
    else if(node->balance == -2 && node->right->balance == -1)
        node = rotateLeft(node);
    // left right case (left right rotation)
    else if(node->balance == 2 && node->left->balance == -1)
        node = rotateLeftRight(node);
    // right left case (right left rotation)
    else //(node->balance == -2 && node->right->balance == 1)
        node = rotateRightLeft(node);

    return node;
} 

// Rotate left operation (right right case)
// return newParent
// Self cite: stepik
Node* AvlTree::rotateLeft(Node* node)
{
    Node* grandchild = node->right->left;
    Node* newParent = node->right;
    newParent->left = node;
    node->right = grandchild;
    return newParent;
}

// Rotate right operation (left left case)
// return newParent
// Self cite: stepik
Node* AvlTree::rotateRight(Node* node)
{
    Node* grandchild = node->left->right;
    Node* newParent = node->left;
    newParent->right = node;
    node->left = grandchild;
    return newParent;
}

// Rotate left right operation (left right case)
// return newParent
// Self cite: stepik
Node* AvlTree::rotateLeftRight(Node* node)
{
    node->left = rotateLeft(node->left);
    return rotateRight(node);
}

// Rotate right left operation (right left case)
// return newParent
// Self cite: stepik
Node* AvlTree::rotateRightLeft(Node* node)
{
    node->right = rotateRight(node->right);
    return rotateLeft(node);
}


// Remove a student by time
bool AvlTree::remove(int time)                      
{
    if (search(time) == "Not found!") return false;
    root = remove(root, time);
    return true;
}

// Remove overload for recursive functionality   
Node* AvlTree::remove(Node* root, int time)
{
    if (!root) return nullptr;
    if (time < root->time)
    root->left = remove(root->left, time);
    else if (time > root->time)
        root->right = remove(root->right, time);
    else
    {
        Node* successor;
        if (!root->left)
        {
            successor = root->right;
            delete root;
            return successor;
        }
        if (!root->right)
        {
            successor = root->left;
            delete root;
            return successor;
        }   

        vector<Node*> nodes;
        inorderSuccessors(nodes, root->right);
        if (nodes.size() == 1)
        {
            nodes[0]->left = root->left;
            delete root;
        }
        else
        {
            nodes[0]->left = root->left;
            nodes[0]->right = root->right;
            nodes[1]->left = nodes[0]->right;
            delete root;
        }
        return nodes[0];
    }
    return root;  
}

// Returns all inorder successors for remove operation
void AvlTree::inorderSuccessors(vector<Node*>& nodes, Node* node)
{
    if (!node) return;
    inorderSuccessors(nodes, node->left);
    nodes.push_back(node);
}


// // Search for student by name
// // Return vector of times, size = 0 if not found
// vector<int> AvlTree::search(string name)
// {
//     vector<int> times;
//     search(root, times, name);
//     return times;
// }

// // Search NAME overload for preorder recursive functionality 
// void AvlTree::search(Node* root, vector<int>& times, string name)
// {
//     if (!root) return;
//     if (root->name == name) 
//         times.push_back(root->time);
//     search(root->left, times, name);
//     search(root->right, times, name);
// }

// Search for student by time
// Return name if found, or "unsuccessful"
string AvlTree::search(int time)
{
    Node* student = search(root, time);
    if(!student) return "unsuccessful";
    return student->name;
}

// Search time overload for recursive functionality
Node* AvlTree::search(Node* node, int time)
{
    if (!node) return nullptr;
    if (time < node->time)
        node = search(node->left, time);
    else if (time > node->time)
        node = search(node->right, time);
    return node;
}


// Return comma-separated string of preorder traversal
string AvlTree::traversePreorder(Node* node)
{
    if (!node) return "";
    return node->name + ", " + traversePreorder(node->left) + traversePreorder(node->right);
}

// Return comma-separated string of inorder traversal
// Self cite: stepik
string AvlTree::traverseInorder(Node* node)
{         
    if (!node) return "";
    return traverseInorder(node->left) + node->name + ", " + traverseInorder(node->right);
}

// Return comma-separated string of postorder traversal
string AvlTree::traversePostorder(Node* node)
{
    if (!node) return "";
    return traversePostorder(node->left) + traversePostorder(node->right) + node->name + ", ";
}


// // Return number of levels in current AVL tree
// int AvlTree::countLevels()
// {
//     return getHeight(root);
}


// Remove the Nth student from the inorder traversal      
bool AvlTree::removeInorder(int n)
{
    vector<Node*> nodes;
    inorderVector(nodes, root);
    if (n >= nodes.size()) return false;
    root = remove(root, nodes[n]->time);
    return true;
}

// Returns inorder vector of nodes
void AvlTree::inorderVector(vector<Node*>& nodes, Node* root)
{
    if (!root) return;
    inorderVector(nodes, root->left);
    nodes.push_back(root);
    inorderVector(nodes, root->right);
}


// bool AvlTree::isAVL(Node* root)
// {
//     if(!root)
//         return true;
    
//     int balance = getHeight(root->left) - getHeight(root->right);
//     if(balance < -1 || balance > 1)
//         return false;
//     if(!isAVL(root->left))
//         return false;
//     if(!isAVL(root->right))
//         return false;
//     return true;
// }

// Comment out main to use Catch2 testing
// main handles input parsing, printing, etc.
// int main()
// {
//     AvlTree avl;

//     int numCommands;
//     cin >> numCommands;

//     string command;
//     string name;
//     int time;
//     for (int i=0; i<numCommands; i++)
//     {
//         cin >> command;
//         if (command == "check")
//         {
//             if(avl.isAVL(avl.getRoot()))
//                 cout << "Valtime" << endl;
//             else
//                 cout << "Invaltime" << endl;
//         }
//         if (command == "insert")
//         {
//             cin >> name;
//             name = name.substr(1, name.size()-2);
//             string time_str;
//             cin >> time_str;

//             if (avl.insert(name, time_str))
//                 cout << "successful" << endl;
//             else
//                 cout << "unsuccessful" << endl;          
//         }
//         else if (command == "remove")
//         {
//             cin >> time;

//             if(avl.remove(time))
//                 cout << "successful" << endl;
//             else
//                 cout << "unsucessful" << endl;
//         }
//         else if (command == "search")
//         {
//             string temp;
//             cin >> temp;
//             bool passed_name =  false; // condition for name or time

//             if (temp.substr(0,1) == "\"")
//             {
//                 name = temp.substr(1, temp.size()-2);
//                 passed_name = true;
//             }
//             else
//                 time = stol(temp);

//             if(passed_name) // passed name
//             {
//                 vector<int> times = avl.search(name);
//                 if(times.size() == 0)
//                     cout << "unsuccessful" << endl;
//                 else
//                     for(auto it = times.begin(); it != times.end(); it++)
//                         cout << setfill('0') << setw(8) << *it << endl;         // Citation: PADDING 0's STACK OVERFLOW: https://stackoverflow.com/questions/1714515/how-can-i-pad-an-int-with-leading-zeros-when-using-cout-operator
//             }
//             else // passed time
//                 cout << avl.search(time) << endl;
//         }
//         else if (command == "printInorder")
//         {
//             string inorder = avl.traverseInorder(avl.getRoot());
//             cout << inorder.substr(0, inorder.size()-2) << endl;
//         }
//         else if (command == "printPreorder")
//         {
//             string preorder = avl.traversePreorder(avl.getRoot());
//             cout << preorder.substr(0, preorder.size()-2) << endl;
//         }
//         else if (command == "printPostorder")
//         {
//             string postorder = avl.traversePostorder(avl.getRoot());
//             cout << postorder.substr(0, postorder.size()-2) << endl;
//         }
//         else if (command == "printLevelCount")
//         {
//             cout << avl.countLevels() << endl;
//         }
//         else if (command == "removeInorder")
//         {
//             int n;
//             cin >> n;
//             if(avl.removeInorder(n))
//                 cout << "successful" << endl;
//             else
//                 cout << "unsuccessful" << endl;
//         }
//         else 
//         {
//             getline(cin, name);
//             cout << "Invaltime Command!" << endl;
//         }
//     }
// }