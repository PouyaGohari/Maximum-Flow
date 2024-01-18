#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <set>
#include <limits.h>

using namespace std;

class graph{
    public:
    void getting_input();
    void hungarian_algorithm();
    private:
    int number_of_nodes;
    vector<vector<int>> adj_matrix;
    vector<vector<int>> x;
    vector<int> u;
    vector<int> v;
    vector<int> mates;
    vector<int> p;
    vector<int> pi;
    set<int> L_set;
    vector<int> label;
    int cards;
    int path;
    void initialize_dual();
    void initialize_primal();
    void finding_path_part1();
    void finding_path_part2();
    void finding_path_part2_1(int k);
    void finding_path_part2_2(int k);
    void iteration_dual();
    void iteration_primal();
};

void graph :: getting_input(){
    cin >> this -> number_of_nodes;
    this -> adj_matrix.assign(number_of_nodes, vector<int>(number_of_nodes, 0));
    this -> x.assign(number_of_nodes, vector<int>(number_of_nodes, 0));
    for(int i = 0; i < number_of_nodes; i++){
        for(int j = 0; j < number_of_nodes; j++){
            cin >> this -> adj_matrix[i][j];
        }
    }
    this -> u.assign(number_of_nodes, 0);
    this -> v.assign(number_of_nodes, 0);
}

void graph :: initialize_dual(){
    for(int i = 0; i < number_of_nodes; i++){
        this -> u[i] = adj_matrix[i][i];
        for(int j = 0; j < number_of_nodes; j++){
            this -> u[i] = min(this -> u[i], this -> adj_matrix[i][j]);
        }
    }
    for(int j = 0; j < number_of_nodes; j++){
        this -> v[j] = this -> adj_matrix[j][j] - this -> u[j];
        for(int i = 0; i < number_of_nodes; i++){
            this -> v[j] = min(this -> v[j], this -> adj_matrix[i][j] - this -> u[i]);
        }
    }
}

void graph :: initialize_primal(){
    this -> mates.assign(2 * this -> number_of_nodes, 0);
    this -> cards = 0;
    for(int i = 0; i < number_of_nodes; i++){
        for(int j = 0; j < number_of_nodes; j++){
            if((this->adj_matrix[i][j]-this->u[i]-this->v[j] == 0) && (mates[i] == 0) && (mates[j+number_of_nodes] == 0)){
                this -> x[i][j] = 1;
                this -> cards += 1;
                mates[i] = j + 1;
                mates[j + number_of_nodes] = i + 1; 
            }
        }
    }
}

void graph :: finding_path_part1(){
    this -> label.assign(2*number_of_nodes, -1);
    this -> L_set.clear();
    this -> p.assign(number_of_nodes, INT_MAX);
    this -> pi.assign(number_of_nodes, 0);
    for(int i = 0; i < number_of_nodes; i++){
        if(this -> mates[i] == 0){
            this -> label[i] = 0;
            this -> L_set.insert(i + 1);
            for(int j = 0; j < number_of_nodes; j++){
                if(this -> label[j + number_of_nodes] == -1){
                    if(this -> adj_matrix[i][j] - this -> u[i] - this -> v[j] < this -> p[j]){
                        this -> p[j] = this -> adj_matrix[i][j] - this -> u[i] - this -> v[j];
                        this -> pi[j] = i + 1;
                    }
                }
            }
        }
    }
}

void graph :: finding_path_part2_1(int k){
    for(int j = 0; j < number_of_nodes; j++){
        if((label[j+number_of_nodes] == -1) && (this -> adj_matrix[k][j] - this -> u[k] - this -> v[j] == 0)){
            this -> label[j + number_of_nodes] = k;
            this -> L_set.insert(j + number_of_nodes + 1);
        }
    }
}

void graph :: finding_path_part2_2(int k){
    if(this -> label[mates[k]-1] == -1){
        this -> label[mates[k]-1] = k;
        this -> L_set.insert(mates[k]);
        for(int j = 0; j < number_of_nodes; j++){
            if(label[j+number_of_nodes] == -1){
                if(this -> adj_matrix[mates[k]-1][j] - this -> u[mates[k]-1] - v[j] < p[j]){
                    p[j] = adj_matrix[mates[k]-1][j] - u[mates[k]-1]-v[j];
                    pi[j] = mates[k];
                }
            }
        }
    }
}

void graph :: finding_path_part2(){
    int k = *(this -> L_set.begin()) - 1;
    this -> L_set.erase(this -> L_set.begin());
    if(k < number_of_nodes){
        finding_path_part2_1(k);
    }
    else if(mates[k] != 0){
        finding_pat  25 40 35 
  40 60 35 
  20 40 25 h_part2_2(k);
    }
    else{
        this -> path = k;
    }
}

void graph :: iteration_dual(){
    int delta = this -> p[0];
    for(int j = 0; j < number_of_nodes; j++){
        if(label[j + number_of_nodes] == -1){
            delta = min(this -> p[j], delta);
        }
    }
    for(int i = 0; i < number_of_nodes; i++){
        if(label[i] != -1){
            u[i] += delta;
        }
    }
    for(int j = 0; j < number_of_nodes; j++){
        if(label[j + number_of_nodes] != -1){
            v[j] -= delta;
        }
        else{
            p[j] -= delta;
        }
    }
    for(int j = 0; j < number_of_nodes; j++){
        if((label[j + number_of_nodes] == -1) && (p[j] == 0)){
            label[j + number_of_nodes] = pi[j]-1;
            L_set.insert(j + number_of_nodes + 1);
        }
    }
}

void graph :: iteration_primal(){
    int j = this -> path;
    while(j != 0){
        int i = label[j];
        if(j < number_of_nodes && i >= number_of_nodes){
            mates[i] = j+1;
            mates[j] = i-number_of_nodes+1;
            x[i-number_of_nodes][j] = 1;
        }
        else if(j >= number_of_nodes && i < number_of_nodes){
            mates[j] = i+1;
            mates[i] = j - number_of_nodes + 1;
            x[i][j-number_of_nodes] = 1;
        }
        cards += 1;
        j = label[i];
        if(label[i] != 0){
            if(j < number_of_nodes && i >= number_of_nodes){
                x[i-number_of_nodes][j] = 0;
            }
            else if(j >= number_of_nodes && i < number_of_nodes){
                x[i][j-number_of_nodes] = 0;
            }
            cards -= 1;
        }
    }
}

void graph :: hungarian_algorithm(){
    initialize_dual();
    initialize_primal();
    while(cards != number_of_nodes){
        finding_path_part1();
        path = INT_MAX;
        while(path == INT_MAX){
            while(path == INT_MAX && (!L_set.empty())){
                finding_path_part2();
            }
            if(path == INT_MAX){
                iteration_dual();
            }
        }
        iteration_primal();
    }
    int sum = 0;
    for(int i = 0; i < number_of_nodes; i++){
        for(int j = 0; j < number_of_nodes; j++){
            if(x[i][j] == 1){
                sum += adj_matrix[i][j];
            }
        }
    }
    cout << sum << endl;
}

int main(){
    graph my_graph;
    my_graph.getting_input();
    my_graph.hungarian_algorithm();
    return 0;
}