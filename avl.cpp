#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Node{
private:
    std::string name;
    int id;

public:
    Node();
    Node(std::string name_, int id_);
    void setName(std::string name_);
    std::string getName();
    void setID(int id_);
    int getID();

    Node* left;
    Node* right;
};

Node::Node() {
    name = "";
    id = 0;
    left = nullptr;
    right = nullptr;
};

Node::Node(std::string name_, int id_) {
    name = name_;
    id = id_;
    left = nullptr;
    right = nullptr;
};

void Node::setName(std::string name_) {
    name = name_;
};

std::string Node::getName() {
    return name;
};

void Node::setID(int id_) {
    id = id_;
};

int Node::getID() {
    return id;
};

class AvlTree {
public:
    AvlTree();
    bool insert(std::string name, int id); //Starts insertion method
    bool remove(int id); //Starts remove method

    string search(int id); //Starts recursive search method by id
    string search(std::string name); //Starts inorder search method by name

    string printInorder(); //Starts inorder function
    string printPreorder(); //Starts preorder function
    string printPostorder(); //Starts postorder function
    int printLevelCount(); //Prints the number of levels in tree

    bool removeInorder(int N);
private:
    Node* root;
    int size = 0;
    int newsize = 0;
    int recursionIndex = 0; //used to help keep count of recursions
	string result = ""; //used to help track traversals

    Node* insertion(Node* node, std::string name, int id);
    Node* removal(Node* node, int id);

    std::string recursiveSearch(Node* node, int id);
    std::vector<int> preorderSearch(Node* node, std::string name, std::vector<int>& ids);

    void inorder(Node* root);
    void preorder(Node* root);
    void postorder(Node* root);

    void removalInorder(Node* node, int& N);

    int height(Node* node);
    int getBalFactor(Node* node);
    Node* findInorderSuccessor(Node* node);
    Node* findInorderPredecessor(Node* node);

    Node* rotateLeft(Node* node);
    Node* rotateRight(Node* node);
};
  

AvlTree::AvlTree() {
	size = 0;
	newsize = 0;
	root = nullptr;
};

bool AvlTree::insert(std::string name, int id) {
	if (root == nullptr) {
		Node* root_ = new Node(name, id);
		root = root_;
		std::cout << "successful" << std::endl;
		size++;
		newsize = size;
		return true;
	}
	else {
		insertion(root, name, id);
		if (newsize == size) {
			std::cout << "unsuccessful" << std::endl;
			return false;
		}
		else {
			std::cout << "successful" << std::endl;
			size = newsize;
			return true;
		}
	}
};

Node* AvlTree::insertion(Node* node, std::string name, int id) {
	if (node == nullptr) {
		Node* node_ = new Node(name, id);
		newsize++;
		return node_;
	}
	else {
		if (id > node->getID()) {
			node->right = insertion(node->right, name, id);
		}
		else if (id < node->getID()) {
			node->left = insertion(node->left, name, id);
		}
		else {
			return node;
		}
	}

	int balFactor = getBalFactor(node);

	if (balFactor > 1) { //we have some sort of left imbalance
		if (id > node->left->getID()) { //we have a left right imbalance
			node->left = rotateLeft(node->left);
			return rotateRight(node);
		}
		else { //we have a left left imbalance
			return rotateRight(node);
		}
	}
	else if (balFactor < -1) { //we have some sort of right imbalance
		if (id < node->right->getID()) { //we have a right left imbalance
			node->right = rotateRight(node->right);
			return rotateLeft(node);
		}
		else { //we have a right right imbalance
			return rotateLeft(node);
		}
	}

	return node;
}

bool AvlTree::remove(int id) {
	if (root == nullptr) {
		std::cout << "unsuccessful" << std::endl;
		return false;
	}
	else {
		removal(root, id);
		if (newsize == size) {
			std::cout << "unsuccessful" << std::endl;
			return false;
		}
		else {
			std::cout << "successful" << std::endl;
			size = newsize;
			return true;
		}
	}
};

Node* AvlTree::removal(Node* node, int id) {
	if (node == nullptr) {
		return nullptr;
	}
	else {
		if (id > node->getID()) {
			node->right = removal(node->right, id);
			return node;
		}
		else if (id < node->getID()) {
			node->left = removal(node->left, id);
			return node;
		}
		else {
			if (node->right != nullptr) {//we will prioritize inorderSuccessor
				Node* inorderSuccessor = findInorderSuccessor(node->right);
				if (node == root) {
					root = inorderSuccessor;
				}
				inorderSuccessor->left = node->left;
				if (inorderSuccessor != node->right) {
					inorderSuccessor->right = node->right;
				}
				delete node;
				node = nullptr;
				newsize--;
				return inorderSuccessor;
			}
			else if (node->left != nullptr) { //if inorderSuccessor DNE, we will use inorderPredecessor
				Node* inorderPredecessor = findInorderPredecessor(node->left);
				if (node == root) {
					root = inorderPredecessor;
				}
				inorderPredecessor->right = node->right;
				if (inorderPredecessor != node->left) {
					inorderPredecessor->left = node->left;
				}
				delete node;
				node = nullptr;
				newsize--;
				return inorderPredecessor;
			}
			else { //If node has no children, it is a leaf and no need to replace it with anything
				if (node == root) {
					root = nullptr;
				}
				delete node;
				newsize--;
				return nullptr;
			}
		}
	}
}

bool AvlTree::removeInorder(int N) {
	recursionIndex = 0;
	if (root == nullptr) {
		std::cout << "unsuccessful" << std::endl;
		return false;
	}
	else {
		removalInorder(root, N);
		if (N == -1) {
			return true;
		}
		else {
			return false;
		}
	}
};

//This private helper function is called so that I can implement removeInorder function the way I want
void AvlTree::removalInorder(Node* node, int& N) {
	if (node == nullptr) {
		return;
	}
	else {
		removalInorder(node->left, N);
		if (recursionIndex == N) {
			remove(node->getID());
			N = -1;
			return;
		}
		recursionIndex++;
		removalInorder(node->right, N);
	}
};

std::string AvlTree::search(int id) {
	std::string result = recursiveSearch(root, id);
	std::cout << result << std::endl;
	return result;
};

std::string AvlTree::recursiveSearch(Node* node, int id) {
	if (node == nullptr) {
		return "unsuccessful";
	}
	else {
		if (id > node->getID()) {
			return recursiveSearch(node->right, id);
		}
		else if (id < node->getID()) {
			return recursiveSearch(node->left, id);
		}
		else {
			return node->getName();
		}
	}
}

string AvlTree::search(std::string name) {
	std::vector<int> ids;
	string search_result = "";
	ids = preorderSearch(root, name, ids);
	if (ids.size() == 0) {
		std::cout << "unsuccessful" << std::endl;
		return "unsuccessful";
	}
	for (unsigned int i = 0; i < ids.size(); i++) {
		std::cout << ids[i] << "\n";
		search_result += to_string(ids[i]);
		if (i != ids.size() - 1)
			search_result += ", ";
	}
	return search_result;
};

std::vector<int> AvlTree::preorderSearch(Node* node, std::string name, std::vector<int>& ids) {
	if (node == nullptr) {
		return ids;
	}
	else {
		if (name.compare(node->getName()) == 0) {
			ids.push_back(node->getID());
		}
		preorderSearch(node->left, name, ids);
		preorderSearch(node->right, name, ids);
	}
	return ids;
};

string AvlTree::printInorder() {
	recursionIndex = 0;
	result = "";
	inorder(root);
	return result;
};

void AvlTree::inorder(Node* root) {
	if (root == nullptr) {
		return;
	}
	else {
		inorder(root->left);
		if (recursionIndex == size - 1) {
			std::cout << root->getName() << "\n";
			result += root->getName();
		}
		else {
			std::cout << root->getName() << ", ";
			result += root->getName() + ", ";
		}
		recursionIndex++;
		inorder(root->right);
	}
};

//Calls preorder function
string AvlTree::printPreorder() {
	recursionIndex = 0;
	result = "";
	preorder(root);
	return result;
};

//Prints out names in tree with preorder traversal
void AvlTree::preorder(Node* root) {
	if (root == nullptr) {
		return;
	}
	else {
		if (recursionIndex == size - 1) {
			std::cout << root->getName() << "\n";
			result += root->getName();
		}
		else {
			std::cout << root->getName() << ", ";
			result += root->getName() + ", ";
		}
		recursionIndex++;
		preorder(root->left);
		preorder(root->right);
	}
};

//Calls postorder function
string AvlTree::printPostorder() {
	recursionIndex = 0;
	result = "";
	postorder(root);
	return result;
};

//Prints out names in tree with post order traversal
void AvlTree::postorder(Node* root) {
	if (root == nullptr) {
		return;
	}
	else {
		postorder(root->left);
		postorder(root->right);
		if (recursionIndex == size - 1) {
			std::cout << root->getName() << "\n";
			result += root->getName();
		}
		else {
			std::cout << root->getName() << ", ";
			result += root->getName() + ", ";
		}
		recursionIndex++;
	}
};

int AvlTree::printLevelCount() {
	int height_root = height(root);
	std::cout << height_root << std::endl;
	return height_root;
};

//obtains the height of each sub-branch given a root (obtained from my stepik question 5.2)
int AvlTree::height(Node* node) { //O(n)
	if (node == nullptr) {
		return 0;
	}
	else {
		int a = height(node->left);
		int b = height(node->right);
		if (a > b) {
			return 1 + a;
		}
		else {
			return 1 + b;
		}
	}
}

int AvlTree::getBalFactor(Node* node) {
	if (node == nullptr) {
		return 0;
	}
	else {
		return (height(node->left) - height(node->right));
	}
}

//Rotate left (obtained from my stepik 5.1)
Node* AvlTree::rotateLeft(Node* node)
{
	Node* temp = node->right->left; //stores grandchild
	node->right->left = node; //places root node where grandchild was
	if (node == root) {
		root = node->right;
	}
	Node* newroot = node->right; //stores the new root
	node->right = temp; //places original grandchild as child of old root
	return newroot;
}

//Rotate right (obtained from my stepik 5.1)
Node* AvlTree::rotateRight(Node* node)
{
	Node* temp = node->left->right; //stores grandchild
	node->left->right = node; //places root node where grandchild was
	if (node == root) {
		root = node->left;
	}
	Node* newroot = node->left; //stores the new root
	node->left = temp; //places original grandchild as child of old root
	return newroot;
}

//requires inserted parameter to be target_node->right
Node* AvlTree::findInorderSuccessor(Node* node) {
	if (node->left == nullptr) {
		return node;
	}
	else if (node->left->left == nullptr) {
		Node* temp = node->left;
		node->left = node->left->right;
		return temp;
	}
	else {
		return findInorderSuccessor(node->left);
	}
};

//requires inserted parameter to be target_node->left
Node* AvlTree::findInorderPredecessor(Node* node) {
	if (node->right == nullptr) {
		return node;
	}
	else if (node->right->right == nullptr) {
		Node* temp = node->right;
		node->right = node->right->left;
		return temp;
	}
	else {
		return findInorderPredecessor(node->right);
	}
};

int main(){
	int number_of_commands;
	vector<string> main_commands;
	vector<string> specific_commands;
	string temp_main;
	string temp_sub;
	int delimiter;


	cin >> number_of_commands;
	getline(cin, temp_sub);
	for (int i = 0; i < number_of_commands; i++) {
		//this loop attempts to store each line that the user inputs parsed into some vectors
		//separating the command (main_commands) and the content of the command (specific_commands)
		getline(cin, temp_sub);
		delimiter = temp_sub.find(" ");
		temp_main = temp_sub.substr(0, delimiter);
		temp_sub.erase(0, delimiter + 1);
		main_commands.push_back(temp_main);
		specific_commands.push_back(temp_sub);
	}

	AvlTree* tree = new AvlTree();
	string name;
	string id;
	for (int i = 0; i < number_of_commands; i++) {
		if (main_commands[i].compare("insert") == 0) {

			//ensures that name is surrounded by quotation marks and parses name out of those marks
			if (specific_commands[i][0] != '\"') {
				cout << "unsuccessful" << endl;
				break;
			}
			specific_commands[i] = specific_commands[i].substr(1, specific_commands[i].length() - 1);
			delimiter = specific_commands[i].find("\"");
			if (delimiter == -1) {
				cout << "unsuccessful" << endl;
				break;
			}
			name = specific_commands[i].substr(0, delimiter);

			//checks if name is within constrains of a-z or A-Z or spaces
			bool success = true;
			for (unsigned int i = 0; i < name.length(); i++) {
				if (!(name[i] >= 65 && name[i] <= 90 || name[i] >= 97 && name[i] <= 122 || name[i] == 32)) {
					success = false;
					break;
				}
			}
			if (success == false) {
				cout << "unsuccessful" << endl;
				break;
			}

			specific_commands[i].erase(0, delimiter + 2);
			id = specific_commands[i];
			//checks if id is within constaints of 0-9
			for (unsigned int i = 0; i < id.length(); i++) {
				if (!(id[i] >= 48 && id[i] <= 57)) {
					success = false;
					break;
				}
			}
			if (success == false) {
				cout << "unsuccessful" << endl;
				break;
			}

			//checks to see if id is 8 characters long
			if (id.length() != 8) {
				cout << "unsuccessful" << endl;
				break;
			}
			tree->insert(name, stoi(id));
		}
		//executes if search command is used
		else if (main_commands[i].compare("search") == 0) {
			//if a quotation mark is not present, we know that we are searching with an id
			if (specific_commands[i].find("\"") == -1) {
				tree->search(stoi(specific_commands[i]));
			}
			else { //if one is present, it will create a substring excluding marks
				name = specific_commands[i].substr(1, specific_commands[i].length() - 2);
				tree->search(name);
			}
		}
		//executes if remove command is used
		else if (main_commands[i].compare("remove") == 0) {
			tree->remove(stoi(specific_commands[i]));
		}
		//executes if printInorder command is used
		else if (main_commands[i].compare("printInorder") == 0) {
			tree->printInorder();
		}
		//exceutes if printPreorder command is used
		else if (main_commands[i].compare("printPreorder") == 0) {
			tree->printPreorder();
		}
		//executes if printPostorder command is used
		else if (main_commands[i].compare("printPostorder") == 0) {
			tree->printPostorder();
		}
		//executes if printLevelCount command is used
		else if (main_commands[i].compare("printLevelCount") == 0) {
			tree->printLevelCount();
		}
		//executes if removeInorder command is used
		else if (main_commands[i].compare("removeInorder") == 0) {
			tree->removeInorder(stoi(specific_commands[i]));
		}
	}
}

