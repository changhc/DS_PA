#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

int main(){
	int arr[11] = {1000, 10000, 20000, 40000, 80000, 100000, 200000, 400000, 800000, 1000000, 10000000};
	for(int i = 0; i < 11; ++i){
		stringstream ss;
		string s;
		ss << arr[i];
		FILE* file = fopen(ss.str().c_str(), "w");
		if(!file) exit(-1);
		srand(time(NULL));
		for(int j = 0; j < arr[i]; ++j){
			double x = (rand()%100000001)/100000000.0;
			double y = (rand()%100000001)/100000000.0;
			fprintf(file, "%.10lf\t%.10lf\n", x, y);
		}
		fclose(file);
	}
	return 0;
}