#include<iostream>
#include <cstdlib>
#include<vector>
#include <chrono>
#include<fstream>
#include <mkl.h>

using namespace std;
using namespace std::chrono;

int step_i = 0;

class input_thread{
    public:

        float *matrix;
        float *kernel;
        int m1;
        int n1;
        int m2;
        int n2;
        float *answer;
        input_thread(float *x,float *y,int x_m, int x_n,int k_m, int k_n ,float *ans){
            matrix = x;
            kernel = y;
            m1 = x_m;
            n1 = x_n;
            m2 = k_m;
            n2 = k_n;
            answer = ans;
        }
};
  
void* multi(void* arg) 
{
  
    input_thread *x = (input_thread *)arg;  
    int m1 = x->m1;
    int n1 = x->n1;
    int m2 = x->m2;
    int n2 = x->n2;
    int core = (step_i++)%8;
    float *kernel = x->kernel;
    float *matrix = x->matrix;
    float *answer = x->answer;
    //cout <<"core :"<<core<<endl;
    // Each thread computes 1/4th of matrix multiplication 
    for (int i = core * m1 / 8; i < (core + 1) * m1 / 8; i++){
      for(int c=0;c<n2;c++){
        for (int j = 0; j <m2 ; j++){
          answer[i*n2+c] += matrix[i*n1+j] * kernel[j*n2+c];
        }
      }
    }    
} 

float* createMatrix(int m,int n) {
    float* matrix=new float[m*n];
    for(int i=0;i<m;i++) {
      for(int j=0;j<n;j++) {
        float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3));
        matrix[i*n+j]=r;
      }
    }
    return matrix;
}


float* matMul(float* matrix1,int m1,int n1,float* matrix2,int m2,int n2,string type) {
  if(n1!=m2) {
    cerr<<"matrixes not compatible for multiplication";
    exit(1);
  }
  float* answer=createMatrix(m1,n2);

  // code to calculate matrix multiplication using iteration.
  if(type == "iteration"){
    for(int a=0;a<m1;a++){
      for(int b=0;b<n2;b++){
          float sum = 0;
          for(int c=0;c<n1;c++){
              sum += matrix1[a*n1+c]*matrix2[c*n2+b];
          }
          answer[a*n2+b] = sum;
      }
    } 
  }

  // code to calculate matrix multiplication using pthreads
  else if (type=="pthreads")
  {
    pthread_t threads[8];
   // Creating eight threads, each evaluating its own part 
   for (int i = 0; i < 8; i++) { 
       input_thread p(matrix1,matrix2,m1,n1,m2,n2,answer);
       pthread_create(&threads[i], NULL, multi, (void*)(&p)); 
   }
  } 
  
  // code to calculate matrix multiplication using mkl/cblas
  else if(type == "mkl" || type == "cblas"){
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                  m1, 1, n1, 1, matrix1, n1, matrix2, n2, 0, answer, 1);
  }
  return answer;
}


int main() {

  int noofvalues=50;
  int kernel_m=100;
  int kernel_n=100;
  int sizes[4] = {10,100,500,1000};
  int methods=1;
  float* kernel=createMatrix(kernel_m,kernel_n);
  float* timeMatrix=createMatrix(noofvalues,methods);
  for(int k=0;k<4;k++) {
    for(int i=0;i<noofvalues;i++) {
      float* matrix=createMatrix(sizes[k],kernel_m);
      for(int j=0;j<methods;j++) {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        float* result=matMul(matrix,sizes[k],kernel_m,kernel,kernel_m,kernel_n,"iteration");
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto time_span = duration_cast<microseconds>( t2 - t1 );
        timeMatrix[i*methods+j]=(float)time_span.count();
      }
      delete matrix;
    }

    //writing to file
    string concat="mkl_size"+to_string(sizes[k])+".txt";
    ofstream fout(concat);
    for(int i=0;i<noofvalues;i++) {
      for(int j=0;j<methods;j++) {
        fout<<timeMatrix[i*methods+j]<<"\t";
      }
      fout<<"\n";
    }
    fout.close();
  }
  delete kernel;
  delete timeMatrix;
  
}

