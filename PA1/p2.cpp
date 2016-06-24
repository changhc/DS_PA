#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdio>
#include "main.h"
using namespace std;

Node* best;
double best_dist = 1.0/0.0;

int main(){
	int arr[11] = {1000, 10000, 20000, 40000, 80000, 100000, 200000, 400000, 800000, 1000000, 10000000};
	FILE* out = fopen("P2_result.txt", "w");
	for(int i = 0; i < 11; ++i){
		Node* root = NULL;		// root.depth == 0
		stringstream ss;
		string s;
		ss << arr[i];
		FILE* file = fopen(ss.str().c_str(), "r");
		if(!file) return -1;
		double x, y;
		clock_t timer = clock();
		
		while(!feof(file)){
			fscanf(file, "%lf", &x);
			fscanf(file, "%lf", &y);
			//cout<<x<<" "<<y<<endl;
			Insert(x, y, root);
		}
		fprintf(out, "%d Insertions: %f sec\n", arr[i], double(clock() - timer)/CLOCKS_PER_SEC);
		if(root) Clear(root);
	}	
	fclose(out);
	return 0;
}