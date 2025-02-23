
//#pragma once
#include <SDL2/SDL.h>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int NEURON_WIDTH = 20;
const int NEURON_HEIGHT = 20;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void init();
//void data_init();
//void data_update();
void close();
std::string read_file(std::string path);
std::vector<std::string> break_page_to_networks(std::string page) ;
std::vector<float> lookup_fitments(std::vector<std::string> networks);
std::string find_fittest(std::vector<std::string> networks, std::vector<float> fitments);
std::vector<std::string> break_network(std::string network);
std::vector<std::string> break_to_layers(std::vector<std::string> BBR);
std::vector<std::string> get_layer_id(std::vector<std::string> layers);
std::vector<std::string> sort_layers(std::vector<std::string> layers, std::vector<std::string> layer_id);
std::vector<std::string> break_to_neuron_strings(std::vector<std::string> layers);
std::vector<std::vector<std::string>> get_neuron_layers(std::vector<std::string> layers);
std::vector<std::string> get_neuron_id(std::vector<std::string> neuron_strings);
std::vector<std::string> get_neuron_bias(std::vector<std::string> neuron_strings);
std::vector<std::vector<std::string>> assemble_neurons(std::vector<std::string> neuron_id, std::vector<std::string> neuron_bias,std::vector<std::vector<std::string>> neuron_layers);
//std::vector<std::string> break_to_edges(vector<std::string> BBR, std::vector<std::vector<std::string>> neurons );    
void draw_clear();
void draw_margin();
void draw_nodes();
void draw_edges();
void draw_nodes(std::vector<std::vector<std::string>> neurons);


int main(int argc, char *argv[])
{
    std::string page = read_file("drawInputTest");
    std::vector<std::string> networks = break_page_to_networks(page);
    std::vector<float> fitments = lookup_fitments(networks);
    std::string network = find_fittest(networks, fitments);
    std::vector<std::string> BBR = break_network(network);
    std::vector<std::string> layers = break_to_layers(BBR);
    std::vector<std::string> layer_id = get_layer_id(layers);
    std::vector<std::string> sorted_layers = sort_layers(layers,layer_id);
    std::vector<std::string> neuron_strings = break_to_neuron_strings(sorted_layers);
    std::vector<std::string> neuron_id = get_neuron_id(neuron_strings);
    std::vector<std::string> neuron_bias = get_neuron_bias(neuron_strings);
    std::vector<std::vector<std::string>> neuron_layers = get_neuron_layers( layers);
    std::vector<std::vector<std::string>> neurons = assemble_neurons(neuron_id,neuron_bias,neuron_layers);
    //data_update();
    //data_init();
    init();
    bool quit = false;
    SDL_Event event;
    while(!quit){
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                    quit = true;
            }
        }
    SDL_UpdateWindowSurface(window);
    draw_clear();
    draw_margin();
    //data_update();
    //draw_edges();
    draw_nodes(neurons);
//draw numbers (must be on top of nodes

    
    SDL_RenderPresent(renderer);
    }
    close(); 
  return 0;  
}

void init() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }
    else{
        window = SDL_CreateWindow("NEAT",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == NULL) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            exit(1);
        }
        else {
             renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
                        if( renderer == NULL )
                        {
                                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                                exit(1);
                        }
                        else
                        {
                                //Initialize renderer color
                                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF );
                        }
        }

    }
}

/*
int data_init() {
    
std::vector<std::string> read_file(std::string path);
std::string find_fittest_network(vector<std::string> networks);
std::vector<std::string> break_network(std::string network);
std::vector<std::string> break_to_layers(vector<std::string> BBR);
std::vector<std::vector<std::string>> break_to_neurons(vector<std::string> layers);
std::vector<std::string> break_to_edges(vector<std::string> BBR);
return 0;
}*/

/*
int data_update(std::string path) {
    std::string page = read_file("drawInputTest");
    std::vector<std::string> networks = break_page_to_networks(page);
    std::vector<float> fitments = lookup_fitments(networks);
    std::string network = find_fittest(networks, fitments);
    std::vector<std::string> BBR = break_network(network);
    std::vector<std::string> layers = break_to_layers(BBR);
    std::vector<int> layer_id = get_layer_id(layers);
    std::vector<std::string> sorted_layers = sort_layers(layers,layer_id);
    std::vector<std::string> neuron_strings = break_to_neuron_strings(sorted_layers);
    std::vector<std::string> neuron_id = get_neuron_id(std::vector<std::string> neuron_strings);
    std::vector<std::string> neuron_bias = get_neuron_bias(std::vector<std::string> neuron_strings);
    std::vector<std::vector<std::string>> neuron_layers = get_neuron_layers(vector<std::string> layers)
    std::vector<std::vector<std::string>> neurons = assemble_neurons(neuron_id,neuron_bias,neuron_layers)
    
    std::cout << network << std::endl;

    
return 0;
}
*/
std::string read_file(std::string path) {
    std::fstream file;
    std::string line;
    std::string page;
    file.open(path);
    if(file.is_open()){
        while (std::getline(file,line)){
             page += line;
        }
    }    
    else{
        std::cout << "could not open file" << std::endl;
        exit(1);
    }
    return page;
}
        
std::vector<std::string> break_page_to_networks(std::string page) {
    std::vector<std::string> networks;
    std::string temp_str;
    int counter_square_bracket = 0;
    int counter_curly_bracket = 0;
    //bool flag_square_bracket = false;
    bool flag_curly_bracket = false;
    for(int i = 0; i < page.size(); i++){
            if(page[i] == '[') {
                counter_square_bracket += 1;
            }
            if(page[i] == ']') {
                counter_square_bracket -= 1;
            }
            if(page[i] == '{') {
                counter_curly_bracket += 1;
                flag_curly_bracket = true;
            }
            if(page[i] == '}') {
                counter_curly_bracket -= 1;
            }
            if(counter_square_bracket > 0 && counter_curly_bracket > 0) {
                 temp_str += page[i];
            }
            if(counter_curly_bracket == 0 && flag_curly_bracket) {
                flag_curly_bracket = false;
                networks.push_back(temp_str);
                temp_str.clear();
            }
    }
    return networks;
}

std::vector<float> lookup_fitments(std::vector<std::string> networks) {        
        std::vector<float> fitments;
        int comma_flag = false;
        int colon_counter = 0;
        std::string temp_str2;
        for(int j = 0; j < networks.size(); j++){
            for(int k = 0; k<networks[j].size(); k++){
                if(networks[j][k] == ',') {
                        comma_flag = true;
                }
                if(networks[j][k] == ':') {
                        colon_counter += 1;
                }
                if(comma_flag && colon_counter > 1) {
                    if(networks[j][k] == ',') {
                        std::cout << temp_str2 << 1 << std::endl;
                        ///tohle hází prázdnej string... jahoda? nemyslím si....
                        float value = std::stof(temp_str2);
                        fitments.push_back(value);
                        temp_str2.clear();
                        comma_flag = false;
                        colon_counter= 0;
                        break;
                    }
                    else if(networks[j][k] == ' '||networks[j][k] == ':') {
                        continue;
                    }
                    else {
                        temp_str2 += networks[j][k];
                    }
                }
            }
        }
        return fitments;
}

std::string find_fittest(std::vector<std::string> networks, std::vector<float> fitments) {
        std::string network;
        int pos = 0;
        float max = -99999.9;
        
        for(int l = 0; l < fitments.size(); l++) {
            if(fitments[l] >= max) {
                //max = fitments[l];
                pos = l;
                
            }
        }
        network = networks[pos];
        return network;
}

std::vector<std::string> break_network(std::string network) {
    std::vector<std::string> BBR;
    std::string temp_str;
    int counter_square_bracket = 0;
    bool flag_square_bracket = false;
    for(int i = 0; i < network.size(); i++){
            if(network[i] == '[') {
                counter_square_bracket += 1;
                bool flag_square_bracket = true;
            }
            if(network[i] == ']') {
                counter_square_bracket -= 1;
            }

            if(counter_square_bracket > 0) {
                temp_str += network[i];
            }
            if(counter_square_bracket == 0 && flag_square_bracket) {
                flag_square_bracket = false;
                BBR.push_back(temp_str);
                temp_str.clear();
            }
    }
    return BBR;
}
std::vector<std::string> break_to_layers(std::vector<std::string> BBR) {
    std::vector<std::string> layers;
    std::string temp_str;
    int counter_curly_bracket = 0;
    bool flag_curly_bracket = false;
    for(int i = 0; i < BBR.size(); i++){
        for(int j = 0; j < BBR[i].size(); j++){
                if(BBR[i][j] == '{') {
                    counter_curly_bracket += 1;
                    bool flag_curly_bracket = true;
                }
                if(BBR[i][j] == '}') {
                    counter_curly_bracket -= 1;
                }

                if(counter_curly_bracket > 0) {
                    temp_str += BBR[i][j];
                }
                if(counter_curly_bracket == 0 && flag_curly_bracket) {
                    flag_curly_bracket = false;
                    layers.push_back(temp_str);
                    temp_str.clear();
                }
        }
    }
    return layers;
}
    
std::vector<std::string> get_layer_id(std::vector<std::string> layers) {
    std::vector<std::string> layer_id;
    std::string temp_str2;
    bool colon_flag = false;
    for(int j = 0; j < layers.size(); j++) {
        for(int k = 0; k <layers[j].size();k++){
            if(layers[j][k] == ':') {
                        colon_flag = true;
            }
            if(colon_flag) {
                if(layers[j][k] == ',') {
                    layer_id.push_back(temp_str2);
                    temp_str2.clear();
                    colon_flag = false;
                    break;
                }
                else if(layers[j][k] == ' ') {
                    continue;
                }
                else {
                    temp_str2 += layers[j][k];
                }
            }
        }
    }
    return layer_id;
}

std::vector<std::string> sort_layers(std::vector<std::string> layers, std::vector<std::string> layer_id) {
    std::vector<std::string> sorted_layers;
    int len = layer_id.size();
    bool swapped;
    for(int i = 0; i < len - 1; i++) {
        swapped = false;
        for(int j = 0; j < len -i -1; j++) {
            if (std::stoi(layer_id[j]) > std::stoi(layer_id[j+1])) {
            std::iter_swap(layer_id.begin() + i, layer_id.begin() + i + 1);
            std::iter_swap(layers.begin() + i, layers.begin() + i + 1);
            swapped = true;
        }
    }
    if(!swapped){
        break;
    }
    }
    
    
    return sorted_layers;
}

std::vector<std::string> break_to_neuron_strings(std::vector<std::string> layers) {
    std::vector<std::string> neuron_strings;
    std::string temp_str;
    int counter_curly_bracket = 0;
    bool flag_curly_bracket = false;
    for(int i = 0; i < layers.size(); i++){
        for(int j = 0; j < layers[i].size(); j++){
                if(layers[i][j] == '{') {
                    counter_curly_bracket += 1;
                    bool flag_curly_bracket = true;
                }
                if(layers[i][j] == '}') {
                    counter_curly_bracket -= 1;
                }

                if(counter_curly_bracket > 0) {
                    temp_str += layers[i][j];
                }
                if(counter_curly_bracket == 0 && flag_curly_bracket) {
                    flag_curly_bracket = false;
                    layers.push_back(temp_str);
                    temp_str.clear();
                }
        }
    }
    return neuron_strings;
}

std::vector<std::vector<std::string>> get_neuron_layers(std::vector<std::string> layers) {
    std::vector<std::vector<std::string>> neuron_layers;
    for(int i = 0; i < layers.size(); i++){
        //std::vector<std::string> neuron_strings = break_to_neuron_strings(layers[i]);
        std::vector<std::string> neuron_id = get_neuron_id(layers);
        neuron_layers.push_back(neuron_id);
        
    }
    
    return neuron_layers;
}

std::vector<std::string> get_neuron_id(std::vector<std::string> neuron_strings) {
    std::vector<std::string> neuron_id;
    bool colon_flag = false;
    std::string temp_str2;
    for(int j = 0; j < neuron_strings.size(); j++) {
        for(int k = 0; k <neuron_strings[j].size();k++){
            if(neuron_strings[j][k] == ':') {
                        colon_flag = true;
            }
            if(colon_flag) {
                if(neuron_strings[j][k] == ',') {
                    neuron_id.push_back(temp_str2);
                    temp_str2.clear();
                    colon_flag = false;
                    break;
                }
                else if(neuron_strings[j][k] == ' ') {
                    continue;
                }
                else {
                    temp_str2 += neuron_strings[j][k];
                }
            }
        }
    }
    return neuron_id;
}

std::vector<std::string> get_neuron_bias(std::vector<std::string> neuron_strings) {
    std::vector<std::string> neuron_bias;
    int colon_counter = 0;
    std::string temp_str2;
    for(int j = 0; j < neuron_strings.size(); j++) {
        for(int k = 0; k <neuron_strings[j].size();k++){
            if(neuron_strings[j][k] == ':') {
                        colon_counter += 1;
            }
            if(colon_counter > 1) {
                if(neuron_strings[j][k] == ',') {
                    neuron_bias.push_back(temp_str2);
                    temp_str2.clear();
                    colon_counter = 0;
                    break;
                }
                else if(neuron_strings[j][k] == ' ') {
                    continue;
                }
                else {
                    temp_str2 += neuron_strings[j][k];
                }
            }
        }
    }
    return neuron_bias;
}

//assemble neurons
std::vector<std::vector<std::string>> assemble_neurons(std::vector<std::string> neuron_id, std::vector<std::string> neuron_bias , std::vector<std::vector<std::string>> neuron_layers) {
    std::vector<std::vector<std::string>> neurons;
    int max_horizontal = neuron_layers.size();
    int max_vertical = 0;
    for(int l = 0; l < neuron_layers.size(); l++) {
        int calc = 0;
        for(int m = 0; m < neuron_layers[l].size(); m++) {
            calc += 1;
            if(calc > max_vertical) {
                max_vertical = calc;
            }
        }
    }
    float max_bias = -99999.9;
    float min_bias = 99999.9; 
    for(int n = 0; n < neuron_bias.size(); n++){
        std::cout << neuron_bias[n] << 2 << std::endl;
        if(std::stof(neuron_bias[n]) > max_bias){
            max_bias = std::stof(neuron_bias[n]);
        }
        if(std::stof(neuron_bias[n]) < min_bias){
            min_bias = std::stof(neuron_bias[n]);
        }
    }
    float bias_range = max_bias - min_bias;
    float bias_divider = bias_range /255;
        
    for(int i = 0; i < neuron_id.size(); i++) {
        std::vector<std::string> neuron;
        //ID,bias,layer,color r, color g, color b, left, top, right, bottom
        neuron.push_back(neuron_id[i]);
        neuron.push_back(neuron_bias[i]);
        //determine layer
        int layer;
        int order;
        for(int j = 0; j < neuron_layers.size(); j++){
                for(int k =0; k < neuron_layers[j].size(); k++){
                    if(neuron_layers[j][k] == neuron_id[i]){
                        neuron.push_back(std::to_string(j));
                        layer = j;
                        order = k;
                    }
                }
        }
        //determine color
        std::cout << neuron_bias[i] << 3 << std::endl;
        float n_bias = std::stof(neuron_bias[i]);
        int clr = floor(n_bias/bias_divider);
        int clr2 = floor(255-n_bias/bias_divider);
        std::string color = std::to_string(clr);
        std::string color2 = std::to_string(clr2);
        neuron.push_back(color);
        neuron.push_back(std::to_string(255));
        neuron.push_back(color2);
        
        std::string left = std::to_string(SCREEN_WIDTH*layer/max_horizontal - NEURON_WIDTH/2);
        std::string top = std::to_string(SCREEN_WIDTH*order/max_vertical - NEURON_HEIGHT/2);
        std::string right = std::to_string(NEURON_WIDTH);
        std::string bottom = std::to_string(NEURON_HEIGHT);
        //determine position
        neuron.push_back(left);
        neuron.push_back(top);
        neuron.push_back(right);
        neuron.push_back(bottom);
        
        
        neurons.push_back(neuron);
    }
    return neurons;
}
//break to edges
std::vector<std::string> break_to_edges(std::vector<std::string> BBR, std::vector<std::vector<std::string>> neurons ) {
    std::vector<std::string> edges;
    
    //break neurons to neurons
    
    //determine color range + add colors
    
    return edges;
}





void draw_clear() {
    
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear(renderer);
}

void draw_margin() {
    SDL_Rect outlineRect = { 10, 10, SCREEN_WIDTH-20, SCREEN_HEIGHT-20 };
    SDL_SetRenderDrawColor(renderer, 0x10, 0x10, 0x10, 0xFF );
    SDL_RenderDrawRect(renderer, &outlineRect );
}

void draw_nodes(std::vector<std::vector<std::string>> neurons) {
    SDL_Rect arr [neurons.size()];
    
    for(int i = 0; i < neurons.size(); i++) {
        arr[i] = { std::stoi(neurons[i][6]) , std::stoi(neurons[i][7]), std::stoi(neurons[i][8]) , std::stoi(neurons[i][9])};
        
    }
    
    for(int j = 0; j < neurons.size(); j++) {
        SDL_SetRenderDrawColor(renderer, std::stoi(neurons[j][3]) , std::stoi(neurons[j][4]) , std::stoi(neurons[j][5]), 0xFF );
        SDL_RenderFillRect(renderer, &arr[j]);
    }
    
}
/*    
void draw_edges() {
    /*
     //draw edges (must be behind nodes)
    //enabled
    SDL_SetRenderDrawColor(renderer, 0xDD, 0xDD, 0x00, 0xFF );
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH*1/3+10, SCREEN_HEIGHT*2/3, SCREEN_WIDTH*1/2-10, SCREEN_HEIGHT*1/3 );
    
    //disabled
    SDL_SetRenderDrawColor(renderer, 0x50, 0x50, 0x00, 0xFF );
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH*1/2+10, SCREEN_HEIGHT*1/3, SCREEN_WIDTH*2/3-10, SCREEN_HEIGHT*2/3 );
    */
    
//}



void close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;
    SDL_Quit();
}

