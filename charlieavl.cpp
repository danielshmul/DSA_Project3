/*
 * Charles Richardson Data Structures and Algorithms Project 1 Resubmission Mar 12 2021
 * Failed cases: insert randomly, removeInorder (edge cases), printPreorder,
 *              insert 100+ entries, print all three traversals
 *              insert 100+ entries, remove, search, and printPreorder and levelCount */

#include <iostream>
#include <utility>
#include <vector>

//Encapsulating class, responsible for all node methods
class Node {
    int id = -1, height = 0;
    std::string name = "";
    Node* left = nullptr;
    Node* right = nullptr;

public:
    Node();
    Node(std::string name, int id);

    static int findMax(Node* left, Node* right);
    int balanceFactor();
    Node* checkBalance();
    Node* checkRotation(int _id);

    Node* rotateLeft();
    Node* rotateRight();

    Node* insert(const std::string& n, int _id);
    Node* remove(int _id);
    Node* helpRemove(int _id);

    void search(int _id);
    bool search(const std::string& n);

    void printInorder();
    void printPreorder();
    void printPostorder();
    void printLevelCount() const;

    Node* searchInorder(int n, int N);
    Node* removeInorder(int N);
};


//************* METHOD DEFINITIONS **************//

//Constructors
Node::Node() {
    id = -1;
    height = 0;
    left = nullptr;
    right = nullptr;
}
Node::Node(std::string name, int id) {
    this->id = id;
    this->name = std::move(name);
    height = 1;
    left = nullptr;
    right = nullptr;
}

//Find the maximum height of the node, ensuring that the left and right nodes return nonnull numbers.
//O(1)
int Node::findMax(Node* l, Node* r) {
    int _l = ((l)? l->height : 0) + 1;
    int _r = ((r)? r->height : 0) + 1;
    return (_l >= _r)? _l : _r;
}

//Returns the difference in the left and right height
//O(1)
int Node::balanceFactor() {
    //Left and right are guaranteed to be nonnull
    int l = (left)? left->height : 0;
    int r = (right)? right->height : 0;
    return l - r;
}
//Checks if the tree needs to be rotated at the specified node
//O(1)
Node* Node::checkBalance() {
    //If there is a left node, then there may be a left inbalance
    if(left) {
        //Left-left
        if (balanceFactor() > 1 && left->balanceFactor() >= 0)
            return rotateRight();
        //Left-right
        if (balanceFactor() > 1 && left->balanceFactor() < 0) {
            left = left->rotateLeft();
            return rotateRight();
        }
    }
    //If there is a right node, there may be a right imbalance
    if (right) {
        //Right-right
        if (balanceFactor() < -1 && right->balanceFactor() <= 0)
            return rotateLeft();
        //Right-left
        if (balanceFactor() > -1 && right->balanceFactor() > 0) {
            right = right->rotateRight();
            return rotateLeft();
        }
    }
    return this;
}

//Checks if the tree is unbalanced
//O(1)
Node* Node::checkRotation(int _id) {
    //If there is a left node, then there may be a left inbalance
    if(left) {
        //Left-left
        if (balanceFactor() > 1 && _id < left->id)
            return rotateRight();
        //Left-right
        if (balanceFactor() > 1 && _id > left->id) {
            left = left->rotateLeft();
            return rotateRight();
        }
    }
    //If there is a right node, there may be a right imbalance
    if (right) {
        //Right-right
        if (balanceFactor() < -1 && _id > right->id)
            return rotateLeft();
        //Right-left
        if (balanceFactor() > 1 && _id < right->id) {
            right = right->rotateRight();
            return rotateLeft();
        }
    }
    return this;
}
//Rotation methods
//O(1)
Node* Node::rotateLeft() {
    //Perform rotation
    Node* temp = right;
    right = temp->left;
    temp->left = this;
    //Correct heights
    height = findMax(left, right);
    temp->height = findMax(temp->left, temp->right);
    return temp;
}
//O(1)
Node* Node::rotateRight() {
    //Perform rotation
    Node* temp = left;
    left = temp->right;
    temp->right = this;
    //Correct heights
    height = findMax(left, right);
    temp->height = findMax(temp->left, temp->right);
    return temp;
}

//Helper function: Returns the updated root of the tree
//O(logN)
Node* callInsert(Node* root, const std::string& a) {
    //Get the name from inside the quotes
    unsigned long n = (a.find_last_of('"') - a.find('"') - 1);
    std::string name = a.substr(a.find('"') + 1, n);
    unsigned long l = a.size() - a.find_last_of(' ');
    //Verify the validity of the ID, must be 8 digits long, +1 for null terminator
    if (l != 9) {
        std::cout << "unsuccessful" << std::endl;
        return root;
    }
    int id = stoi(a.substr(a.find_last_of(' ') + 1, l));
    if (!root) { //If the root is null, create the node outside of the insert function
        std::cout << "successful" << std::endl;
        return new Node(name,id);
    }
    //Finally, call the function
    //O(logN)
    return root->insert(name, id);
}

//Places a node into the tree, given it already has a nonnull root
//O(logN)
Node* Node::insert(const std::string& n, int _id) {
    //Check left
    //O(logN)
    if (this->id > _id) {
        if (!left) { //Left node is null
            std::cout << "successful" << std::endl;
            left = new Node(n, _id);
        } else this->left = left->insert(n, _id);
    }
    //Check right
    else if (this->id < _id) {
        if (!right) {
            std::cout << "successful" << std::endl;
            right = new Node(n, _id);
        } else this->right = right->insert(n, _id);
    }
    else {
        //Key has an identical, not inserted
        std::cout << "unsuccessful" << std::endl;
        return nullptr;
    }
    //Check if node is unbalanced
    //O(1)
    height = findMax(left, right);
    //O(1)
    return checkRotation(_id);
}

//Removes a node from the tree, given the tree has a nonnull root
//O(logN)
Node* Node::remove(int _id) {
    //Base case, node not found
    if (this->id != _id && !this->left && !this->right) {
        std::cout << "unsuccessful" << std::endl;
        return this;
    }
    //Base case, node not found
    if (this->id == _id) {
        //if node has no children, delete node, return null
        if (!this->left && !this->right) {
            std::cout << "successful" << std::endl;
            return nullptr;
        }
        //if node has one child, delete node, return child
        else if (!this->left || !this->right) {
            std::cout << "successful" << std::endl;
            return (!this->left)? right : left;
        //if node has two children
        } else {
            //Get next inOrder substitute
            Node *sub = this->right;
            while (sub->left)
                sub = sub->left;
            this->id = sub->id;
            this->name = sub->name;
            //delete the substitute using recursion
            right = right->helpRemove(this->id);
            std::cout << "successful" << std::endl;

        }
    }

    //Recursive steps
    else if (this->id > _id) left = left->remove(_id);
    else if (this->id < _id) right = right->remove(_id);

    //Check if node is unbalanced
    //O(1)
    height = findMax(left, right);
    //O(1)
    return checkBalance();
}

//O(logN)
//Helper for remove when node has two children, omits print statements. Node exists & has max 1 child.
Node* Node::helpRemove(int _id) {
    //Base case, node  found
    if (this->id == _id) {
        //if node has no children, delete node, return null
        if (!this->left && !this->right) {
            return nullptr;
        }
            //otherwise, node must have one child, delete node, return child
        else return (!this->left)? right : left;

    }

    if (this->id > _id) left = left->helpRemove(_id);
    if (this->id < _id) right = right->helpRemove(_id);

    //Check if node is unbalanced
    //O(1)
    height = findMax(left, right);
    //O(1)
    return checkBalance();
}

//Searches for a node by id
//O(logN)
void Node::search(int _id) {
    //Found
    if (this->id == _id) {
        std::cout << name << std::endl;
        return;
    }
    //Not found
    if (!left && !right) {
        std::cout << "unsuccessful" << std::endl;
        return;
    }
    //Recursively search for node by ID
    if (this->id > _id) left->search(_id);
    if (this->id < _id) right->search(_id);
}
//Searches ENTIRE TREE for a nodes by name, returns if node exists or not.
//O(N)
bool Node::search(const std::string& n) {
    bool current = false;
    //Found
    if (this->name == n) {
        std::cout << id << std::endl;
        current = true;
    }
    //Recursively search for node by name (preOrder)
    bool l = (left)? left->search(n) : current;
    bool r = (right)? right->search(n) : current;
    return (current || l || r);
}

//Print the tree by a specified traversal
//O(N)
void Node::printInorder() {
    if(left) {
        left->printInorder();
        if (left) std::cout << ", ";
    }
    std::cout << name;
    if(right) {
        std::cout << ", ";
        right->printInorder();
    }
}
//O(N)
void Node::printPreorder() {
    std::cout << name;
    if (left) {
        std::cout << ", ";
        left->printPreorder();
    }
    if (right) {
        std::cout << ", ";
        right->printPreorder();
    }
}
//O(N)
void Node::printPostorder() {
    if(left) {
        left->printPostorder();
        std::cout << ", ";
    }
    if (right) {
        right->printPostorder();
        std::cout << ", ";
    }
    std::cout << name;
}
//Prints the max height of the tree
//O(1)
void Node::printLevelCount() const {
    std::cout << height << std::endl;
}

//Helper function for removeInorder()
//O(1), O(logN), O(N)
Node* Node::searchInorder(int n, int N) {
    Node* node = nullptr;
    if (left) node = left->searchInorder(n, N);
    n++;
    if (n == N) return this;
    if (right) node = right->searchInorder(n, N);
    return node;
}
//Removes the Nth null from the inOrder traversal
//O(logN)
Node* Node::removeInorder(int N) {
    //O(N)
    Node* target = searchInorder(0, N);
    //If nullptr, the search is unsuccessful.
    if (!target) {
        std::cout << "unsuccessful" << std::endl;
        return nullptr;
    }

    //If the node has no children, just delete the node, if not, set the node equal to its child
    if (!target->left && !target->right)
         remove(target->id);
    //Check if there one child
    else if (!target->left || !target->right) {
        target = (target->left) ? target->left : target->right;
    } else {
        //There are two children
        Node *sub = target->right;
        //Get next inOrder substitute
        while (sub->left)
            sub = sub->left;
        target->id = sub->id;
        //Delete the substitute
        target->right = target->right->remove(sub->id);
    }
//    }
    //Deletion complete, rebalance the tree
    if (!target) return nullptr;

    //Check if node is unbalanced
    height = findMax(target->left, target->right);
    return target->checkBalance();
}

//Determines which search function to call
Node* whichSearch(Node* root, const std::string& a) {
    if (a.find('"') == std::string::npos) {
        //ID search: Verify the validity of the ID, must be 8 digits long, +1 for null terminator
        unsigned long l = a.size() - a.find_last_of(' ');
        if (l != 9) {
            std::cout << "unsuccessful" << std::endl;
            return root;
        }
        int id = stoi(a.substr(a.find_last_of(' ') + 1, l));
        root->search(id);
        return root;
    } else {
        //Name search
        std::string name = a.substr(a.find('"') + 1, (a.find_last_of('"') - a.find('"') - 1));
        if (!root->search(name)) std::cout << "unsuccessful" << std::endl;
        return root;
    }
}

bool validStr(const std::string& s) {
    for (char c : s) {
        if (!((c > 47 && c < 58) || (c > 64 && c < 91) || (c > 96 && c < 123) || c == 32 || c == 34)) return false;
    } return true;
}

//Responsible for calling the individual functions based on the parsed input
void callFunctions(Node* root, const std::vector<std::string>& args) {

    for (auto &a : args) {
        if (validStr(a)) {
            //Create an input stringstream to parse each word
            std::string func = a.substr(0, a.find(' '));

            if (func == "insert")
                root = callInsert(root, a);
            else if (func == "remove") {
                //Verify the validity of the ID, must be 8 digits long, +1 for null terminator
                unsigned long l = a.size() - a.find_last_of(' ');
                if (l != 9) {
                    std::cout << "unsuccessful" << std::endl;
                    continue;
                }
                int id = stoi(a.substr(a.find_last_of(' ') + 1, l));
                if (!root)
                    std::cout << "unsuccessful" << std::endl;
                else root = root->remove(id);
            } else if (func == "search") {
                if (!root)
                    std::cout << "unsuccessful" << std::endl;
                else
                    root = whichSearch(root, a);
            } else if (func == "printInorder") {
                if (root) {
                    root->printInorder();
                    std::cout << std::endl;
                }
            } else if (func == "printPreorder") {
                if (root) {
                    root->printPreorder();
                    std::cout << std::endl;
                }
            } else if (func == "printPostorder") {
                if (root) {
                    root->printPostorder();
                    std::cout << std::endl;
                }
            } else if (func == "printLevelCount") {
                if (root) root->printLevelCount();
            } else if (func == "removeInorder") {
                if (root) {
                    unsigned long n = a.size() - a.find_last_of(' ');
                    int index = stoi(a.substr(a.find_last_of(' ') + 1, n));
                    root->removeInorder(index);
                }
            } else exit(1);  //Should only reach this point if the command has a typo in it. CASE SENSITIVE.
        } else     std::cout << "unsuccessful" << std::endl; //Special character input
    }
}

int main() {
    int numArgs = 0;
    std::vector <std::string> args;

    std::cin >> numArgs;
    std::cin.ignore();

    for (int i = 0; i < numArgs; i++) {
        std::string arg;
        getline(std::cin, arg);
        args.push_back(arg);
    }

    Node* root = nullptr;
    callFunctions(root, args);

    return 0;
}

//TODO: Go outside