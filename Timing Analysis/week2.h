#ifndef WEEK2_H
#define WEEK2_H

#include <bits/stdc++.h>
#include <iostream>
using namespace std;
// CPP Program to multiply two matrix using pthreads 
 
int step_i = 0;

class input_thread{
    public:

        float **matrix;
        float *kernel;
        int num;
        int kernel_size;
        float **answer;
        input_thread(float **x,float *y,int n,int k,float **ans){
            matrix = x;
            kernel = y;
            num = n;
            kernel_size = k;
            answer = ans;
        }
};
  
void* multi(void* arg) 
{
	int core = (step_i++)%4;
    input_thread *x = (input_thread *)arg;  
    int num = x->num;
    int kernel_size = x->kernel_size;
    float *kernel = x->kernel;
    float **matrix = x->matrix;
    float **answer = x->answer;
    cout <<"core :"<<core<<endl;
    // Each thread computes 1/4th of matrix multiplication 
    for (int i = core * num*num / 4; i < (core + 1) * num*num / 4; i++){
        for (int j = 0; j <kernel_size*kernel_size ; j++){
        	answer[i/num][i%num] += matrix[i][j] * kernel[j];
        }
   	}    
} 
  
// Driver Code 
float ** pthread_matrix_mul(float **arr,float *ker,int num,int kernel_size) 
{ 
    // declaring four threads 
    pthread_t threads[4]; 
    float **answer = new float*[num];
    for(int i=0;i<num;i++){
    	answer[i] = new float[num];
    }
    // Creating four threads, each evaluating its own part 
    for (int i = 0; i < 4; i++) { 
        input_thread p(arr,ker,num,kernel_size,answer);
        pthread_create(&threads[i], NULL, multi, (void*)(&p)); 
    } 
  
    // joining and waiting for all threads to complete 
    for (int i = 0; i < 4; i++)  
        pthread_join(threads[i], NULL);

    return answer;
 
} 

#endif