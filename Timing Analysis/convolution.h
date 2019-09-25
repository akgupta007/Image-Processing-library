#ifndef CONCOLUTION_H
#define	CONVOLUTION_H
#include<iostream>
#include<string>
#include<cblas.h>
using namespace std;


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


float conv(float** mat1,int row, int col, float** mat2, int n)
{
	float sum =0 ;
	for(int i=0;i<n; i++){
		for(int j=0; j<n; j++){
			sum += mat1[row+i][col+j]*mat2[i][j];
		}
	}
	return sum;
}

float** conv_filter(float** matrix, float** kernel, int n, int f, int stride){
	int size = ((n-f)/stride)+1;
	float** output = new float*[size];
	for(int i=0;i<size;i++){
		output[i] = new float[size];
	}
	for (int i= 0; i<size; i += 1){
		for(int j= 0; j<size; j += 1){
			output[i][j] = conv(matrix,i*stride,j*stride,kernel,f);
		}
	}
	return output;
}

void padding(float** ref_mat, float** &pad_mat,int pad,int n){
	int n_pad = n+2*pad;
    pad_mat = new float*[n_pad];
	for(int i=0;i<n_pad;i++){
		pad_mat[i] = new float[n_pad]();
	}
    for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			pad_mat[pad+i][pad+j] = ref_mat[i][j];
		}
	}
}

float** convolution_mul(float **matrix,float **kernel, int size, int kernel_size,int stride,string type = ""){
    int num = (size - kernel_size)/stride + 1;
    //vector kernel - row major
    float *ker = new float[kernel_size*kernel_size];
    for(int i=0;i<kernel_size;i++){
        for(int j=0;j<kernel_size;j++){
            ker[i*kernel_size+j] = kernel[i][j];          
        }
    }
    // new matrix for returning answer
    float **answer = new float*[num*num];
    for(int i=0;i<num;i++){
        answer[i] = new float[num];
    }
    //new transformed matrix
    float *arr = new float[num*num*kernel_size*kernel_size];
    int i = 0;
    int p = 0;
    int l = 0;
    while(i+kernel_size-1<size){
        int j = 0;
        int q = 0;
        while(j+kernel_size-1<size){
            int m = 0;
            for(int t=i;t<i+kernel_size;t++){
                for(int s=j;s<j+kernel_size;s++){
                    arr[l*kernel_size*kernel_size+m] = matrix[t][s];
                    m++;
                }
            }
            j += stride;
            q++;
            l++;
        }
        i += stride;
        p++;
    }
    float alpha = 1;
    float beta = 0;
    if(strcmp(type.c_str(),"") == 0){
        for(int a=0;a<num;a++){
            for(int b=0;b<num;b++){
                float sum = 0;
                for(int c=0;c<kernel_size*kernel_size;c++){
                    sum += arr[(a*num+b)*kernel_size*kernel_size+c]*ker[c];
                }
                answer[a][b] = sum;
            }
        }
    }
    else if(strcmp(type.c_str(),"mkl") == 0){
        float* C = new float[num*num];
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                num*num, 1, kernel_size*kernel_size, 1, arr, kernel_size*kernel_size, ker, 1, 0, C, 1);
        for(int a=0;a<num;a++){
            for(int b=0;b<num;b++){
                answer[a][b] = C[a*num+b];
            }
        }
    }
    else if(strcmp(type.c_str(),"openblas") == 0){
        float* C = new float[num*num];
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                num*num, 1, kernel_size*kernel_size, 1, arr, kernel_size*kernel_size, ker, 1, 0, C, 1);
        for(int a=0;a<num;a++){
            for(int b=0;b<num;b++){
                answer[a][b] = C[a*num+b];
            }
        }
    }
    else if(type == "pthreads"){
    }
    return answer;   
}

	
	
	


#endif