#include <iostream>
#include<bits/stdc++.h>
#include<string>
#include<cstdlib>
#include<fstream>
#include<math.h>
#include "softmax.h"
#include "activation.h"
#include "convolution.h"
#include "sampling.h"
using namespace std;



int main(int args, char **argc){
	float** matrix;
	float** kernel;
	int n,f,pad,stride;
	string filename = "";
	string operation = "";

	if(args < 2){
		cout<<"For any help type:"<<endl;
		cout<<"a.out help"<<endl;
		return 0;
	}

	else if(strcmp(argc[1],"help") == 0){
		cout<<"HELP USAGE:"<<endl;
		cout<<"./a.out matrix_file matrix_size convolution type kernel_matrix_file kernel_file kernel_size padding_size stride_length"<<endl;
		cout<<"./a.out matrix_file matrix_size relu/tanh"<<endl;
		cout<<"./a.out matrix_file matrix_size subsample_average/subsample_max sample_size stride_length"<<endl;
		cout<<"./a.out matrix_file vector_size softmax"<<endl;
		cout<<"  type: simple/toep_iter/toep_mkl/toep_blas/pthreads"<<endl;
		return 0;
	}
	
	filename = argc[1];
	n = atoi(argc[2]);
	operation = argc[3];
	ifstream file(filename.c_str());
	if(!file.is_open()){
		cout<<"unable to open file : "<<filename<<endl;
		cout<<"For any help type:"<<endl;
		cout<<"a.out help"<<endl;
		return 0;
	}
	// reading the matrix
	if(strcmp(operation.c_str(),"softmax") != 0){
		matrix = new float*[n];
		for(int i=0;i<n;i++){
			matrix[i] = new float[n];
		}
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				file>>matrix[j][i];
			}
		}
	}
	//file.close();

	if(strcmp(operation.c_str(),"convolution") == 0){
		if(args != 9){
			cout<<"For any help type:"<<endl;
			cout<<"a.out help"<<endl;
			return 0;
		}
		string type = argc[4];
		string kernel_file = argc[5];
		f = atoi(argc[6]);
		pad = atoi(argc[7]);
		stride = atoi(argc[8]);
		kernel = new float*[f];

		// open file to read kernel matrix
		ifstream kernelfile(kernel_file.c_str());
		if(!kernelfile.is_open()){
			cout<<"unable to open file : "<<kernel_file<<endl;
			cout<<"For any help type:"<<endl;
			cout<<"a.out help"<<endl;
			return 0;
		}

		// reading the kernel matrix
		for(int i=0;i<f;i++){
			kernel[i] = new float[f];
		}
		for(int i=0;i<f;i++){
			for(int j=0;j<f;j++){
				kernelfile>>kernel[j][i];
			}
		}
		//kernelfile.close();

		float** matrix_pad;
		padding(matrix,matrix_pad,pad,n);
		int n_pad = n+2*pad;
		int s = (n_pad - f)/stride+1;
		float** output;
		if (type == "simple"){
			output = conv_filter(matrix_pad, kernel, n_pad, f, stride);
		}
		else{
			if(strcmp(type.c_str(),"toep_iter") == 0){
				output = convolution_mul(matrix_pad, kernel, n_pad, f, stride);
			}
			else if(strcmp(type.c_str(),"toep_mkl") == 0){
				output = convolution_mul(matrix_pad, kernel, n_pad, f, stride,"mkl");
			}
			else if(strcmp(type.c_str(),"toep_blas") == 0){
				output = convolution_mul(matrix_pad, kernel, n_pad, f, stride,"openblas");
			}
			else{
				cout<<"Wrong type chosen"<<endl;
				return 0;
			}
			else if(type == "pthreads"){
				
			}
			
		}
		for(int i=0;i<s;i++){
			for(int j=0; j<s; j++){
				cout<<output[i][j]<<"\t";
			}
			cout<<endl;
		}
		cout<<endl;
	}

	else if(strcmp(operation.c_str(),"relu") == 0 || strcmp(operation.c_str(),"tanh") == 0){
		if(args != 4){
			cout<<"For any help type:"<<endl;
			cout<<"a.out help"<<endl;
			return 0;
		}
		float** output;
		if(strcmp(operation.c_str(),"relu") == 0){
			output = relu_filter(matrix,n);
		}
		else{
			output = tanh_filter(matrix,n);
		}
		for(int i=0;i<n;i++){
			for(int j=0; j<n; j++){
				cout<<output[i][j]<<" ";
			}
			cout<<endl;
		}
	}

	else if(strcmp(operation.c_str(),"subsample_average") == 0 || strcmp(operation.c_str(),"subsample_max") == 0){
		if(args != 6){
			cout<<"For any help type:"<<endl;
			cout<<"a.out help"<<endl;
			return 0;
		}
		int sample_size = atoi(argc[4]);
		stride = atoi(argc[5]);
		int num = (n - sample_size)/stride + 1;
		float** output = new float*[num];
		for(int i=0;i<num;i++){
			output[i] = new float[num];
		}
		if(strcmp(operation.c_str(),"subsample_average") == 0){
			subsample_average(matrix,n,sample_size,stride,output);
		}
		else{
			subsample_max(matrix,n,sample_size,stride,output);
		}
		
		for(int i=0;i<num;i++){
			for(int j=0;j<num;j++){
				cout << output[i][j]<<" ";
			}
			cout<<endl;
		}
		return 0;

	}

	else if(strcmp(operation.c_str(),"softmax") == 0){
		if(args != 4){
			cout<<"For any help type:"<<endl;
			cout<<"a.out help"<<endl;
			return 0;
		}
		float *answer = new float[n]();
		float* vec = new float[n];
		for(int i=0;i<n;i++){
			file>>vec[i];
		}		
		vec_to_prob(vec,n,answer);
		for(int i=0;i<n;i++){
			cout << answer[i] <<" ";
		}
		cout <<endl;
		return 0;
	}
	else{
		cout<<"For any help type:"<<endl;
		cout<<"a.out help"<<endl;
		return 0;
	}

}
