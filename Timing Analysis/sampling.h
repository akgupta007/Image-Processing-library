#ifndef SAMPLING_H
#define SAMPLING_H
void subsample_max(float **arr, int matrix_size,int sample_size,int stride,float **answer){
	int i = 0;
	int p = 0;
	while(i+sample_size-1<matrix_size){
		int j = 0;
		int q = 0;
		while(j+sample_size-1<matrix_size){
			float max = -100000;
			for(int t=i;t<i+sample_size;t++){
				for(int s=j;s<j+sample_size;s++){
					if (arr[t][s]>max) max= arr[t][s];
				}
			}
			answer[p][q] = max;
			j += stride;
			q++;
		}
		i += stride;
		p++;
	}
}

void subsample_average(float **arr, int matrix_size,int sample_size,int stride,float **answer){
	int i = 0;
	int p = 0;
	while(i+sample_size-1<matrix_size){
		int j = 0;
		int q = 0;
		while(j+sample_size-1<matrix_size){
			float sum = 0;
			for(int t=i;t<i+sample_size;t++){
				for(int s=j;s<j+sample_size;s++){
					sum += arr[t][s];
				}
			}
			sum /= sample_size;
			sum /= sample_size;
			answer[p][q] = sum;
			j += stride;
			q++;
		}
		i += stride;
		p++;
	}
}

#endif