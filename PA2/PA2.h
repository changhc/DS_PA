#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace std;

typedef struct P{
	double x, y;
	P(double x_in, double y_in): x(x_in), y(y_in) {}
	P operator=(P p){ x = p.x; y = p.y;}
} Point;

void Sort(vector<Point> &vec){
	for(size_t i = 1; i < vec.size(); ++i){
		for(size_t j = 0; j < i; ++j){
			if(vec[j].x > vec[i].x){
				Point p = vec[i];
				for(size_t k = 0; k < vec.size(); ++k)
					printf("%lf ", vec[k].x);
				vec.insert(vec.begin() + j, p);
				vec.erase(vec.begin() + i + 1);
				for(size_t k = 0; k < vec.size(); ++k)
					printf("%lf ", vec[k].x);
				break;
			}
		}
	}
}

void LoadPoly(char* poly_name, vector<vector<Point> > &vec){
	FILE *file = fopen(poly_name, "r");
	if(!file){
		fprintf(stderr, "%s\n", "Invalid Polygon File!");
		exit(-1);
	}
	char seq[50], seq2[50];
	double x = 0, y = 0;
	bool back = false;
	int poly_count = 0;
	Point prev(0, 0);
	while(!feof(file)){
		fscanf(file, "%*[^0-9#]%[0-9.#], %[0-9.#]%*s", seq, seq2);
		if(strcmp(seq, "#") == 0){
			++poly_count;
			vector<Point> tmp;
			vec.push_back(tmp);
			prev.x = 0;
			prev.y = 0;
			back = false;
			continue;
		}
		Point p(atof(seq), atof(seq2));
		if(p.x == prev.x && p.y == prev.y) break;
		if(prev.x > p.x) back = true;
		if(prev.x == p.x && back){
			vec[poly_count].pop_back();
			vec[poly_count].push_back(p);
			vec[poly_count].push_back(prev);
			continue;
		}
		prev = p;
		vec[poly_count].push_back(p);
	}

	for(size_t i = 0; i < vec.size(); ++i)
		Sort(vec[i]);

	fclose(file);
}

bool Inside(double x, double y, vector<vector<Point> > poly){
	for(size_t i = 1; i < poly.size(); ++i){
		double x1 = poly[i][0].x, y1 = poly[i][0].y, x2 = -1, y2 = -1;
		for(size_t j = 1; j < poly[i].size(); ++j){
			if(poly[i][j].x == x1 && poly[i][j].y > y1) y2 = poly[i][j].y;
			if(poly[i][j].x == x1 && poly[i][j].y < y1) y1 = poly[i][j].y;
			if(poly[i][j].x != x1) x2 = poly[i][j].x;
			if(x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) continue;		//any coordinate not set
			if(x < x2 && x > x1 && y < y2 && y > y1) return false;
			x1 = x2; x2 = -1; y2 = -1;
		}
	}

	double x1 = poly[0][0].x, y1 = poly[0][0].y, x2 = -1, y2 = -1;
	for(size_t j = 0; j < poly[0].size(); ++j){
		if(poly[0][j].x == x1 && poly[0][j].y > y1) y2 = poly[0][j].y;
		if(poly[0][j].x == x1 && poly[0][j].y < y1) y1 = poly[0][j].y;
		if(poly[0][j].x != x1) x2 = poly[0][j].x;
		if(x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) continue;		//any coordinate not set
		if(x <= x2 && x >= x1 && y <= y2 && y >= y1) return true;
		x1 = x2; x2 = -1; y2 = -1;
	}
	return false;
}

void run(int input_num, int poly_num){
	char input_name[50] = "input", poly_name[50] = "polygon", output_name_in[50] = "inner-", output_name_out[50] = "outer-";
	input_name[5] = input_num + 48;
	poly_name[7] = poly_num + 48;
	output_name_in[6] = poly_num + 48; output_name_in[7] = '-', output_name_in[8] = input_num + 48;
	output_name_out[6] = poly_num + 48; output_name_out[7] = '-', output_name_out[8] = input_num + 48;
	strcat(input_name, ".txt");
	strcat(poly_name, ".txt");
	strcat(output_name_in, ".txt");
	strcat(output_name_out, ".txt");
	vector<vector<Point> > poly(1);
	LoadPoly(poly_name, poly);
	//for(size_t i = 0; i < poly[0].size(); ++i){
	//	printf("%lf\t%lf\n", poly[0][i].x, poly[0][i].y);
	//}
	//printf("%d, %d\n", poly.size(), Inside(0.3, 0.3, poly));
	FILE *input_file = fopen(input_name, "r"), *output_in = fopen(output_name_in, "w"), *output_out = fopen(output_name_out, "w");
	if(!input_file){
		fprintf(stderr, "%s\n", "Invalid Input File!");
		exit(-1);
	}
	while(!feof(input_file)){
		double x, y;
		fscanf(input_file, "%lf", &x);
		fscanf(input_file, "%lf", &y);
		if(Inside(x, y, poly)) fprintf(output_in, "%lf\t%lf\n", x, y); 
		else fprintf(output_out, "%lf\t%lf\n", x, y);
	}
	fclose(input_file);
	fclose(output_in);
	fclose(output_out);
	
}
