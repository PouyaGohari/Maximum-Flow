#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits.h>
#include <set>

// priority queue in c++ how to add and decrease and extract min?
// How to make a set in c++?

using namespace std;
class Graph{
    public:
    vector<int> dikestra(int source);
    void change_edge(int first, int second, int weight);
    void getting_input();
    vector<vector<int>> get_adj(){ return this -> adjacency_matrix; };
    private:
    int number_of_nodes;
    vector<vector<int>> adjacency_matrix;
    vector<int> calculations;
    vector<int> dist;
};

void Graph :: getting_input(){
    int real_number_nodes;
    cin >> real_number_nodes;
    this -> number_of_nodes = 2*real_number_nodes + 2;
    this -> adjacency_matrix.assign(this->number_of_nodes, vector<int>(this->number_of_nodes, 0));
    this -> calculations.assign(this->number_of_nodes ,0);
    for(int i = 1; i <= real_number_nodes; i++){
        change_edge(0, i, 1);
        change_edge(i + real_number_nodes , this->number_of_nodes-1, 1);
    }
    for(int i = 1; i <= real_number_nodes; i++){
        for(int j = real_number_nodes + 1; j <= 2*real_number_nodes; j++){
            int weight;
            cin >> weight;
            change_edge(i, j, weight);
        }
    }
}

void Graph :: change_edge(int first, int second, int weight){
    this -> adjacency_matrix[first][second] = weight;
}

vector<int> Graph :: dikestra(int source){
    set<int> my_set = {};
    priority_queue<pair<int,int> , vector<pair<int,int>>, greater<pair<int,int>>> min_heap;
    this -> dist.assign(this -> number_of_nodes, INT_MAX);
    vector<int> parrent(this -> number_of_nodes, 0);
    dist[source] = 0;
    min_heap.push(make_pair(dist[source], 0));
    while(!min_heap.empty()){
        pair<int,int> u_pair = min_heap.top();
        my_set.insert(u_pair.second);
        min_heap.pop();
        for(int i = 0; i < this -> number_of_nodes; i++){
            if(this -> adjacency_matrix[u_pair.second][i] != 0){
                if(dist[i] > dist[u_pair.second] + this -> adjacency_matrix[u_pair.second][i]){
                    dist[i] = dist[u_pair.second] + this -> adjacency_matrix[u_pair.second][i];
                    parrent[i] = u_pair.second;
                    min_heap.push(make_pair(dist[i], i));
                }
            }
        }

    }
    return dist;
}

int main(){
    return 0;
}