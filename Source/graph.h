

#include "synLeadsTo.h"

enum vertexType {
	isolated, source, sink, noCycle, None, All, no_isolated, source_restrict
};
enum listType
{
	in, out
};

class Graph
{
	void DFSUtil(int v, bool visited[]);
public:
	int V; // No. of vertices
	Graph(int V); // Constructor 
	Graph(int V, set<std::pair<int, int>> Edgeset);
	Graph();
	list<int> *outAdj;
	list<int> *inAdj;
	bool *visited;
	int Gamma;

	void addEdge(int v, int w);
	void print(listType lType = out);
	void setUnusedGamma();
	void copyTo(Graph * g);
	void copyTo(Graph * g, set<int> vertex);
	void removeEdges(set<int> sourceVertex);
	void  GetEdgesInCycles(int v1, int v2);
	set<int> getVertex(vertexType typ);
	set<int> getVertex(int src);
	bool ExistsEdge(int src, int dest);

	void DFS(int v);
	set<std::pair<int, int>> getSpanningTreeEdges(int root);
};

bool isValidVertex(int);

	

