#ifndef __TEST__
    #pragma once
#endif
//generate
#include <random>
#include <cmath>

//read
#include <fstream>
#include <string>
#include <sstream>


//everything
#include <iostream>
#include <vector>

//Read input matrix/vector
std::vector<float> read_input_matrix(std::string path) {
    //file format: every member of matrix row, separated by coma, every line separated by semicolon
    std::fstream file;
    std::string line;
    std::string temp_str;
    std::vector<float> temp_vec;
    float member;
    bool row_stop = false;
    bool dot_present = false;
    int row_length;
    file.open (path);
    if (file.is_open()){
        while (std::getline(file,line)){
            for (int i = 0; i < line.size(); i++){
                if (line[i] != ',' && line[i] != ';'){
                    temp_str += line[i];
                    if(!row_stop){
                        row_length = i;
                    }
                    if(line[i] == '.'){
                        dot_present = true;
                    }
                }
                else if(line[i] == ';'){
                    row_stop = true;
                    if(!dot_present){
                        temp_str += ".0";
                    }
                    member = std::stof(temp_str);
                    temp_vec.push_back(member);
                    temp_str.clear();
                    
                }
                else{
                    if(!dot_present){
                        temp_str += ".0";
                    }
                    member = std::stof(temp_str);
                    temp_vec.push_back(member);
                    temp_str.clear();
                }
            }
            dot_present = false;
        }
       
        file.close();
        return temp_vec;
        

    }
    else {
     std::cout << "could not open file" << std::endl;
     exit(1);
    }
}
 
//Generate random vector matrix/vector
std::vector<float> generate_input_matrix(int rows, int cols, int min = 0, int max = 1) {
    int element_count = rows * cols;
    std::vector<float> temp_vec;
    for(int i = 0; i < element_count; i++){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(min, max);
        float value = distrib(gen);
        //x = round(increment * value)/increment;
        temp_vec.push_back(value);
    }
    return temp_vec;
}

/*
int main() {
    std::cout << "test read from inputTest:" << std::endl;
    std::vector<float> arr = read_input_matrix("inputTest");
    int arr_len = arr.size();
    float* arr_data = arr.data();
    for(int i = 0; i < arr_len; i++){
        std::cout << arr_data[i] << " "; 
    }
    std::cout << std::endl;
    
    std::cout << "test generate 10 elements from 0.0 to 1.0 :" << std::endl;
    std::vector<float> gen = generate_input_matrix(5,2);
    int gen_len = gen.size();
    float* gen_data = gen.data();
    for(int j = 0; j < gen_len; j++){
        std::cout << gen_data[j] << " "; 
    }
    std::cout << std::endl;
    
    return 0;
}
*/

