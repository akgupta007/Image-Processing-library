#ifndef SOFTMAX_H
#define SOFTMAX_H

void vec_to_prob(float *arr,int n, float * answer){
	float sum = 0;
	for(int i=0;i<n;i++){
		answer[i] = exp(arr[i]);
		sum += answer[i];
	}
	for(int i=0;i<n;i++){
		answer[i] /= sum;
	}
}

#endif