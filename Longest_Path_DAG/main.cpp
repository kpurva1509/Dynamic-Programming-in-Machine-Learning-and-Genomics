#include "headers.hpp"

/*
 * source is the integer label of the source node
 * sincurrent_edge is the integer label of the sincurrent_edge node
 * edges is a vector of the string representations of the edges from the input file
 *
 * return a string with the correct output format
 */

string longest_path(int source, int sink, vector<string> edges) {

    /* Create a matrix with source vertex, destination
     * vertex, and the weight of the edge between the
     * two points 
     */
    int** dag_edges = new int*[edges.size()];
    for(int i=0; i<edges.size(); i++) {
        /* each entry of the dag_edges will have src, dest, weight */
        dag_edges[i] = new int[3];
        memset(dag_edges[i],0,3*sizeof(int));
    }

    /* Iterate through the input vector of strings -> edges */
    for(int current_edge=0; current_edge<edges.size(); current_edge++) {

        std::string str = edges[current_edge];

        /*int length = edge.size();
        auto start_vertex = edge[0];
        start_vertex = int(start_vertex);
        auto destination_vertex = edge[3];
        destination_vertex = int(destination_vertex);
        int weight = edge[length-1];
        Datatype is std::basic_string */

       auto start_vertex = str.substr(0, str.find("-"));
       auto destination_vertex = str.substr(str.find(">")+1, str.find(":")-(str.find(">")+1));
       auto edge_weight = str.substr(str.find(":")+1);
       
       int i=0;
       while(i < start_vertex.length()){ dag_edges[current_edge][0] = dag_edges[current_edge][0]*10+((int)(start_vertex[i]) - 48); i++; }
       int j=0;
       while(j < destination_vertex.length()){ dag_edges[current_edge][1] = dag_edges[current_edge][1]*10+((int)(destination_vertex[j]) - 48); j++; }
       int k=0;
       while(k < edge_weight.length()){ dag_edges[current_edge][2] = dag_edges[current_edge][2]*10+((int)(edge_weight[k]) - 48); k++; }
    }

    std::vector<int> weight;
    std::vector<std::string> path;
    weight.push_back(0);
    path.push_back(std::to_string(source));

    /* Assuming node labeling as topological ordering,
     * evaluate longest path from source to sink 
     */
    for(int i=source+1; i<=sink; i++) {
        weight.push_back(MIN_VALUE);
        path.push_back(std::to_string(source));
    }

    /* Relaxing edges adjacent to a given vertex */
    for(int i=source; i<=sink; i++) {
        for(int j=0; j<edges.size(); j++) {
            /* Relax the edge only if vertex under 
             * consideration lies on one end of the
             * edge
             */
            if(dag_edges[j][1] == i && (weight[dag_edges[j][0]-source]+dag_edges[j][2]>weight[i-source])) {
                weight[i-source] = weight[dag_edges[j][0]-source]+dag_edges[j][2];
                path[i-source] = path[dag_edges[j][0]-source]+"->"+std::to_string(i);
            }
        }
    }

    /* Clear memory for assigned to dag_edges */
    for(int i=0; i<edges.size(); i++) {
        delete [] dag_edges[i];
    }
    delete [] dag_edges;

    /* Return final string */
    return std::to_string(weight[sink-source]) + "\n" + path[sink-source];
}

int main() {

    int source,sink;
    cin >> source >> sink;
    cin.ignore(256, '\n');

    vector<string> edges;

    for (string line; getline(cin, line);) { 
        // if(line == "-") break;
        if(line.empty()) break;
        edges.push_back(line); 
    }

    cout << longest_path(source, sink, edges) << endl;

    return 0;
}

/*********************************************
  Test Code : 
  vector<string> edges;
  edges.push_back("0->1:7");
  edges.push_back("0->2:4");
  edges.push_back("2->3:2");
  edges.push_back("1->4:1");
  edges.push_back("3->4:3");
  int source =0;
  int sink=4; 
**********************************************/