#ifndef _MAIN_H_
#define _MAIN_H_
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

#define BOUND 1

typedef struct myNode
{
	int depth;
	double x, y, bx1, bx2, by1, by2;
	struct myNode* left = NULL;
	struct myNode* right = NULL;
	struct myNode* parent = NULL;
	bool black;
	myNode(double x_in, double y_in): x(x_in), y(y_in) {depth = 0;}
	myNode() {}
} Node;

extern Node* best;
extern double best_dist;

void Insert(double x, double y, Node* &root){
	//if(!root && root->depth != 0) return;	//leaf
	double num = 0, num1 = 0;
	//cout<<isnan(root->x)<<endl;
	if(!root) {
		Node* _node = new Node(x,y);
		_node->bx1 = 0;
		_node->by1 = 0;
		_node->bx2 = 1;
		_node->by2 = 1;
		root = _node; return;
	}
	else{
		if(root->depth % 2) {		//odd depth
			num = root->y;
			num1 = y;
		}	
		else {						//even depth
			num = root->x;
			num1 = x;
		}						

		if(num1 <= num){
			if(root->left) Insert(x, y, root->left);
			else {
				Node* _node = new Node(x,y);
				_node->depth = root->depth + 1;
				_node->parent = root;
				_node->bx1 = (_node->parent)->bx1;
				_node->by1 = (_node->parent)->by1;
				_node->bx2 = (_node->depth % 2 == 0) ? (_node->parent)->bx2 : (_node->parent)->x;
				_node->by2 = (_node->depth % 2 == 0) ? (_node->parent)->y : (_node->parent)->by2;
				root->left = _node;
				return;
			}
		}
		else{
			if(root->right) Insert(x, y, root->right);
			else {
				Node* _node = new Node(x,y);
				_node->depth = root->depth + 1;
				_node->parent = root;
				_node->bx1 = (_node->depth % 2 == 0) ? (_node->parent)->bx1 : (_node->parent)->x;
				_node->by1 = (_node->depth % 2 == 0) ? (_node->parent)->y : (_node->parent)->by1;
				_node->bx2 = (_node->parent)->bx2;
				_node->by2 = (_node->parent)->by2;
				root->right = _node;
				return;
			}
		}
	}
	//Insert(x, y, root);	
}

void Traverse(Node* root){
	//if(!root && root->depth != 0) return;
	if(root->left) Traverse(root->left);
	if(root->right) Traverse(root->right);
	cout << root->x << "\t" << root->y << endl;
}

Node* FindMin(Node* &root, bool y){
	if(!root) return NULL;
	if(y == (root->depth % 2)){
		if(!(root->left)) return root;
		else return FindMin(root->left, y);
	}
	else{
		Node* l = FindMin(root->left, y);
		Node* r = FindMin(root->right, y);
		if(y){
			Node* tmp;
			if(!l && !r) tmp = root;
			else if(l && r) tmp = (l->y < r->y) ? l : r;
			else tmp = (!l) ?r :l;
			tmp = (tmp->y < root->y) ? tmp : root;
			return tmp;
		}
		else{
			Node* tmp;
			if(!l && !r) tmp = root;
			else if(l && r) tmp = (l->x < r->x) ? l : r;
			else tmp = (!l) ?r :l;
			tmp = (tmp->x < root->x) ? tmp : root;
			return tmp;
		}
	}
}

Node* Delete(double x, double y, Node* &root){
	//if(!root && root->depth != 0) return;	//leaf
	double num = 0, num1 = 0;
	if(!root) {
		return NULL;
	}
	else{
		if(root->depth % 2) {		//odd depth
			num = root->y;
			num1 = y;
		}	
		else {						//even depth
			num = root->x;
			num1 = x;
		}						

		if(num1 == num){
			if(root->right){
				Node* tmp = FindMin(root->right, root->depth % 2);
				root->x = tmp->x; root->y = tmp->y;
				root->right = Delete(root->x, root->y, root->right);
			}
			else if(root->left){
				Node* tmp = FindMin(root->left, root->depth % 2);
				root->x = tmp->x; root->y = tmp->y;
				root->right = Delete(root->x, root->y, root->left);
				root->left = NULL;
			}
			else {
				delete root;
				root = NULL;
				return NULL;
			}

		}
		else if(num1 < num){
			root->left = Delete(x, y, root->left);
		}
		else{
			root->right = Delete(x, y, root->right);
		}
		return root;
	}	
}

void Clear(Node* &root){
	if(root->left) Clear(root->left);
	if(root->right) Clear(root->right);
	delete root;
	root = NULL;
}

void RangeSearch(double x1, double y1, double x2, double y2, vector<Node>& list, Node* root){
	if(!root) return;
	//p1 : smaller x, y
	double tmpx = (x1 < x2) ? x1 : x2, tmpy = (y1 < y2) ? y1 : y2;
	x2 = (x1 < x2) ? x2 : x1; y2 = (y1 < y2) ? y2 : y1;
	x1 = tmpx; y1 = tmpy;
	if(root->x >= x1 && root->x <= x2 && root->y >= y1 && root->y <= y2){
		//printf("(%f, %f)\n",root->x, root->y);	//node is in the rect
		list.push_back(*root);
	}
	if(root->left){
		if(root->depth % 2 == 0){	
			if(x2 <= root->x || (x1 <= root->x && x2 >= root->x))	RangeSearch(x1, y1, x2, y2, list, root->left);
		}
		else{
			if(y2 <= root->y || (y1 <= root->y && y2 >= root->y))	RangeSearch(x1, y1, x2, y2, list, root->left);
		}
	}
	if(root->right){
		if(root->depth % 2 == 0){
			if(x1 >= root->x || (x1 <= root->x && x2 >= root->x))	RangeSearch(x1, y1, x2, y2, list, root->right);
		}
		else{
			if(y1 >= root->y || (y1 <= root->y && y2 >= root->y))	RangeSearch(x1, y1, x2, y2, list, root->right);
		}
	}

}

void InitNN(){
	best = NULL;
	best_dist = 1.0/0.0;
}

double Distance(double x, double y, Node* root){
	if(x <= root->bx2 && x >= root->bx1 && y <= root->by2 && y >= root->by1) return -1;	//inside
	if(x <= root->bx2 && x >= root->bx1) return (y < root->by1) ? (y - root->by1) * (y - root->by1) : (y - root->by2) * (y - root->by2);
	if(y <= root->by2 && y >= root->by1) return (x < root->bx1) ? (x - root->bx1) * (x - root->bx1) : (x - root->bx2) * (x - root->bx2);
	double tmpx = ((x - root->bx1) * (x - root->bx1) > (x - root->bx2) * (x - root->bx2)) ? root->bx2 : root->bx1;
	double tmpy = ((y - root->by1) * (y - root->by1) > (y - root->by2) * (y - root->by2)) ? root->by2 : root->by1;
	return (x - tmpx) * (x - tmpx) + (y - tmpy) * (y - tmpy);	// corner
}

void NNSearch(double x, double y, Node *root){
	if(!root || Distance(x, y, root) > best_dist) return;
	if(root->depth == 0) InitNN();
	double dist = (x - root->x) * (x - root->x) + (y - root->y) * (y - root->y);
	if(dist < best_dist){
		best = root;
		best_dist = dist;
	}
	if(root->depth % 2 == 0){
		if(x < root->x)	{
			NNSearch(x, y, root->left);
			NNSearch(x, y, root->right);
		}
		else{
			NNSearch(x, y, root->right);
			NNSearch(x, y, root->left);
		}
	}
	else{
		if(y < root->y){
			NNSearch(x, y, root->left);
			NNSearch(x, y, root->right);
		}
		else{ 
			NNSearch(x, y, root->right);
			NNSearch(x, y, root->left);
		}
	}
}

#endif