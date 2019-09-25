#ifndef LENET_H
#define	LENET_H

#include "convolution.h"
#include "sampling.h"
#include "softmax.h"
#include "activation.h"

void print_matrix(float*** matrix, int n, int c_in){
	for(int j=0;j<c_in;j++){
		for(int k=0;k<n;k++){
			for(int l=0;l<n;l++){
				cout<<matrix[j][k][l]<<"\t";
			}
			cout<<endl;
		}
		cout<<endl;
	}
	cout<<endl;
	cout<<endl;

}

int LeNet(float**** image, float**** ker_1, float* b1, float**** ker_2, float* b2, float**** fc_1,float* b3, float**** fc_2, float* b4){
	float*** layer1 = new float**[1];
	for (int i = 0;i<1;i++){
		layer1[i] = new float*[28];
		for(int j=0;j<28;j++){
			layer1[i][j] = new float[28];
			for(int k=0;k<28;k++){
				layer1[0][j][k] = image[0][0][j][k];
			}
		}
	}
	float*** layer2 = convolve(layer1, ker_1, b1, 28, 1, 5, 20);
	float*** layer3 = subsample_max(layer2, 24, 20, 2, 2);
	float*** layer4 = convolve(layer3, ker_2, b2, 12, 20, 5, 50);
	float*** layer5 = subsample_max(layer4, 8, 50, 2, 2);
	float*** layer6 = convolve(layer5, fc_1, b3, 4, 50, 4, 500);
	relu_filter(layer6, 1, 500);
	float*** layer7 = convolve(layer6, fc_2, b4, 1, 500, 1, 10);
	
	float arr[10];
	for (int i=0;i<10;i++){
		arr[i] = layer7[i][0][0];
	}
	sort(arr,arr+10, greater<float>());
	for(int i=0;i<5;i++){
		for(int j=0;j<10;j++){
			if(layer7[j][0][0] == arr[i]){
				cout<<j<<endl;
				break;
			}
		}
	}
	return 0;
}

#endif