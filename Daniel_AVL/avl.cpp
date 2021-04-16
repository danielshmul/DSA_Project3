#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

class Node{
public:
    string name;
    long id;
    int balance;
    Node* left;
    Node* right;
    Node() : id(0), name(""), balance(0), left(nullptr), right(nullptr) {}
    Node(string name, long id) : 
    name(name), id(id), balance(0), left(nullptr), right(nullptr) {}
    Node(string name, long id, int balance, Node* left, Node* right) : 
    name(name), id(id), balance(balance), left(left), right(right) {}
};

class AvlTree{
    Node* root;
    
public:
    AvlTree() : root(nullptr) {}
    Node* getRoot() {return root;}
    bool isAVL(Node* root);

    bool insert(string, string);
    Node* insert(Node*, string, long);
    bool isValid(string, string);
    void updateBalance(Node*);
    int calculateBalance(Node*);
    int getHeight(Node*);
    Node* balanceTree(Node*);
    Node* rotation(Node*);
    Node* rotateLeft(Node*);
    Node* rotateRight(Node*);
    Node* rotateLeftRight(Node*);
    Node* rotateRightLeft(Node*);

    bool remove(long);
    Node* remove(Node*, long);
    void inorderSuccessors(vector<Node*>&, Node*);

    vector<long> search(string);
    void search(Node*, vector<long>&, string);
    string search(long);
    Node* search(Node*, long);

    string traversePreorder(Node* node);
    string traverseInorder(Node* node);
    string traversePostorder(Node* node);

    int countLevels();

    bool removeInorder(int);
    void inorderVector(vector<Node*>&, Node*);
};

// Insert node containing name and id (and maybe more) into tree
// Return true if insertion successful (False if constraints broken)
/* Insertion: 
 * Insert same as BST
 * Identify deepest node that breaks the balance factor and rotate (traversal)
 */
bool AvlTree::insert(string name, string id_str)
{
    // check constraints (ID 8 digit long, Name only alpha and spaces, unique ID)
    if (!isValid(name, id_str)) return false;
    long id = stol(id_str);
    if (search(root, id)) return false;                

    root = insert(root, name, id);
    updateBalance(root); 
    root = balanceTree(root);
    
    return true;
}

// Insert overload for recursive functionality
// Self cite: stepik
Node* AvlTree::insert(Node* root, string name, long id)
{
    if (!root) return new Node(name, id);
    if (id < root->id)
        root->left = insert(root->left, name, id);
    else 
        root->right = insert(root->right, name, id);
    return root;
}

// checks if Name and ID meet constraints
bool AvlTree::isValid(string name, string id)
{
    // "Name must include only alphabet from [a-z, A-Z, and spaces]"
    for(auto it = name.begin(); it != name.end(); it++)
         if(!isalpha(*it) && !isspace(*it)) return false;

    // "UFIDs are strictly 8 digtis long"
    if (id.size() != 8) return false;

    return true;    
}

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


// Remove a student by ID
bool AvlTree::remove(long id)                      
{
    if (search(id) == "Not found!") return false;
    root = remove(root, id);
    return true;
}

// Remove overload for recursive functionality   
Node* AvlTree::remove(Node* root, long id)
{
    if (!root) return nullptr;
    if (id < root->id)
    root->left = remove(root->left, id);
    else if (id > root->id)
        root->right = remove(root->right, id);
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


// Search for student by name
// Return vector of IDs, size = 0 if not found
vector<long> AvlTree::search(string name)
{
    vector<long> ids;
    search(root, ids, name);
    return ids;
}

// Search NAME overload for preorder recursive functionality 
void AvlTree::search(Node* root, vector<long>& ids, string name)
{
    if (!root) return;
    if (root->name == name) 
        ids.push_back(root->id);
    search(root->left, ids, name);
    search(root->right, ids, name);
}

// Search for student by id
// Return name if found, or "unsuccessful"
string AvlTree::search(long id)
{
    Node* student = search(root, id);
    if(!student) return "unsuccessful";
    return student->name;
}

// Search ID overload for recursive functionality
Node* AvlTree::search(Node* node, long id)
{
    if (!node) return nullptr;
    if (id < node->id)
        node = search(node->left, id);
    else if (id > node->id)
        node = search(node->right, id);
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


// Return number of levels in current AVL tree
int AvlTree::countLevels()
{
    return getHeight(root);
}


// Remove the Nth student from the inorder traversal      
bool AvlTree::removeInorder(int n)
{
    vector<Node*> nodes;
    inorderVector(nodes, root);
    if (n >= nodes.size()) return false;
    root = remove(root, nodes[n]->id);
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


bool AvlTree::isAVL(Node* root)
{
    if(!root)
        return true;
    
    int balance = getHeight(root->left) - getHeight(root->right);
    if(balance < -1 || balance > 1)
        return false;
    if(!isAVL(root->left))
        return false;
    if(!isAVL(root->right))
        return false;
    return true;
}

// Comment out main to use Catch2 testing
// main handles input parsing, printing, etc.
int main()
{
    AvlTree avl;

    int numCommands;
    cin >> numCommands;

    string command;
    string name;
    long id;
    for (int i=0; i<numCommands; i++)
    {
        cin >> command;
        if (command == "check")
        {
            if(avl.isAVL(avl.getRoot()))
                cout << "Valid" << endl;
            else
                cout << "Invalid" << endl;
        }
        if (command == "insert")
        {
            cin >> name;
            name = name.substr(1, name.size()-2);
            string id_str;
            cin >> id_str;

            if (avl.insert(name, id_str))
                cout << "successful" << endl;
            else
                cout << "unsuccessful" << endl;          
        }
        else if (command == "remove")
        {
            cin >> id;

            if(avl.remove(id))
                cout << "successful" << endl;
            else
                cout << "unsucessful" << endl;
        }
        else if (command == "search")
        {
            string temp;
            cin >> temp;
            bool passed_name =  false; // condition for name or id

            if (temp.substr(0,1) == "\"")
            {
                name = temp.substr(1, temp.size()-2);
                passed_name = true;
            }
            else
                id = stol(temp);

            if(passed_name) // passed name
            {
                vector<long> ids = avl.search(name);
                if(ids.size() == 0)
                    cout << "unsuccessful" << endl;
                else
                    for(auto it = ids.begin(); it != ids.end(); it++)
                        cout << setfill('0') << setw(8) << *it << endl;         // Citation: PADDING 0's STACK OVERFLOW: https://stackoverflow.com/questions/1714515/how-can-i-pad-an-int-with-leading-zeros-when-using-cout-operator
            }
            else // passed id
                cout << avl.search(id) << endl;
        }
        else if (command == "printInorder")
        {
            string inorder = avl.traverseInorder(avl.getRoot());
            cout << inorder.substr(0, inorder.size()-2) << endl;
        }
        else if (command == "printPreorder")
        {
            string preorder = avl.traversePreorder(avl.getRoot());
            cout << preorder.substr(0, preorder.size()-2) << endl;
        }
        else if (command == "printPostorder")
        {
            string postorder = avl.traversePostorder(avl.getRoot());
            cout << postorder.substr(0, postorder.size()-2) << endl;
        }
        else if (command == "printLevelCount")
        {
            cout << avl.countLevels() << endl;
        }
        else if (command == "removeInorder")
        {
            int n;
            cin >> n;
            if(avl.removeInorder(n))
                cout << "successful" << endl;
            else
                cout << "unsuccessful" << endl;
        }
        else 
        {
            getline(cin, name);
            cout << "Invalid Command!" << endl;
        }
    }
}