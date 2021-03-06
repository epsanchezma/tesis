// C++ program for implementation of Ford Fulkerson algorithm!
#include <iostream>
#include <limits.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <libgen.h>

using namespace std;
struct ekResult
{
    int max_flow;
    int **flujo_completo;
    std::vector<std::vector<int> > paths;
};
  
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

    //std::cout << "Result: " << (visited[sink] == true) << std::endl;

    return (visited[sink] == true);
}
 
// Returns the maximum flow from s to t in the given graph
struct ekResult edmondsKarp(std::vector<std::vector<int> > graph, int source, int sink)
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
    memset(flujo_actual, 0, sizeof(flujo_actual));

    struct ekResult result;
 
    // Augment the flow while there is path from source to sink
    while (bfs(rGraph, source, sink, parent, V))
    {   
        std::cout << "Entro al while de bfs..." << std::endl;
        bool otrosw = false;
        // increment how many paths have been found
        jp++;
        // Find minimum residual capacity of the edges along the
        // path filled by BFS. Or we can say find the maximum flow
        // through the path found.

        // grab the paths and do the remove of paths algorithm
        std::vector<int> pathaumentado; //current path

        for (v = sink; v != source; v = parent[v]) {
            if (pathaumentado.size() > 0) {
                //llenar matrix flujo_actual
                flujo_actual[v][pathaumentado.back()] = 1;  
            }
            pathaumentado.push_back(v);
        }
        flujo_actual[source][pathaumentado.back()] = 1;  
        pathaumentado.push_back(source);

        //imprimiendo flujo actual
        /*
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
        */
        // si hay mas de un path
        if (jp >= 2) {
            //tomar flujo_actual y columna por columna y guardar las posiciones de los 1s

            // mientras que ii no sea el nodo destino, !=sink o sink-1
            std::vector<int> posc; //vector con las posiciones que tienen 1s
            bool swe = false;
            int ii = 0;
            for (ii = 0; ii != sink; ii++) {
                posc.clear();
                for (int i = source; i < V; ++i) {
                    if (flujo_actual[i][ii] == 1) {
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
                std::cout << "Found a repeated route, modifying graph and running bfs again..." << std::endl;
                otrosw = true;
                std::vector<int> pathaumentado2; //not current path

                for (v = sink; v != source; v = parent[v]) {
                    if (pathaumentado2.size() > 0)
                        flujo_actual[v][pathaumentado2.back()] = 0;
                    pathaumentado2.push_back(v);
                }
                flujo_actual[source][pathaumentado2.back()] = 0;
                pathaumentado2.push_back(source);

                for (int i = 0; i < pathaumentado.size(); ++i) {
                    bool sw = false;
                    for (int j = 0; j < posc.size(); ++j) {
                        if (posc[j] == pathaumentado[i]) {
                            rGraph[posc[j]][ii] = 0;
                            sw = true;
                            break;
                        }
                    }
                    if (sw)
                        break;
                }

                // break; // break the while(bfs)  
            }
        }
        
        if (!otrosw)
        {
            // TODO: investigate if having max integer is necessary since capacity is always going to be 1
            int path_flow = INT_MAX;

            std::cout << "Path: ";
            std::vector<int> path; // crear un vector para guardar el path
            for (v = sink; v != source; v = parent[v]) {
                path.push_back(v);
                std::cout << v << " - ";
                u = parent[v];
                path_flow = min(path_flow, rGraph[u][v]);
            }
            std::cout << source << std::endl;
            path.push_back(source);

            // save paths to an array of paths (vectors)
            result.paths.push_back(path);
            // update residual capacities of the edges and reverse edges
            // along the path
            for (v = sink; v != source; v = parent[v]) {
                u = parent[v];
                rGraph[u][v] -= path_flow;
                rGraph[v][u] += path_flow;
            }
     
            // Add path flow to overall flow
            max_flow += path_flow;
        }
    } // termina while de bfs

    // Return the overall flow and flujo_actual
    result.max_flow = max_flow;

    result.flujo_completo =  new int*[V];
    for (int i = 0; i < V; i++) {
        result.flujo_completo[i] = new int[V];
        for (int j = 0; j < V; j++)
            result.flujo_completo[i][j] = flujo_actual[i][j];
    }

    return result;
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
    memset(maxFlows, 0, sizeof(maxFlows));
    int V = matrix[0].size();
    int finalMatrix[V][V];
    memset(finalMatrix, 0, sizeof(finalMatrix));

    // array de resultados
    std::vector<struct ekResult> generalResults;

    for (int i = 0; i < sinksLength; i++)
    {
        struct ekResult result = edmondsKarp(matrix, 0, sinks[i]-1);
        // cout << "The maximum possible flow for node " << sinks[i] << " is " << result.max_flow << std::endl;
        std::cout << "original max flow: " << result.max_flow << std::endl;

        maxFlows[i] = result.max_flow;
        generalResults.push_back(result);
    }

    int minFlow = maxFlows[0];
    for (int i = 0; i < matrix.size(); i++) {
        if (maxFlows[i] < minFlow && maxFlows[i] != 0) {
            minFlow = maxFlows[i];
        }
    }

    // con el minFlow, eliminar paths en results que tengan su maxflow mayor al minFlow general
    for (int i = 0; i < generalResults.size(); ++i)
    {
        while (generalResults[i].max_flow > minFlow)
        {
            
            std::cout << "eliminando i: " << i << ", current max flow: " << generalResults[i].max_flow << ", should be min_flow: " << minFlow << std::endl;
            int ajdl = 0;
            int indice = 0;
            std::vector<int> biggerPath;
            for (int j = 0; j < generalResults[i].paths.size(); ++j)
            {
                if (generalResults[i].paths[j].size() > ajdl) {
                    indice = j;
                    ajdl = generalResults[i].paths[j].size();
                    biggerPath = generalResults[i].paths[j];
                }
            }

            generalResults[i].paths.erase(generalResults[i].paths.begin() + indice);

            for (int k = biggerPath.size() - 1; k > 0 ; --k)
            {
                generalResults[i].flujo_completo[biggerPath[k]][biggerPath[k-1]] = 0;
            }

            generalResults[i].max_flow -= 1;
        }
    }

    // join general results
    for (int l = 0; l < generalResults.size(); ++l)
    {
        cout << "The maximum flow for node " << sinks[l]-1 << " is " << generalResults[l].max_flow << std::endl;

        for (int m = 0; m < V; m++) {
            for (int n = 0; n < V; n++) {
                finalMatrix[m][n] += generalResults[l].flujo_completo[m][n];
                std::cout << generalResults[l].flujo_completo[m][n] << ' ';
            }
            std::cout << std::endl;
        }
    }

    // create file with results
    // std::string filename = std::to_string(V) + "_nodes_graph_mf_" + std::to_string(minFlow) + "_ff5.ffm";
    std::string filename = basename(argv[1]);
    
    size_t pos = filename.rfind('.');
    if(pos != string::npos)
        filename.replace(pos, filename.length() - pos, ".ffm");
    else
        filename.append(".ffm");

    ofstream myfile;
    myfile.open (filename);

    for (std::vector<int>::const_iterator i = sinks.begin(); i != sinks.end(); ++i)
        myfile << *i - 1 << ' ';
    myfile << '\n';

    std::vector<int> nodos_codificadores;
    for (int i = 0; i < V; i++) {
        // If we're not in a sink
        if (!( std::find(sinks.begin(), sinks.end(), i+1) != sinks.end() )) {
            int counter = 0;
            // Check for more than one 1 in that column
            for (int j = 0; j < V; j++) {
                //std::cout << finalMatrix[j][i] << ' ';
                if (finalMatrix[j][i] > 0)
                    counter ++;

                if (counter > 1) {
                    nodos_codificadores.push_back(i);
                    break;
                }
            }
            //std::cout << std::endl;
        }
    }

    for (std::vector<int>::const_iterator i = nodos_codificadores.begin(); i != nodos_codificadores.end(); ++i)
        myfile << *i << ' ';
    myfile << '\n';

    std::cout << "\nFinal matrix: \n";
    for (int m = 0; m < V; m++) {
        for (int n = 0; n < V; n++) {
            std::cout << (finalMatrix[m][n] > 0) << ' ';
            myfile << (finalMatrix[m][n] > 0) << ' ';
        }
        std::cout << std::endl;
        myfile << '\n';
    }

    myfile.close();

    return 0;
}