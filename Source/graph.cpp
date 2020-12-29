#include "graph.h"
list<int> domainsSizes;
set<std::pair<int, int>> EdgesSet;
Graph::Graph(int V)
{
	this->V = V;
	outAdj = new list<int>[V];
	inAdj = new list<int>[V];
	Gamma = -1;
	visited = new bool[V];
	for (int i = 0; i < V; i++)
	{

		visited[i] = false;
	}
}
Graph::Graph()
{

	outAdj = new list<int>[V];
	inAdj = new list<int>[V];
	Gamma = -1;
	visited = new bool[V];
	for (int i = 0; i < V; i++)
	{

		visited[i] = false;
	}
}
Graph::Graph(int V, set<std::pair<int, int>> Edgeset)
{
	this->V = V;
	outAdj = new list<int>[V];
	inAdj = new list<int>[V];
	Gamma = -1;
	set<std::pair<int, int>>::iterator it1;
	visited = new bool[V];
	for (int i = 0; i < V; i++)
	{

		visited[i] = false;
	}
	for (it1 = Edgeset.begin();it1 != Edgeset.end();it1++)
	{
		outAdj[(*it1).first].push_back((*it1).second); // Add w to v’s out list. 
		inAdj[(*it1).second].push_back((*it1).first); // Add v to w’s in list
	}
}

void Graph::addEdge(int v, int w)
{

	outAdj[v].push_back(w); // Add w to v’s out list. 
	inAdj[w].push_back(v); // Add v to w’s in list
}
bool Graph::ExistsEdge(int src, int dest)
{
	list<int>::iterator it;
	for (it = outAdj[src].begin(); it != outAdj[src].end(); it++)
	{
		if ((*it) == dest)
			return true;
	}
	return false;
}
set<std::pair<int, int>> Graph::getSpanningTreeEdges(int root)
{
	// Mark all the vertices as not visited 
	//bool *visited = new bool[V];
	for (int i = 0; i < V; i++)
		visited[i] = false;

	// Create a queue for BFS 
	list<int> queue;
	set<std::pair<int, int>> EdgesSet1;
	set<int> selectedVertx;
	// Mark the current node as visited and enqueue it 
	visited[root] = true;
	queue.push_back(root);

	// 'i' will be used to get all adjacent 
	// vertices of a vertex 
	list<int>::iterator i;
	list<int>::iterator rIt;
	set<std::pair<int, int>>::iterator it1;
	set<int>::iterator it;
	int v;
	while (!queue.empty())
	{
		// Dequeue a vertex from queue and print it 
		v = queue.front();
		cout << v << " " << endl;
		queue.pop_front();
		if (v == root)
		{
			if (inAdj[v].size() > 0)
			{
				int r = rand() % (distance(inAdj[v].begin(), inAdj[v].end()));
				rIt = next(inAdj[v].begin(), r);
				EdgesSet1.insert(make_pair(*rIt, v));
				cout << "Add Edge : " << *rIt << " --> " << v << endl;
				cout << "Edge set :\n";
				for (it1 = EdgesSet1.begin();it1 != EdgesSet1.end();it1++)
					cout << (*it1).first << "-> " << (*it1).second << endl;
				selectedVertx.insert(v);
				selectedVertx.insert(*rIt);
				cout << "selected vertex:\n";
				for (it = selectedVertx.begin();it != selectedVertx.end();it++)
					cout << (*it) << endl;
			}
		}
		else
		{

			//while (!visited[*(rIt = next(outAdj[v].begin(), rand() % (distance(outAdj[v].begin(), outAdj[v].end()))))]);
			while (!selectedVertx.count(*(rIt = next(outAdj[v].begin(), rand() % (distance(outAdj[v].begin(), outAdj[v].end()))))));
			EdgesSet1.insert(make_pair(v, (*rIt)));
			cout << "Add Edge : " << v << " --> " << *rIt << endl;
			cout << "Edge set :\n";
			for (it1 = EdgesSet1.begin();it1 != EdgesSet1.end();it1++)
				cout << (*it1).first << "-> " << (*it1).second << endl;

			selectedVertx.insert(v);
			selectedVertx.insert(*rIt);
			cout << "selected vertex:\n";
			for (it = selectedVertx.begin();it != selectedVertx.end();it++)
				cout << (*it) << endl;
		}

		if (inAdj[v].size() > 0)
		{
			// Get all adjacent vertices of the dequeued 
			// vertex s. If a adjacent has not been visited,  
			// then mark it visited and enqueue it 
			for (i = inAdj[v].begin(); i != inAdj[v].end(); ++i)
			{
				if (!visited[*i])
				{
					visited[*i] = true;
					if (*i != *rIt)
						queue.push_back(*i);

				}
			}
		}
	}
	return EdgesSet1;
}

void Graph::DFSUtil(int v, bool visited[])
{
	// Mark the current node as visited and 
	// print it 
	visited[v] = true;
	cout << v << " ";

	// Recur for all the vertices adjacent 
	// to this vertex 
	list<int>::iterator i;
	for (i = outAdj[v].begin(); i != outAdj[v].end(); ++i)
		if (!visited[*i])
			DFSUtil(*i, visited);
}

bool isValidVertex(int i)
{
	list<string>::iterator it;
	list<int>::iterator it2;
	int M = 0;
	//<<"\n>>>>>>>" << *(domainsSizes.begin()) << endl;
	for (it2 = domainsSizes.begin(); it2 != domainsSizes.end(); it2++)
	{
		int preVarsSep = pow(2.0, M);
		int varSep = pow(2.0, ((floor(log2((*it2) - 1)) + 1)));
		if ((preVarsSep > 1 ? ((i / preVarsSep) % varSep) : (i%varSep)) >= (*it2))
			return false;
		M += (floor(log2((*it2) - 1)) + 1);
	}
	return true;
}
void Graph::print(listType lType)
{
	for (int i = 0; i < V; i++)
	{
		cout << i << ((lType == out) ? "--->" : "<---");
		if (isValidVertex(i))
		{

			list<int> ::iterator it;
			for (it = ((lType == out) ? outAdj[i].begin() : inAdj[i].begin()); it != ((lType == out) ? outAdj[i].end() : inAdj[i].end()); ++it) {
				cout << *it << " ";
			}
			cout << endl;
		}
		else
		{
			cout << "Vertex " << i << " is invalid!\n";
		}
	}

}

void Graph::setUnusedGamma()
{
	for (int i = Gamma + 1;i < V;i++)
	{
		if (isValidVertex(i))
		{
			if (i == *outAdj[i].begin())
			{
				Gamma = i;
				return;

			}
		}


	}
	Gamma = -1;
}
void Graph::copyTo(Graph * g)
{
	g->V = V;
	for (int i = 0; i < V; i++)
	{
		list<int> ::iterator it;
		for (it = outAdj[i].begin(); it != outAdj[i].end(); it++)
		{
			g->addEdge(i, *it);
		}

	}
}
void Graph::copyTo(Graph * g, set<int> sourceVertex)
{
	g->V = V;
	for (int i = 0; i < V; i++)
	{
		list<int> ::iterator it;
		for (it = outAdj[i].begin(); it != outAdj[i].end(); it++)
		{
			if (sourceVertex.count(i) == 0)//Remove edges that source in sourceVertex
				g->addEdge(i, *it);
		}

	}
}
void Graph::removeEdges(set<int> sourceVertex)
{
	set<int>::iterator sourceIt;
	list<int> ::iterator destIt;
	for (sourceIt = sourceVertex.begin(); sourceIt != sourceVertex.end(); sourceIt++)
	{
		for (destIt = outAdj[*sourceIt].begin(); destIt != outAdj[*sourceIt].end(); destIt++)
		{
			inAdj[*destIt].remove(*sourceIt);

		}
		outAdj[*sourceIt].clear();
	}
}
void Graph::DFS(int v)
{
	// Mark all the vertices as not visited 
	bool *visited = new bool[V];
	for (int i = 0; i < V; i++)
		visited[i] = false;

	// Call the recursive helper function 
	// to print DFS traversal 
	DFSUtil(v, visited);
}
set<std::pair<int, int>> checkedPath;
void  Graph::GetEdgesInCycles(int v1, int v2)
{
	visited[v1] = true;
	visited[v2] = true;
	cout << "(" << v1 << "," << v2 << "):\n";
	list<int> ::iterator out_it;
	list<int> ::iterator in_it;
	//sleep(3);
	checkedPath.insert(make_pair(v1, v2));

	set<std::pair<int, int>>::iterator it1;
	for (out_it = outAdj[v1].begin();(out_it != outAdj[v1].end()) && (outAdj[*out_it].size() > 0);out_it++)
	{
		cout << "outAdj[" << v1 << "]:" << *out_it << endl;

		if (visited[*out_it] == false)
		{
			cout << "++ select outAdj[" << v1 << "]:" << *out_it << endl;

			for (in_it = inAdj[v2].begin();(in_it != inAdj[v2].end()) && (inAdj[*in_it].size() > 0);in_it++)
			{
				cout << "inAdj[" << v2 << "]:" << *in_it << endl;

				if (visited[*in_it] == false)
				{
					cout << "++ select inAdj[" << v2 << "]:" << *in_it << endl;

					if (*out_it == *in_it)
					{
						cout << *out_it << " = " << *in_it << endl;
						EdgesSet.insert(make_pair(v1, *out_it));
						EdgesSet.insert(make_pair(*in_it, v2));
						cout << "Add Edge : " << v1 << " --> " << *out_it << endl;
						cout << "Add Edge : " << *in_it << " --> " << v2 << endl;
						cout << "Edge set :\n";
						for (it1 = EdgesSet.begin();it1 != EdgesSet.end();it1++)
							cout << (*it1).first << "-> " << (*it1).second << endl;
						
						cout<<"1-&&&&&&&&-> EdgesSet size: "<<EdgesSet.size()<<endl;
					}
					else
					{

						cout << *out_it << " != " << *in_it << endl;
						if(checkedPath.count(make_pair(v1, v2)) == 0)
						{
							cout << "GetEdgesInNoCycle(" << *out_it << "," << *in_it << ")\n";
							GetEdgesInCycles(*out_it, *in_it);
						}
						else
						{
							cout <<"Repetetive Path: "<<" Calling **** " <<"GetEdgesInNoCycle(" << *out_it << "," << *in_it << ") rejected!\n";
						}
						visited[*in_it] = false;
						visited[*out_it] = false;
						EdgesSet.insert(make_pair(v1, *out_it));
						EdgesSet.insert(make_pair(*in_it, v2));

						cout << "After backtrack Add Edge : " << v1 << " --> " << *out_it << endl;
						cout << "After backtrack Add Edge : " << *in_it << " --> " << v2 << endl;
						if (*in_it != *out_it)
						{
							(EdgesSet.insert(make_pair(*in_it, *out_it)));
							cout << "After back track: " << *in_it << " != " << *out_it << endl;
							cout << "After backtrack Add Edge : " << *in_it << " --> " << *out_it << endl;
						}
						cout << "Edge set :\n";
						for (it1 = EdgesSet.begin();it1 != EdgesSet.end();it1++)
							cout << (*it1).first << "-> " << (*it1).second << endl;
						cout<<"2-&&&&&&&&-> EdgesSet size: "<<EdgesSet.size()<<endl;


					}
				}
				else
					cout << "** reject inAdj[" << v2 << "]:" << *in_it << endl;
			}
		}
		else
			cout << "** reject outAdj[" << v1 << "]:" << *out_it << endl;
	}
	cout << "Final edge set is :\n";
	for (it1 = EdgesSet.begin();it1 != EdgesSet.end();it1++)
		cout << (*it1).first << "-> " << (*it1).second << endl;
}
set<int> Graph::getVertex(vertexType typ)
{
	set<int> vertex;
	switch (typ)
	{
	case isolated:
		for (int i = 0; i < V; i++)
		{
			if (isValidVertex(i))
				if (outAdj[i].size() == 0 && inAdj[i].size() == 0)
					vertex.insert(i);
		}
		break;
	case no_isolated:
		for (int i = 0; i < V; i++)
		{
			if (isValidVertex(i))
				if (outAdj[i].size() != 0 || inAdj[i].size() != 0)
					vertex.insert(i);
		}
		break;
	case source:
		/*if (!inAdj->empty())
		{*/
		for (int i = 0; i < V; i++)
		{
			if (isValidVertex(i))
				if (inAdj[i].size() == 0)
					vertex.insert(i);
		}
		//}
		break;
	case source_restrict:

		for (int i = 0; i < V; i++)
		{
			if (isValidVertex(i))
				if (inAdj[i].size() == 0 && outAdj[i].size() != 0)
					vertex.insert(i);
		}

		break;
	case sink:break;
		for (int i = 0; i < V; i++)
		{
			if (isValidVertex(i))
				if (outAdj[i].size() == 0)
					vertex.insert(i);
		}
	case noCycle:
		for (int i = 0; i < V; i++)
		{
			if (isValidVertex(i))
				if (outAdj[i].size() == 0 || inAdj[i].size() == 0)
					vertex.insert(i);
		}
		break;
	case None:
		for (int i = 0; i < V; i++)
		{
			if (isValidVertex(i))
				if (outAdj[i].size() != 0 && inAdj[i].size() != 0)
					vertex.insert(i);
		}
		break;
	case All:
		for (int i = 0; i < V; i++)
		{
			if (isValidVertex(i))
				vertex.insert(i);
		}
		break;

	}
	return vertex;
}
set<int> Graph::getVertex(int src)
{
	set<int> vertex;
	list<int>::iterator setIt;
	for (setIt = outAdj[src].begin(); setIt != outAdj[src].end(); setIt++)
	{
		vertex.insert((*setIt));
	}
	return vertex;
}
