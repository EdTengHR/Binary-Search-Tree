#include "infectionAnalyzer.h"
#include <fstream> //for reading file
#include <queue> //you may use STL queue
#include <algorithm> //you may use STL algorithm
using namespace std;
//you are NOT allowed to include additional libraries yourself

//destructor
//dellocate all the trees
InfectionAnalyzer::~InfectionAnalyzer(){
	for (size_t i = 0; i < trees.size(); ++i){
		delete trees[i];
	}
}

//load the infection file of the specified filename, and construct the infection trees, return true at the end
//for the file format and examples of the resulting trees, see webpage and sample output
//if the file cannot be loaded, do nothing and return false
//note: it is one of the more challenging tasks. if you cannot implement it completely, you may simply manually construct some trees for testing the other parts of your assignment
bool InfectionAnalyzer::loadInfectionFile(const string filename){			//INCOMPLETE
	ifstream ifs(filename);
	if (!ifs) return false;
	else {
		string parent, child;
		while (ifs >> parent >> child){
			Tree<string>* p;
			Tree<string>* c;
			int i = -1;
			int j = -1;
			for (size_t index = 0; index < trees.size(); ++index){
				p = trees[index]->find(parent);							//find the child tree
				if (p){
					i = index;
					break;
				}
			}
			for (size_t index = 0; index < trees.size(); ++index){
				c = trees[index]->find(child);							//find the child tree
				if (c){
					j = index;
					break;
				}
			}
			//at this point, i and j should be the indices for the vector as to where child and parent belong
			if (p && !c){						//if parent can be found in the tree but not child
				p->addChild(parent, child);										//add the child to the parent
			}
			else if (c && !p){		//child found but not parent, meaning that original root has a parent
				c->addRoot(parent);					//also handles the case of the children of the original root
			}
			else if ((!c && !p) || (i == -1 && j == -1)){								//if neither parent nor child can be found, make a new tree
				Tree<string>* newtree = new Tree<string>;
				newtree->addRoot(parent);
				newtree->addChild(parent, child);
				trees.push_back(newtree);
			}
			else {									//both are found, merge the trees if i != j, do nothing if i == j (repeated input)
				if (i != j && (i != -1 || j != -1)){
					p->addChild(parent, child);					//set up a child tree under parent tree so that it can be added
					Tree<string>* temp = p->find(child);		//set up temp tree to be equal to child tree under parent tree
					*temp = *c;								//set child tree to temp tree using = operator
					delete trees[j];
					trees.erase(trees.begin() + j);			//clear the original child tree, since it has now been assigned under parent
					trees.shrink_to_fit();				//guarantee reallocation by shrinking the tree size
				}
			}
		}
		ifs.close();
	}
	return true;
}


//return true if the specified name can be found in any of the infection trees
//return false otherwise
bool InfectionAnalyzer::isInfected(const string name) const{
	for (size_t i = 0; i < trees.size(); ++i){
		if (trees[i]->find(name)){
			return true;
		}
	}
	return false;
}

//check all trees, and find the node with the specified name, and return the name of its parent
//if the node with the specified name is a root node of a tree (i.e. no parent), then return "ZERO" to indicate that the person might be a patient-zero
//if the specified name is not found at all in all trees, return "NA"
//see the sample output for examples
//you cannot use recursion for this function
//for hints on solving this iteratively, you may refer to the webpage
string InfectionAnalyzer::getInfectionSource(const string name) const{
	size_t i;
	bool check = false;
	for (i = 0; i < trees.size(); ++i){
		Tree<string>* t = trees[i]->find(name);
		if (t){								//node with specified name is found
			check = true;
			break;
		}
		else continue;
	}
	if (check == true){
		if (trees[i]->getDepth(name) == 0){
			return "ZERO";
		}
		else {								//now, look through this entire tree to find the parent of name w/o recursion
			queue<Tree<string>*> q;
			q.push(trees[i]);
			while (!q.empty()){				//while q isn't empty
				for (int i = 0; i < q.front()->root->childCount; ++i){		//loop through each of the children
					Tree<string>* child = &q.front()->root->children[i];
					if (child->root->data == name) return q.front()->root->data;	//return parent if child data found
					q.push(child);				//sort of recursion?? adding child to the q so that same process is repeated
				}
				q.pop();			//pop the front (q follows FIFO), since we're done with it
			}
		}
	}
	else {
		return "NA";
	}
	return "NA";
}

//return the depth of the node with the specified name
//if the specified name is not found at all in all trees, return -1
int InfectionAnalyzer::getInfectionGeneration(const string name) const{
	size_t i;
	bool check = false;
	for (i = 0; i < trees.size(); ++i){
		Tree<string>* t = trees[i]->find(name);
		if (t) {
			check = true;
			break;
		}
		else continue;
	}
	if (check == true){
		return trees[i]->getDepth(name);
	}
	else {
		return -1;
	}
}

bool second_sorting(const pair<string, int> &a, const pair<string, int> &b){		//HELPER FN
	return b.second < a.second;
}

//generate the infectionPowerVector and return it
//the infection power of a person is defined as the number of descendants it has in a tree
//the name-power pairs should be ordered by the infection power in descending order
//if two pairs have the the same power, then they are ordered by their names in ascending order (hint: you can directly use > or < to compare strings)
//see the sample output for examples
//you cannot use recursion for this function
//for hints on solving this iteratively, you may refer to the webpage
//note: you should clear the infectionPowerVector at the beginning of this function
const vector<pair<string, int>>& InfectionAnalyzer::getInfectionPowerVector(){
	infectionPowerVector.clear();
	for (vector<Tree<string>*>::iterator it = trees.begin(); it != trees.end(); ++it){
		int i = distance(trees.begin(), it);
		queue<Tree<string>*> q;
		q.push(trees[i]);
		while (!q.empty()){
			for (int j = 0; j < q.front()->root->childCount; ++j){			//loop through every tree in the vector trees
				q.push(&q.front()->root->children[j]);
			}
			infectionPowerVector.push_back(make_pair(q.front()->root->data, q.front()->getDescendantCount()));
			q.pop();
		}
	}
	sort(infectionPowerVector.begin(), infectionPowerVector.end());
	sort(infectionPowerVector.begin(), infectionPowerVector.end(), second_sorting);
	return infectionPowerVector;
}










