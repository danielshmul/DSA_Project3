/*
 * Charles Richardson Data Structures and Algorithms Project 1 Resubmission Mar 12 2021
 * Failed cases: insert randomly, removeInorder (edge cases), printPreorder,
 *              insert 100+ entries, print all three traversals
 *              insert 100+ entries, remove, search, and printPreorder and levelCount */

#include <iostream>
#include <utility>
#include <vector>
#include "odom.h"
using namespace std;

//Encapsulating class, responsible for all node methods
class Node {

public:
    Node();
    Node(Odom* name_, int id);
    Node* left = nullptr;
    Node* right = nullptr;

    static int count;
    Odom* name = nullptr;
    int id = -1, height = 0;

    static int findMax(Node* left, Node* right);
    int balanceFactor();
    Node* checkBalance();
    Node* checkRotation(int _id);

    Node* rotateLeft();
    Node* rotateRight();

    Node* insert(Odom* n_, int _id);
    Node* remove(int _id);
    Node* helpRemove(int _id);

    Odom* search(Node* root, int _id);

    double minInorder(double min, string option, int min_time, int max_time);
    double maxInorder(double max, string option, int min_time, int max_time);
    double sumInorder(double sum, string option, int min_time, int max_time);
};


//************* METHOD DEFINITIONS **************//
int Node::count = 0;
//Constructors
Node::Node() {
    id = -1;
    height = 0;
    left = nullptr;
    right = nullptr;
}
Node::Node(Odom* name_, int id) {
    this->id = id;
    this->name = name_;
    height = 1;
    left = nullptr;
    right = nullptr;
    count++;
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

//Places a node into the tree, given it already has a nonnull root
//O(logN)
Node* Node::insert(Odom* n_, int _id) {    
    //Check left
    //O(logN)
    if (this->id > _id) {
        if (!left) { //Left node is null
            left = new Node(n_, _id);
        }
        else {
            this->left = left->insert(n_, _id); 
        }
    }
    //Check right
    else if (this->id < _id) {
        if (!right) {
            right = new Node(n_, _id);
        }
        else { 
            this->right = right->insert(n_, _id);
        }
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
            return nullptr;
        }
        //if node has one child, delete node, return child
        else if (!this->left || !this->right) {
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
Odom* Node::search(Node* root, int _id) {
    if (root == nullptr) {
        return nullptr;
    }
    else {
        cout << _id << " " << root->id << endl;
        if (_id > root->id) {
            return right->search(root->right, _id);
        }
        else if (_id < root->id) {
            return left->search(root->left, _id);
        }
        else {
            return name;
        }
    }
}

double Node::minInorder(double min, string option, int min_time, int max_time) {
    if (left) {
        min = left->minInorder(min, option, min_time, max_time);
    }
    
    if (option == "1" && name->px_() < min && name->t_() >= min_time && name->t_() <= max_time) {
        min = name->px_();
    } 
    else if (option == "2" && name->py_() < min && name->t_() >= min_time && name->t_() <= max_time) {
        min = name->py_();
    }
    else if (option == "3" && name->pz_() < min && name->t_() >= min_time && name->t_() <= max_time) {
        min = name->pz_();
    }       
    else if (option == "4" && name->lvx_() < min && name->t_() >= min_time && name->t_() <= max_time) {
        min = name->lvx_();
    }       
    else if (option == "5" && name->lvy_() < min && name->t_() >= min_time && name->t_() <= max_time) {
        min = name->lvy_();
    }       
    else if (option == "6" && name->lvz_() < min && name->t_() >= min_time && name->t_() <= max_time) {
        min = name->lvz_();
    }       
    else if (option == "7" && name->avx_() < min && name->t_() >= min_time && name->t_() <= max_time) {
        min = name->avx_();
    }       
    else if (option == "8" && name->avy_() < min && name->t_() >= min_time && name->t_() <= max_time) {
        min = name->avy_();
    }
    else if (option == "9" && name->avz_() < min && name->t_() >= min_time && name->t_() <= max_time) {
        min = name->avz_();
    }


    if (right) {
        min = right->minInorder(min, option, min_time, max_time);
    }

    return min;
}

double Node::maxInorder(double max, string option, int min_time, int max_time) {
    if (left) {
        max = left->maxInorder(max, option, min_time, max_time);
    }

    if (option == "1" && name->px_() > max && name->t_() >= min_time && name->t_() <= max_time) {
        max = name->px_();
    }
    else if (option == "2" && name->py_() > max && name->t_() >= min_time && name->t_() <= max_time) {
        max = name->py_();
    }
    else if (option == "3" && name->pz_() > max && name->t_() >= min_time && name->t_() <= max_time) {
        max = name->pz_();
    }
    else if (option == "4" && name->lvx_() > max && name->t_() >= min_time && name->t_() <= max_time) {
        max = name->lvx_();
    }
    else if (option == "5" && name->lvy_() > max && name->t_() >= min_time && name->t_() <= max_time) {
        max = name->lvy_();
    }
    else if (option == "6" && name->lvz_() > max && name->t_() >= min_time && name->t_() <= max_time) {
        max = name->lvz_();
    }
    else if (option == "7" && name->avx_() > max && name->t_() >= min_time && name->t_() <= max_time) {
        max = name->avx_();
    }
    else if (option == "8" && name->avy_() > max && name->t_() >= min_time && name->t_() <= max_time) {
        max = name->avy_();
    }
    else if (option == "9" && name->avz_() > max && name->t_() >= min_time && name->t_() <= max_time) {
        max = name->avz_();
    }


    if (right) {
        max = right->maxInorder(max, option, min_time, max_time);
    }

    return max;
}

double Node::sumInorder(double sum, string option, int min_time, int max_time) {
    if (left) {
        sum = left->sumInorder(sum, option, min_time, max_time);
    }

    if (option == "1" && name->t_() >= min_time && name->t_() <= max_time) {
        sum += name->px_();
    }
    else if (option == "2" && name->t_() >= min_time && name->t_() <= max_time) {
        sum += name->py_();
    }
    else if (option == "3" && name->t_() >= min_time && name->t_() <= max_time) {
        sum += name->pz_();
    }
    else if (option == "4" && name->t_() >= min_time && name->t_() <= max_time) {
        sum += name->lvx_();
    }
    else if (option == "5" && name->t_() >= min_time && name->t_() <= max_time) {
        sum += name->lvy_();
    }
    else if (option == "6" && name->t_() >= min_time && name->t_() <= max_time) {
        sum += name->lvz_();
    }
    else if (option == "7" && name->t_() >= min_time && name->t_() <= max_time) {
        sum += name->avx_();
    }
    else if (option == "8" && name->t_() >= min_time && name->t_() <= max_time) {
        sum += name->avy_();
    }
    else if (option == "9" && name->t_() >= min_time && name->t_() <= max_time) {
        sum += name->avz_();
    }


    if (right) {
        sum = right->sumInorder(sum, option, min_time, max_time);
    }

    return sum;
}