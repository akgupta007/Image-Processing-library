#ifndef ACTIVATION_H
#define ACTIVATION_H

float relu(float x){
	if(x>0){
		return x;
	}
	return 0;
}

float** relu_filter(float** matrix, int n){
	float** output = new float*[n];
	for(int i=0;i<n;i++){
		output[i] = new float[n];
		for(int j=0;j<n;j++){
			output[i][j] = relu(matrix[i][j]);
		}
	}
	return output;
}

float** tanh_filter(float** matrix, int n){
	float** output = new float*[n];
	for(int i=0;i<n;i++){
		output[i] = new float[n];
		for(int j=0;j<n;j++){
			output[i][j] = tanh(matrix[i][j]);
		}
	}
	return output;
}

#endif