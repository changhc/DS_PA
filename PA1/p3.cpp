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

int main(int argc, char** argv){
	/*	part3	*/
	if(argc % 2 != 0){
		cerr << "Invalid input arguments!\n";
		exit(-1);
	}

	FILE* file = fopen(argv[1], "r");
	if(!file) exit(-1);
	char out_name[] = "P3_result_";
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
		Insert(tmpx, tmpy, root);
	}
	
	if(argc != 6){
		vector<Node> list;
		NNSearch(x, y, root);
		fprintf(out, "The nearest neighbor of (%lf, %lf) is (%lf, %lf).\n\n", x, y, best->x, best->y);
	}

	if(argc != 4){
		RangeSearch(bx1, by1, bx2, by2, list, root);
		fprintf(out, "There are %d points in the rectangle (%f, %f), (%f, %f), (%f, %f), (%f, %f): \n", list.size(), bx1, by1, bx1, by2, bx2, by1, bx2, by2);
		for(unsigned int i = 0; i < list.size(); ++i){
			fprintf(out, "(%lf, %lf)\n", list[i].x, list[i].y);
		}
	}
	
	clock_t timer = clock();
	int count = 0;
	srand(time(NULL));
	while(double(clock() - timer)/CLOCKS_PER_SEC < 1.0){
		x = double(rand()%10000001) / 10000000.0;
		y = double(rand()%10000001) / 10000000.0;
		NNSearch(x, y, root);
		++count;
	}
	fprintf(out, "\nWe can perform %d NN calculations in 1 second.\n", count);
	if(root) Clear(root);

	fclose(out);
	return 0;
}