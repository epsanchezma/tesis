// C++ program for implementation of Ford Fulkerson algorithm
#include <iostream>
#include <limits.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>

using namespace std;
  
/* Returns true if there is a path from source 's' to sink 't' in
  residual graph. Also fills parent[] to store the path */
bool bfs(std::vector<std::vector<int> > rGraph, int source, int sink, int *parent, int V)
{
    // Create a visited array and mark all vertices as not visited
    bool visited[V];
    memset(visited, 0, sizeof(visited));
    // TODO: change definition of visited array
    // bool *visited = calloc(V, sizeof(bool))
 
    // Create a queue, enqueue source vertex and mark source vertex
    // as visited
    queue <int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;
 
    // Standard BFS Loop
    // TODO: optimize to have O(VE^2) time
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
 
        for (int v = 0; v < V; v++)
        {
            if (visited[v]==false && rGraph[u][v] > 0)
            {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    // If we reached sink in BFS starting from source, then return
    // true, else false
    //Before returning print path and save it to a collection of paths
    // std::cout << "===Path===" << std::endl;
    // for (int i = 0; i < V; i++)
    //     std::cout << i << " -> " << parent[i] << std::endl;
    // std::cout << std::endl;

    std::cout << "Result: " << (visited[sink] == true) << std::endl;

    return (visited[sink] == true);
}
 
// Returns the maximum flow from s to t in the given graph
int edmondsKarp(std::vector<std::vector<int> > graph, int source, int sink)
{
    // matrix indexes
    int u, v;
    int V = graph[0].size();    

    // Create a residual graph and fill the residual graph with
    // given capacities in the original graph as residual capacities
    // in residual graph
    //int rGraph[V*V]; // Residual graph where rGraph[i][j] indicates 
                     // residual capacity of edge from i to j (**IT WILL ALWAYS BE 1**) (if there
                     // is an edge. If rGraph[i][j] is 0, then there is not)
    std::vector<std::vector<int> > rGraph;
    rGraph = graph;

    // TODO: maybe have the shortest path also stored, since we may need it for the other edmonds-karp
    int parent[V];  // This array is filled by BFS and to store path
 
    int max_flow = 0;  // There is no flow initially
    int jp = 0; // counter for how many paths have been found
    // create a matrix with only zeros
    int flujo_actual[V][V];
    // bool visited[V];
    memset(flujo_actual, 0, sizeof(flujo_actual));
    
 
    // Augment the flow while there is path from source to sink
    while (bfs(rGraph, source, sink, parent, V))
    {
        // increment how many paths have been found
        jp++;
        // Find minimum residual capacity of the edges along the
        // path filled by BFS. Or we can say find the maximum flow
        // through the path found.

        // grab the paths and do the remove of paths algorithm
        std::vector<int> pathaumentado; //current path

        for (v = sink; v != source; v = parent[v])
        {
            if (pathaumentado.size() > 0)
            {
                //llenar matrix flujo_actual
                flujo_actual[v][pathaumentado.back()] = 1;  
            }
            pathaumentado.push_back(v);
        }
        flujo_actual[source][pathaumentado.back()] = 1;  
        pathaumentado.push_back(source);

        //imprimiendo flujo actual
        std::cout << "flujo actual:" << std::endl;
        for (int i = 0; i < V; ++i)
        {
            for (int j = 0; j < V; ++j)
            {
                std::cout << flujo_actual[i][j] << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        // si hay mas de un path
        if (jp>=2)
        {
            std::cout << "jp >=2" << std::endl;
            //tomar flujo_actual y columna por columna y guardar las posiciones de los 1s

            // mientras que ii no sea el nodo destino, !=sink o sink-1
            std::vector<int> posc; //vector con las posiciones que tienen 1s
            bool swe = false;
            int ii = 0;
            for (ii = 0; ii != sink; ii++) {
                posc.clear();
                for (int i = source; i < V; ++i)
                {
                    if (flujo_actual[i][ii] == 1)
                    {
                        posc.push_back(i);
                    }
                }

                if (posc.size() >= 2) {
                    swe = true;
                    break;
                }
            }

            if (swe) {
                // cambiar rGraph quitando los nodos repetidos
                // pathaumentado y posc
                std::cout << "swe: " << swe << std::endl;

                std::vector<int> pathaumentado2; //not current path

                for (v = sink; v != source; v = parent[v])
                {
                    if (pathaumentado2.size() > 0)
                    {
                        //llenar matrix flujo_actual
                        flujo_actual[v][pathaumentado2.back()] = 0;
                    }
                    pathaumentado2.push_back(v);
                }
                flujo_actual[source][pathaumentado2.back()] = 0;
                pathaumentado2.push_back(source);

                for (int i = 0; i < pathaumentado.size(); ++i)
                {
                    bool sw = false;
                    for (int j = 0; j < posc.size(); ++j)
                    {
                        if (posc[j] == pathaumentado[i])
                        {
                            rGraph[posc[j]][ii] = 0;
                            sw = true;
                            break;
                        }
                    }
                    if (sw)
                    {
                        break;
                    }
                }

                break; // break the while(bfs)
            }
        }
        
        // TODO: investigate if having max integer is necessary since capacity is always going to be 1
        int path_flow = INT_MAX;
        std::cout << "Path: ";
        for (v = sink; v != source; v = parent[v])
        {
            std::cout << v + 1 << " - ";
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }
        std::cout << source + 1 << std::endl;
        // update residual capacities of the edges and reverse edges
        // along the path
        for (v=sink; v != source; v=parent[v])
        {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
 
        // Add path flow to overall flow
        max_flow += path_flow;
    } // termina while de bfs

    // Return the overall flow
    return max_flow;
}

// load matrix from an ascii text file.
void load_matrix(std::istream* is,
        std::vector<std::vector<int> >* matrix,
        std::vector<int>* sinks,
        const std::string& delim = " \t")
{

    matrix->clear();
    std::string line;
    std::string strnum;
    int line_number = 0;

    while (getline(*is, line)) {
        if (line_number > 0)
            matrix->push_back(std::vector<int>());

        for (std::string::const_iterator i = line.begin(); i != line.end(); ++ i) {
            if (delim.find(*i) == std::string::npos) {
                strnum += *i;
                if (i + 1 != line.end())
                    continue;
            }

           if (strnum.empty())
                continue;

            int number;
            std::istringstream(strnum) >> number;

            // First line has the sinks
            if (line_number > 0)
                matrix->back().push_back(number);
            else
                sinks->push_back(number);

            strnum.clear();
        }
        line_number++;
    }
}

// Driver program to test above functions
int main(int argc, char *argv[])
{   
    // File read validations
    // /Users/epsanchezma/Documents/Tesis/2017/ejemplos/12grafo2fm1v3des.txt
    if (argc != 2) {
        std::cerr << "Use: ./reader FILENAME" << std::endl;
        return -1;
    }

    std::ifstream is(argv[1]);
    if (!is.is_open()) {
        std::cerr << "Can't open file" << std::endl;
        return -1;
    }

    // Reading the file to get matrix and sinks
    std::vector<std::vector<int> > matrix;
    std::vector<int> sinks;
    load_matrix(&is, &matrix, &sinks);

    // Printing matrix and sinks
    std::cout << "Matrix:" << std::endl;
    for (std::vector< std::vector<int> >::const_iterator it = matrix.begin(); it != matrix.end(); ++ it) {
        for (std::vector<int>::const_iterator itit = it->begin(); itit != it->end(); ++ itit)
            std::cout << *itit << '\t';
        std::cout << std::endl;
    }
    std::cout << "Sinks:" << std::endl;
    for (std::vector<int>::const_iterator i = sinks.begin(); i != sinks.end(); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;
    
    // For every sink run Edmonds-Karp
    int sinksLength = sinks.size();
    int maxFlows[matrix.size()];
    for (int i = 0; i < sinksLength; i++)
    {
        int result = edmondsKarp(matrix, 0, sinks[i]-1);
        cout << "The maximum possible flow for node " << sinks[i] << " is " << result << std::endl;
        maxFlows[i] = result;
    }

    return 0;
}