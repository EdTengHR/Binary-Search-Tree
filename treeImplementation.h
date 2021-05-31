
using namespace std;

//deep copy constructor

template <typename T>
Tree<T>::Tree(const Tree& t){
	if (t.isEmpty()) return;
	root = new Node<T>(*t.root);
}

//destructor

template <typename T>
Tree<T>::~Tree(){
	delete root;
}

//deep assignment
//remember to check for self-assignment: if it is a self-assignmnt, do NOT copy
template <typename T>
const Tree<T>& Tree<T>::operator=(const Tree<T>& another){
	if (this != &another){			//avoid self assignment
		delete root;
		root = new Node<T>(*another.root);				//double check if it will lead to infinite loop
	}
	return *this;
}

//search in the whole tree
//and return the tree pointer to the tree with the root node that contains the specified data
//return nullptr if the specified data is not found
template <typename T>
Tree<T>* Tree<T>::find(const T& data){
	if (root->data == data) return this;
	for (int i = 0; i < root->childCount; ++i){
		Tree* res = root->children[i].find(data);
		if (res == nullptr) continue;
		else return res;
	}
	return nullptr;
}

//this is the const version of find
template <typename T>
const Tree<T>* Tree<T>::find(const T& data) const{
	const Tree* node = const_cast<const Tree*>(this);
	if (root->data == data) return node;
	for (int i = 0; i < root->childCount; ++i){
		const Tree* res = root->children[i].find(data);
		if (res == nullptr) continue;
		else return res;
	}
	return nullptr;
}

//return the depth of the node that contains the specified data in this whole tree
//return -1 if the tree is empty or the data is not found
//note: root node has a depth of 0
template <typename T>
int Tree<T>::getDepth(const T& data) const{
	if (!root || !find(data)) return -1;					//if root or find data == nullptr
	if (root->data == data) return 0;
	else {
		for (int i = 0; i < root->childCount; ++i){
			int depth = root->children[i].getDepth(data);
			if (depth >= 0){
				return 1 + depth;
			}
			else {
				continue;
			}
		}
	}
	return -1;
}

//return the descendant count of the node that contains the specified data in this whole tree
//return -1 if the tree is empty or the data is not found
//note: any direct or indirect child of a node is considered as a descendant of a node
//see sample output for examples
//hint: you may make use of the overloaded version of this function that takes no parameter
template <typename T>
int Tree<T>::getDescendantCount(const T& data) const{
	const Tree* t = find(data);
	return (t == nullptr) ? -1: t->getDescendantCount();
}

//return the descendant count of the root node in this whole tree
//return -1 if the tree is empty
template <typename T>
int Tree<T>::getDescendantCount() const{
	if (root == nullptr) return -1;
	else {
		int res = 0;
		for (int i = 0; i < root->childCount; ++i){
			res += root->children[i].getDescendantCount();
		}
		return res + root->childCount;
	}
}

//add a new root with the specified data to the tree, and return true at the end
//if the tree already has a root (i.e. non-empty tree), then make a new root while keeping the existing tree
//that means, you will create a new root node with the specified data; it also has exactly one child which is the old root
//note: if the specified data already exists in the tree, do nothing and return false
template <typename T>
bool Tree<T>::addRoot(const T& data){
	if (root == nullptr){
		root = new Node<T>(data);
		return true;
	}
	else {
		if (find(data)) return false;
		Node<T>* old = root;
		root = new Node<T>(data, 1);
		root->children[0].root = old;
		return true;
	}
}

//add a new node (child with data being childData) to an existing node (parent with data being parentData) in the tree, and return true at the end
//however, if the parent node is not found in the tree or if the child data is already in the tree, do nothing and return false
//you should add the child to the parent node's children array so that the children[0] is the first child ever added, children[1] is the second one,
//and so on
//this is to make sure your output is always the same as ours
template <typename T>
bool Tree<T>::addChild(const T& parentData, const T& childData){				//INCOMPLETE - MEMLEAK?
	if (!find(parentData)) return false;
	Tree* parent = find(parentData);
	int childCount = parent->root->childCount;
	Node<T>* cnode = new Node<T>(childData);			//Create the child node
	Node<T>* pnode = new Node<T>(parentData, childCount + 1);
	for (int i = 0; i < childCount; ++i){
		pnode->children[i] = parent->root->children[i];
	}
	pnode->children[childCount].root = cnode;

	delete parent->root;
	parent->root = pnode;

	return true;
}







