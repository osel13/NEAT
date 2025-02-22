#ifndef __TEST__
    #pragma once
#endif
#include <iostream>
#include <vector>

//make XOR vector
std::vector<float> make_XOR_vector(std::vector<float> A, std::vector<float> B) {
    // cassert
    if (A.size() != B.size()){
        std::cout << "uneven input length" << std::endl;
        exit(1);
    }
    else {
        std::vector<float> C;
        for(int i = 0; i < A.size(); i++){
            if((A[i] == 1 && B[i] == 0)||(A[i] == 0 && B[i] == 1)){
                C.push_back(1);
                // A[i] != B[i]
            }
            else {
                C.push_back(0);
            }
        }
        return C;
    }
    
}

/*
int main () {
    std::vector<float> A = {0,0,1,1};
    std::vector<float> B = {0,1,0,1};
    std::vector<float> C = make_XOR_vector(A,B);
    
    for(int i = 0; i < C.size(); i++){
        std::cout << A[i] << " " << B[i] << "|" << C[i] << std::endl;
    }
    
    return 0;
    
}
*/
