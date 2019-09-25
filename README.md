## Image-Processing-library

Machine Learning and other image processing algorithms involves convolution on input images which is very time consuming task. 
Convolution operation can be optimized in many ways :
  + Iteratively computing convolution.
  + Using Multi-threading
  + Converting convolution into **toeplitz form** and then do matrix multiplication using optimized libraries like intel MKL, or BLAS.
  
We have done time analysis of all different algorithms discussed above, highlighting the clear timing distinctions and Implemented LeNet architecure from scratch using best approach.
