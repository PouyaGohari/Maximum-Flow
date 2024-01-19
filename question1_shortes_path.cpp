#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits.h>

// priority queue in c++ how to add and decrease and extract min?
// How to make a set in c++?

using namespace std;

struct dist_parrent{
    vector<int> dist;
    vector<int> parrent;
}typedef dist_parr;

class Graph{
    public:
    void bipartite_matching();
    private:
    void getting_input();
    void dikestra(int source);
    int number_of_nodes;
    vector<vector<int>> adjacency_matrix;
    vector<vector<int>> costs;
    vector<vector<int>> capacity;
    dist_parr shortest_path;
    int max_flow;
    int sum_of_cost;
};

void Graph :: getting_input(){
    cin >> number_of_nodes;
    this -> adjacency_matrix.assign(2*number_of_nodes+2, vector<int>(2*number_of_nodes + 2, 0));
    this -> capacity.assign(2*number_of_nodes+2, vector<int>(2*number_of_nodes + 2, 0));
    this -> costs.assign(2*number_of_nodes+2, vector<int>(2*number_of_nodes + 2, 0));
    for(int i = 1; i <= number_of_nodes; i++){
        adjacency_matrix[0][i] = 1;
        adjacency_matrix[i][0] = 1;
        capacity[0][i] = 1;
        adjacency_matrix[i+number_of_nodes][2*number_of_nodes+1] = 1;
        adjacency_matrix[2*number_of_nodes+1][i+number_of_nodes] = 1;
        capacity[i+number_of_nodes][2*number_of_nodes+1] = 1;
    }
    
    for(int i = 1; i <= number_of_nodes; i++){
        for(int j = number_of_nodes+1; j <= 2*number_of_nodes; j++){
            cin >> costs[i][j];
            costs[j][i]= - costs[i][j];
            adjacency_matrix[i][j] = 1;
            adjacency_matrix[j][i] = 1;
            capacity[i][j] = 1;
        }
    }
}

void Graph :: dikestra(int source){
    priority_queue<pair<int,int> , vector<pair<int,int>>, greater<pair<int,int>>> min_heap;
    this -> shortest_path.dist.assign(2*number_of_nodes+2, INT_MAX);
    this -> shortest_path.parrent.assign(2*number_of_nodes + 2, 0);
    shortest_path.dist[source] = 0;
    min_heap.push(make_pair(shortest_path.dist[source], 0));
    while(!min_heap.empty()){
        pair<int,int> u_pair = min_heap.top();
        min_heap.pop();
        for(int i = 0; i < 2 * number_of_nodes + 2; i++){
            if(adjacency_matrix[u_pair.second][i] == 1){
                if(capacity[u_pair.second][i] > 0 && shortest_path.dist[i] > shortest_path.dist[u_pair.second] + costs[u_pair.second][i]){
                    shortest_path.dist[i] = shortest_path.dist[u_pair.second] + costs[u_pair.second][i];
                    shortest_path.parrent[i] = u_pair.second;
                    min_heap.push(make_pair(shortest_path.dist[i], i));
                }
            }
        }
    }
}

void Graph :: bipartite_matching(){
    getting_input();
    max_flow = 0;
    sum_of_cost = 0;
    while(true){
        dikestra(0);
        if(shortest_path.dist[2*number_of_nodes + 1] == INT_MAX) break;
        int buttleneck = INT_MAX;
        int current_node = 2*number_of_nodes+1;
        while(current_node != 0){
            buttleneck = min(buttleneck, capacity[shortest_path.parrent[current_node]][current_node]);
            current_node = shortest_path.parrent[current_node];
        }
        max_flow += buttleneck;
        sum_of_cost += buttleneck*shortest_path.dist[2*number_of_nodes+1];
        current_node = 2*number_of_nodes + 1;
        while(current_node != 0){
            capacity[shortest_path.parrent[current_node]][current_node] -= buttleneck;
            capacity[current_node][shortest_path.parrent[current_node]] += buttleneck;
            current_node = shortest_path.parrent[current_node];
        }
    }
    cout << sum_of_cost << endl;
}

int main(){
    Graph* my_graph = new Graph;
    my_graph->bipartite_matching();
    return 0;
}