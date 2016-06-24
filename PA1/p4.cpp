#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include "main.h"
using namespace std;
Node* best;
double best_dist = 1.0/0.0;
void RB_InsertFixup(Node* &z, Node* &root);
void RB_Insert(double x , double y,  Node* &root);
void RB_RangeSearch(double n1, double n2, vector<Node> &list, Node* root);
void RB_NNSearch(double x, double y, Node* root);

int main(int argc, char** argv){
	if(argc % 2 != 0){
		cerr << "Invalid input arguments!\n";
		exit(-1);
	}

	FILE* file = fopen(argv[1], "r");
	if(!file) exit(-1);
	char out_name[] = "P4_result_";
	FILE* out = fopen(strcat(out_name, argv[1]), "w");
	double x, y, bx1, by1, bx2, by2;
	Node *root = NULL;
	vector<Node> list;
	//clock_t timer = clock();
	
	if(argc == 2){
		x = 0.5; y = 0.5;
		bx1 = 0.3; by1 = 0.3; bx2 = 0.6; by2 = 0.41;
	}
	if(argc == 4 || argc == 8){
		x = atof(argv[2]);
		y = atof(argv[3]);
	}
	if(argc == 6 || argc == 8){
		bx1 = (atof(argv[argc - 4]) < atof(argv[argc - 2])) ? atof(argv[argc - 4]) : atof(argv[argc - 2]);
		by1 = (atof(argv[argc - 3]) < atof(argv[argc - 1])) ? atof(argv[argc - 3]) : atof(argv[argc - 1]);
		bx2 = (atof(argv[argc - 4]) > atof(argv[argc - 2])) ? atof(argv[argc - 4]) : atof(argv[argc - 2]);
		by2 = (atof(argv[argc - 3]) > atof(argv[argc - 1])) ? atof(argv[argc - 3]) : atof(argv[argc - 1]);
	}

	while(!feof(file)){
		double tmpx, tmpy;
		fscanf(file, "%lf", &tmpx);
		fscanf(file, "%lf", &tmpy);
		//cout<<x<<" "<<y<<endl;
		RB_Insert(tmpx, tmpy, root);
	}
	
	if(argc != 6){
		RB_NNSearch(x, y, root);
		fprintf(out, "The nearest neighbor of (%lf, %lf) is (%lf, %lf).\n\n", x, y, best->x, best->y);
	}

	if(argc != 4){
		vector<Node> list;
		RB_RangeSearch(bx1, bx2, list, root);
		Node* root2 = NULL;
		for(size_t i = 0; i < list.size(); ++i){
			RB_Insert(list[i].y, list[i].x, root2);
		}
		list.clear();
		RB_RangeSearch(by1, by2, list, root2);

		fprintf(out, "There are %d points in the rectangle (%f, %f), (%f, %f), (%f, %f), (%f, %f): \n", list.size(), bx1, by1, bx1, by2, bx2, by1, bx2, by2);
		for(unsigned int i = 0; i < list.size(); ++i){
			fprintf(out, "(%lf, %lf)\n", list[i].x, list[i].y);
		}
		if(root2) Clear(root2);
	}
	
	clock_t timer = clock();
	int count = 0;
	srand(time(NULL));
	while(double(clock() - timer)/CLOCKS_PER_SEC < 1.0){
		x = double(rand()%10000001) / 10000000.0;
		y = double(rand()%10000001) / 10000000.0;
		RB_NNSearch(x, y, root);
		++count;
	}
	fprintf(out, "\nWe can perform %d NN calculations in 1 second.\n", count);
	
	if(root) Clear(root);

	fclose(out);
	return 0;
}

void L_Rotate(Node* x, Node* &root){
    Node* y = x->right;
    x->right = y->left;
    if(y->left) (y->left)->parent = x;
    y->parent = x->parent;
    if(!x->parent) root = y;
    else if(x == (x->parent)->left) (x->parent)->left = y;
    else (x->parent)->right = y;
    y->left = x;
    x->parent = y;
}

void R_Rotate(Node* x, Node* &root){
    Node* y = x->left;
    x->left = y->right;
    if(y->right) (y->right)->parent = x;
    y->parent = x->parent;
    if(!x->parent) root = y;
    else if(x == (x->parent)->right) (x->parent)->right = y;
    else (x->parent)->left = y;
    y->right = x;
    x->parent = y;
}

void RB_Insert(double x, double y, Node* &root){
	Node* node_y = NULL, *node_x = root;
	while(node_x){
		node_y = node_x;
		if(x < node_x->x) node_x = node_x->left;
		else node_x = node_x->right;
	}
	Node* z = new Node(x, y);
	z->parent = node_y;
	if(!node_y)	root = z;	//empty tree
	else if(x < node_y->x) node_y->left = z;
	else node_y->right = z;
	if(!z->parent)	z->black = true;
	else{
		z->black = false;
		RB_InsertFixup(z, root);
	}
}

void RB_InsertFixup(Node* &z, Node* &root){
	while(!(z->parent)->black){
		if(z->parent == ((z->parent)->parent)->left){
			Node *y = ((z->parent)->parent)->right;
			if(!y){		//NULL is also black
				if(z == (z->parent)->right){
					z = z->parent;
					L_Rotate(z, root);
				}
				(z->parent)->black = true;
				((z->parent)->parent)->black = false;
				R_Rotate((z->parent)->parent, root);
			}
			else if(!y->black){
				(z->parent)->black = true;
				y->black = true;
				((z->parent)->parent)->black = false;
				z = (z->parent)->parent;
			}
			else{ 
				if(z == (z->parent)->right){
					z = z->parent;
					L_Rotate(z, root);
				}
				(z->parent)->black = true;
				((z->parent)->parent)->black = false;
				R_Rotate((z->parent)->parent, root);
			}
		}
		else{
			Node *y = ((z->parent)->parent)->left;
			if(!y){
				if(z == (z->parent)->left){
					z = z->parent;
					R_Rotate(z, root);
				}
				(z->parent)->black = true;
				((z->parent)->parent)->black = false;
				L_Rotate((z->parent)->parent, root);
			}
			else if(!y->black){
				(z->parent)->black = true;
				y->black = true;
				((z->parent)->parent)->black = false;
				z = (z->parent)->parent;
			}
			else{
				if(z == (z->parent)->left){
					z = z->parent;
					R_Rotate(z, root);
				}
				(z->parent)->black = true;
				((z->parent)->parent)->black = false;
				L_Rotate((z->parent)->parent, root);
			}
		}
		if(!z->parent) break;
	}
	root->black = true;
}

void RB_RangeSearch(double n1, double n2, vector<Node> &list, Node* root){
	if(!root) return;
	if(root->x >= n1 && root->x <= n2) {
		list.push_back(*root);
		if(root->left) RB_RangeSearch(n1, n2, list, root->left);
		if(root->right) RB_RangeSearch(n1, n2, list, root->right);
	}
	else if(root->x < n1){	//search in the greater part: right subtree
		if(root->right) RB_RangeSearch(n1, n2, list, root->right);
	}
	else 
		if(root->left) RB_RangeSearch(n1, n2, list, root->left);
}

void RB_NNSearch(double x, double y, Node* root){
	double range = 0.05;
	vector<Node> list;
	RB_RangeSearch(x - range, x + range, list, root);
	Node *root2 = NULL;
	for(size_t i = 0; i < list.size(); ++i){
		RB_Insert(list[i].y, list[i].x, root2);
	}
	list.clear();
	RB_RangeSearch(y - range, y + range, list, root2);
	for(size_t i = 0; i < list.size(); ++i){
		double dist = (x - list[i].y) * (x - list[i].y) + (y - list[i].x) * (y - list[i].x);
		if(dist < best_dist){
			best_dist = dist;
			if(!best) best = new Node(list[i].y, list[i].x);
			else{
				best->x = list[i].y;
				best->y = list[i].x;
			}
		}
	}
	if(root2) Clear(root2);
}