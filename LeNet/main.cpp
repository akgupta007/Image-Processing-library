#include <iostream>
#include<bits/stdc++.h>
#include<string>
#include "LeNet.h"
#include<cstdlib>
#include<fstream>
using namespace std;

float**** mat_init(int n, int c_in, int c_out){
	float**** matrix = new float***[c_out];
	for(int i=0;i<c_out;i++){
		matrix[i] = new float**[c_in];
		for(int j=0;j<c_in;j++){
			matrix[i][j] = new float*[n];
			for(int k=0;k<n;k++){
				matrix[i][j][k] = new float[n];
			}
		}
	}
	return matrix;
}

float**** read_file(string filename, int n, int c_in, int c_out, float* b = NULL){
	ifstream file(filename.c_str());
	if(!file.is_open()){
		cout<<"unable to open file : "<<filename<<endl;
		cout<<"For any help type:"<<endl;
		cout<<"a.out help"<<endl;
		return 0;
	}
	// reading the matrix
	float**** matrix = mat_init(n, c_in, c_out);

	for(int i=0;i<c_out;i++){
		for(int j=0;j<c_in;j++){
			for(int k=0;k<n;k++){
				for(int l=0;l<n;l++){
					file>>matrix[i][j][k][l];
					if(b == NULL){
						matrix[i][j][k][l] = 1-matrix[i][j][k][l]/255.0;
					}
				}
			}
		}
	}
	if (b!= NULL){
		for(int i = 0; i<c_out; i++){
			file>>b[i];
		}
	}
	file.close();
	return matrix;
}

void print_mat(float**** matrix, int n, int c_in, int c_out){
	for(int i=0;i<c_out;i++){
		for(int j=0;j<c_in;j++){
			for(int k=0;k<n;k++){
				for(int l=0;l<n;l++){
					cout<<matrix[i][j][k][l]<<"\t";
				}
				cout<<endl;
			}
			cout<<endl;
		}
		cout<<endl;
		cout<<endl;
	}
}

int main(int args, char **argc){
	float**** image;
	float**** ker_1;
	float**** ker_2;
	float**** fc_1;
	float**** fc_2;

	if(args < 2){
		cout<<"For any help type:"<<endl;
		cout<<"a.out help"<<endl;
		return 0;
	}

	else if(strcmp(argc[1],"help") == 0){
		cout<<"HELP USAGE:"<<endl;
		cout<<"./a.out image.png conv_1.txt conv_2.txt fc_1.txt fc_2.txt"<<endl;
		return 0;
	}


	float* b1 = new float[20];
	float* b2 = new float[50];
	float* b3 = new float[500];
	float* b4 = new float[10];

	image = read_file(argc[1], 28, 1, 1);
	ker_1 = read_file(argc[2], 5, 1, 20, b1);
	ker_2 = read_file(argc[3], 5, 20, 50, b2);
	fc_1 = read_file(argc[4], 4, 50, 500, b3);
	fc_2 = read_file(argc[5], 1, 500, 10, b4);

	//print_mat(fc_1, 4, 50, 500);
	int output = LeNet(image, ker_1, b1, ker_2, b2, fc_1, b3, fc_2, b4);
	//cout<< output;

}
