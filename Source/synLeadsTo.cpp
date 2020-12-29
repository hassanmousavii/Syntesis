#include "graph.h" 
#include <vector>
#include <stdlib.h>
#include <utility>
#include<random>
#include<algorithm>
//#include<dos.h>
#include<tuple>
#include <fstream>
#include <dlfcn.h>
#include <stdlib.h>
#include <cstdarg>
#include <unistd.h>
#include <sstream>
#include <sys/time.h>

#define max_vars 10

struct timeval start, stop;

ofstream fOutLeadsTo; //object created **fout**
ifstream finDecoded;

//list<int> domainsSizes;

int eval(const char *boolean_expression) {
	FILE *fp = fopen("temp.c", "w");
	int ret;
	fprintf(fp,
		"#include <stdbool.h>\n"
		"#include <iso646.h>\n"
		"int main(void){\n"
		"return (int)%s;}", boolean_expression);
	fflush(fp);
	fclose(fp);
	system("gcc temp.c -o temp");
	remove("temp.c");
	ret = system("temp.exe");
	remove("temp.exe");
	return ret;
}
void replaceAll(string& str, const string& from, const string& to) {
	if (from.empty())
		return;
	string wsRet;
	wsRet.reserve(str.length());
	size_t start_pos = 0, pos;
	while ((pos = str.find(from, start_pos)) != string::npos) {
		wsRet += str.substr(start_pos, pos - start_pos);
		wsRet += to;
		pos += from.length();
		start_pos = pos;
	}
	wsRet += str.substr(start_pos);
	str.swap(wsRet); // faster than str = wsRet;
}


inline std::string format(const char* fmt, ...) {
	int size = 2048;
	char* buffer = 0;
	buffer = new char[size];
	va_list vl;
	va_start(vl, fmt);
	int nsize = vsnprintf(buffer, size, fmt, vl);
	if (size <= nsize) { //fail delete buffer and try again
		delete[] buffer;
		buffer = 0;
		buffer = new char[nsize + 1]; //+1 for /0
		nsize = vsnprintf(buffer, size, fmt, vl);
	}
	std::string ret(buffer);
	va_end(vl);
	delete[] buffer;
	return ret;
}		
		

typedef void* (*arbitrary)(Graph *,Graph *,list<int>);
void createGraph(string q_pred,Graph * g_q, string r_pred,Graph * g_r)
{
	arbitrary FillAdjsLists;
	// create own program
	ofstream f("tmp.cpp");
	f << format("#include \"graph.h\"\n #include<stdlib.h>\n"
                "bool isValidVertex_1(int i,list<int> d_Sizes)\n"
                "{\n"
                "list<string>::iterator it;\n"
	        "list<int>::iterator it2;\n"
	        "int M = 0;\n"
	        "for (it2 = d_Sizes.begin(); it2 != d_Sizes.end(); it2++)\n"
	        "{\n"
		"int preVarsSep = pow(2.0, M);\n"
		"int varSep = pow(2.0, ((floor(log2((*it2) - 1)) + 1)));\n"
		"if ((preVarsSep > 1 ? ((i / preVarsSep) % varSep) : (i%varSep)) >= (*it2))\n"
		"return false;\n"
		"M += (floor(log2((*it2) - 1)) + 1);\n"
	        "}\n"
	        "return true;\n"
                "}\n"
		"extern \"C\" void FillAdjsLists(Graph *g_Q,Graph *g_R,list<int> d_Sizes ){\n"
		"int pre_x=0;\n"
		"int curr_x=0;\n"
                "int v = g_Q->V;\n"
		"for (int i = 0; i < v; i++)\n"
		"{\n"
		"if (isValidVertex_1(i,d_Sizes))\n"
		"{\n"
		"//cout <<\"i=\"<<i<<\" isValid! \"<<endl; \n"
		"pre_x=i;\n"
		"curr_x=i;\n"
		"if(%s)\n"
		"{\n"
		"g_Q->addEdge(i, i);\n"
		"if (g_Q->Gamma == -1) g_Q->Gamma = i;\n"
		"}\n"
		"if(%s)\n"
		"{\n"
		"g_R->addEdge(i, i);\n"
		"if (g_R->Gamma == -1) g_R->Gamma = i;\n"
		"}\n"
		"for (int j = 0; j < v; j++)\n"
		"{\n"
		"if(j!=i)\n"
		"{\n"
                "pre_x=i;\n"
		"curr_x=j;\n"
		"if (isValidVertex_1(j,d_Sizes))\n"
		"{\n"
		"//cout <<\"j=\"<<j<<\" isValid! \"<<endl; \n"
		"if(%s)\n"
		"{\n"
		"g_Q->addEdge(i, j);\n"
		"}\n"
		"if(%s)\n"
		"{\n"
		"g_R->addEdge(i, j);\n"
		"}\n"
		"}\n"
		"}\n"
		"}\n"
                "}\n"
		"}\n"
                "}\n", q_pred.c_str(),r_pred.c_str(), q_pred.c_str(), r_pred.c_str());
	f.close();
	// create library
	system("g++ -shared graph.cpp tmp.cpp -o libtmp.so");
	// load library        
	void * fLib = dlopen("./libtmp.so", RTLD_LAZY);
	if (!fLib) {
		cerr << "Cannot open library: " << dlerror() << '\n';
	}
	if (fLib) {
		*(void**)(&FillAdjsLists) = dlsym(fLib, "FillAdjsLists");
		if (FillAdjsLists) {
			list<int> *l = new list<int>[128];;
			// use function
			(void)FillAdjsLists(g_q,g_r,domainsSizes);
		}
		//dlclose(fLib);
	}
}

bool hasEdge(string predicate, int x_i_1, int x_i)
{
	replaceAll(predicate, "pre_x", std::to_string(x_i_1));
	replaceAll(predicate, "curr_x", std::to_string(x_i));
	
	char predicateArr[1000];
	strncpy(predicateArr, predicate.c_str(), sizeof(predicateArr));
	int condition = eval(predicateArr);
	if (condition)
		return true;
	
	return false;
}
void CreateGraph(Graph *g,std::string predicate)
{
	//Graph * g1 = createGraph(g,predicate);
	int x;
	for (int i = 0; i < g->V; i++)
	{
		if (isValidVertex(i))
		{
			if (hasEdge(predicate, i, i))
			{
				if (g->Gamma == -1) g->Gamma = i;
				g->addEdge(i, i); // self loop vertex in the begining of the each adj list
			}
			for (int j = 0; j < g->V; j++)
			{
				if (isValidVertex(j))
				{
					if (i == 3 && j == 2)
						x = 1;
					if (j != i)
						if (hasEdge(predicate, i, j))
						{
							g->addEdge(i, j);
						}
				}
			}
		}
		
		
	}
}
void buildQPrime(Graph *q,Graph *qprime)
{
	for (int i = 0; i < qprime->V; i++)
	{
		if (isValidVertex(i))
		{
			if (qprime->outAdj[i].size() == 0)
				for (int j = 0; j < qprime->V; j++)
				{
					if (isValidVertex(j))
					{
						list<int> ::iterator it;
						for (it = qprime->outAdj[j].begin(); it != qprime->outAdj[j].end(); it++)
						{
							if (*it == i)
							{
								qprime->outAdj->erase(it);
								break;
							}
						}
					}
				}
		}

	}
}
void removeEdgesInNoCyc(int gV, int v, list<int> *listToSearch, list<int> *listToRemove)
{
	cout << "v: " << v << endl;
	if (v + 1 > gV)
		return;
	if (listToSearch[v].size() > 1 || (listToSearch[v].size() == 1 && *listToSearch[v].begin() != v))
	{
		/*if(listToRemove[v].size() == 0)
		{ }*/
		removeEdgesInNoCyc(gV, ++v, listToSearch, listToRemove);
		return;
	}

	list<int> ::iterator it;
	list<int> ::iterator it1;
	for (it = listToRemove[v].begin(); it != listToRemove[v].end();)
	{
		it = listToRemove[v].erase(it);
		listToSearch[*it].remove(v);
		//gV--;
		cout << "listToSearch after remove " << v << endl;
		for (int i = 0; i < gV - 1; i++)
		{
			if (isValidVertex(i))
			{
				cout << i << "-->";
				for (it1 = listToSearch[i].begin(); it1 != listToSearch[i].end(); it1++)
				{
					cout << (*it1) << " ";
				}
				cout << endl;
			}
		}
		cout << "listToRemove after remove " << *it << endl;
		for (int j = 0; j < gV - 1; j++)
		{
			if (isValidVertex(j))
			{
				cout << j << "-->";
				for (it1 = listToRemove[j].begin(); it1 != listToRemove[j].end(); it1++)
				{
					cout << (*it1) << " ";
				}
				cout << endl;
			}
		}

		if ((listToSearch[*it].size() == 0) || ((listToSearch[*it].size() == 1) && (*listToSearch[*it].begin() == *it)))
			removeEdgesInNoCyc(gV, *it, listToSearch, listToRemove);
	}
	removeEdgesInNoCyc(gV, ++v, listToSearch, listToRemove);

}
void print(set<int> setParam)
{
	set<int>::iterator setIt;
	for (setIt = setParam.begin(); setIt != setParam.end(); setIt++)
	{
		//VR_leaf_prime.insert(*setIt);
		cout << (*setIt) << " ";
	}
	cout << endl;
}
void print(set<std::pair<int,int>> setParam)
{
	set<std::pair<int, int>>::iterator setIt;
	for (setIt = setParam.begin(); setIt != setParam.end(); setIt++)
	{
		//VR_leaf_prime.insert(*setIt);
		cout << (*setIt).first << "--->"<< (*setIt).second;
	}
}

void do_step1(string r_predicate,Graph *g_R,string q_predicate,Graph *g_Q)
{
	//CreateGraph(g_R, r_predicate);
	//CreateGraph(g_Q, q_predicate);
	createGraph(q_predicate, g_Q, r_predicate, g_R);
}
int do_step2(Graph *g_Q,bool setUnused)
{
	if(setUnused)
		(*g_Q).setUnusedGamma();
	return (*g_Q).Gamma;
}
void do_step3(Graph * g_Q,int Gamma,Graph * g_Q_prime,set<int> *V_q_prime)
{
	(*g_Q).GetEdgesInCycles(Gamma, Gamma);

	// Testing Edges In Cycles involving gamma
	if (EdgesSet.size() > 0)
	{
		cout << "Graph Q has edges in Cycle involving Gamma <<" << Gamma << ">> :\n";
		cout << "The edges are :\n";
		print(EdgesSet);
	}
	else
	{
		cout << "There is no edge in cycles involving Gamma <<" << Gamma << " in Graph Q\n";
	}
	//return;
	// End of testing Edges In Cycles involving gamma
	cout << "Now Creation of Q_prime graph:\n";
	Graph g_Q_prime_cop((*g_Q).V, EdgesSet);
	cout << "Q_prime graph:\n";
	g_Q_prime_cop.copyTo(g_Q_prime);
	(*g_Q_prime).print();
	(*V_q_prime)= (*g_Q_prime).getVertex(None);
	if (EdgesSet.size() == 0)
		(*V_q_prime).insert(Gamma);
	cout << "V_q_prime:\n";
	print(*V_q_prime);
	cout << "*********End of 3*********\n";
}
void do_step4(Graph * g_Q_prime,int Gamma, Graph *sTree_gamma)
{
	set<std::pair<int, int>> sTreeEdgesSet = (*g_Q_prime).getSpanningTreeEdges(Gamma);
	cout << "sTreeEdgesSet:\n";
	print(sTreeEdgesSet);
	Graph sTree_gamma_cop((*g_Q_prime).V, sTreeEdgesSet);
	sTree_gamma_cop.copyTo(sTree_gamma);
	cout << "sTree_gamma:\n";
	(*sTree_gamma).print();
	cout << "*********End of 4*********\n";
}
void do_step5(Graph * g_R,set<int> *V_R_prime)
{
	Graph g_R_cop((*g_R).V);
	(*g_R).copyTo(&g_R_cop);

	cout << "listToSearch before call " << endl;
	g_R_cop.print(out);

	
	cout << "listToRemove before call " << endl;
	g_R_cop.print(in);
	
	
	removeEdgesInNoCyc(g_R_cop.V, 0, g_R_cop.outAdj, g_R_cop.inAdj);
	cout << "listToSearch after call removeEdgesInNoCyc: " << endl;
	g_R_cop.print(out);
	
	cout << "listToRemove after call removeEdgesInNoCyc: " << endl;
	g_R_cop.print(in);
	
	cout << "Vertexes of G_R that  participate in atleast one cycle:\n";
	cout << "VR:" << endl;
	set<int> V_R = g_R_cop.getVertex(All);
	print(V_R);
	
	(*V_R_prime) = g_R_cop.getVertex(noCycle);
	
	cout << "noCycle VR, V_R_Prime:\n";
	print(*V_R_prime);
	
	cout << "*********End of 5*********\n";
}
void do_step6(Graph *sTree_gamma, set<int> *V_R_prime,Graph *sTree_gamma_prime,set<int> * VR_leaf_prime,int Gamma)
{
	set<int> sTree_gamma_leaves = (*sTree_gamma).getVertex(source);
	if (sTree_gamma_leaves.size() == 0 && (*sTree_gamma).inAdj->empty())
		sTree_gamma_leaves.insert(Gamma);
	cout << "sTree_gamma_leaves:\n";
	print(sTree_gamma_leaves);


	set_intersection((*V_R_prime).begin(), (*V_R_prime).end(), sTree_gamma_leaves.begin(), sTree_gamma_leaves.end(), insert_iterator< set<int> >((*VR_leaf_prime), (*VR_leaf_prime).end()));
	
	cout << "V_R_Prime intersection sTree_gamma_leaves:\n";
	print(*VR_leaf_prime);
	
	(*sTree_gamma).copyTo(sTree_gamma_prime, (*VR_leaf_prime));
	
	cout << "sTree_gamma_prime:\n";
	(*sTree_gamma_prime).print();
	cout << endl;
	cout << "*********End of 6*********\n";
}
void do_step7(Graph * g_Q,Graph *g_R, Graph *sTree_gamma_prime,set<int> *V_q_prime, set<int> * VR_leaf_prime, set<int> * V_q)
{
	cout << "V_q_prime:\n";
	print(*V_q_prime);
	(*V_q) = (*g_Q).getVertex(All);
	cout << "V_q:\n";
	print(*V_q);
	
	set<int> V_union;
	set_union((*V_q_prime).begin(), (*V_q_prime).end(), (*VR_leaf_prime).begin(), (*VR_leaf_prime).end(), insert_iterator< set<int> >(V_union, V_union.end()));
	
	cout << "V_union:\n";
	print(V_union);
	set<int> V_sub;
	set_difference((*V_q).begin(), (*V_q).end(), V_union.begin(), V_union.end(), insert_iterator< set<int> >(V_sub, V_sub.end()));
	cout << "V_sub:\n";
	print(V_sub);
	cout << "sTree_gamma_prime before 7:\n";
	(*sTree_gamma_prime).print();
	set<int>::iterator setIt;
	for (setIt = V_sub.begin(); setIt != V_sub.end(); setIt++)
	{
		if (!(*g_R).ExistsEdge(*setIt, (*g_Q).Gamma))
		{
			(*sTree_gamma_prime).addEdge(*setIt, (*g_Q).Gamma);
		}


	}
	cout << "sTree_gamma_prime after 7:\n";
	(*sTree_gamma_prime).print();
	cout << "*********End of 7*********\n";

}
int do_step8(Graph * g_R,int Gamma,set<int> V_q,set<int> sTree_gamma_prime_leaves,Graph *sTree_gamma_ze, set<int> *sTree_gamma_ze_leaves)
{
	set<int>::iterator rIt,setIt;
	for (setIt = V_q.begin(); setIt != V_q.end(); setIt++)
	{
		if ((*g_R).ExistsEdge(*setIt, Gamma))
		{
			if (sTree_gamma_prime_leaves.size() > 0)
			{
				int r = rand() % (distance(sTree_gamma_prime_leaves.begin(), sTree_gamma_prime_leaves.end()));
				rIt = next(sTree_gamma_prime_leaves.begin(), r);
				(*sTree_gamma_ze).addEdge(*setIt, *rIt);
			}
		}

	}
	(*sTree_gamma_ze).addEdge(Gamma, Gamma);
	cout << "sTree_gamma_ze:\n";
	(*sTree_gamma_ze).print();
	
	Graph g_R_cop((*g_R).V);
	(*g_R).copyTo(&g_R_cop);
	removeEdgesInNoCyc(g_R_cop.V, 0, g_R_cop.outAdj, g_R_cop.inAdj);
	removeEdgesInNoCyc(g_R_cop.V, 0, g_R_cop.inAdj, g_R_cop.outAdj);
	cout << "g_R after remove edges that are in no cycles:\n";
	g_R_cop.print();
	set<int> v_r_inCycle = g_R_cop.getVertex(None);
	cout << "V_R that is in a cycle:\n";
	print(v_r_inCycle);
	(*sTree_gamma_ze_leaves) = (*sTree_gamma_ze).getVertex(source);
	cout << "sTree_gamma_ze_leaves:\n";
	print(*sTree_gamma_ze_leaves);
	set<int> v_intersect;
	set_intersection((*sTree_gamma_ze_leaves).begin(), (*sTree_gamma_ze_leaves).end(), v_r_inCycle.begin(), v_r_inCycle.end(), insert_iterator< set<int> >(v_intersect, v_intersect.end()));
	cout << "v_r_inCycle_intersect_sTree_gamma_ze_leaves:\n";
	print(v_intersect);

	if (v_intersect.size() == 0)
	{
		cout << "sTree_gamma_ze has no leaves in common with any cycle in g_R, Go to step2:\n ";
		return -1;//Go to step 2
	}
	else
	{
		return 0;
	}

}
enum basedOnGraph
{
	_g_Q, _g_R, _both
};
void LabelArcs(basedOnGraph base, Graph g_toSerach, set<int> vertex, Graph g_Q, Graph g_R, set<std::tuple<int, int, int>> *tupl)
{
	set<int>::iterator vertexSetIt;
	list<int>::iterator listIt;
	set<int>::iterator rIt;
	set<int>::iterator subIt;

	if (base == _both)
	{
		cout << "step 9: LabelArcs!\n";
	}
	else
		cout << "step 10: LabelArcs!\n";
	cout << "g_toSerach:\n";
	g_toSerach.print();
	cout << "label the outgoing arcs of the vertexes:\n";
	print(vertex);
	cout << "G_R:\n";
	g_R.print();
	cout << "G_Q:\n";
	g_Q.print();
	000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/

	set<int> V_sub;
	set < int > g_r_destVertex;
	set<int> g_q_destVertex;
	set<int> g_toSerachVertex;
	int r = 0;
	
	for (vertexSetIt = vertex.begin(); vertexSetIt != vertex.end(); vertexSetIt++)
	{
		cout << "label outgoing arcs of the vertex, "<<(*vertexSetIt)<<endl;
		for (listIt = g_toSerach.outAdj[*vertexSetIt].begin(); listIt != g_toSerach.outAdj[*vertexSetIt].end(); listIt++)
		{
			cout << "select the arc: " << (*vertexSetIt) << " --> " << (*listIt)<<endl;
			g_q_destVertex = g_Q.getVertex(*vertexSetIt);
			cout << "the outgoing arcs of vertex, " << (*vertexSetIt) << " in g_Q:\n";
			print(g_q_destVertex);
			if (base == _both)
			{
				g_r_destVertex = g_R.getVertex(*vertexSetIt);
				cout << "the outgoing arcs of vertex, " << (*vertexSetIt) << " in g_R:\n";
				print(g_r_destVertex);
				V_sub.clear();
				set_difference(g_r_destVertex.begin(), g_r_destVertex.end(), g_q_destVertex.begin(), g_q_destVertex.end(), insert_iterator< set<int> >(V_sub, V_sub.end()));
				cout << "v_sub(g_r_destVertex,g_r_destVertex): \n";
				print(V_sub);
			}
			else if (base == _g_Q)
			{
				V_sub.clear();
				g_toSerachVertex=g_toSerach.getVertex(All);
				set_difference(g_toSerachVertex.begin(), g_toSerachVertex.end(), g_q_destVertex.begin(), g_q_destVertex.end(), insert_iterator< set<int> >(V_sub, V_sub.end()));
				cout << "v_sub(g_toSerachVertex,g_q_destVertex): \n";
				print(V_sub);
			}
			V_sub.erase((*listIt));
			cout << "Erase " << (*listIt) << " from v_sub, v_sub_new:\n";
			print(V_sub);
			if (V_sub.size() > 0)
			{
				for (subIt = V_sub.begin();subIt != V_sub.end();subIt++)
				{
					cout << "Add ( " << (*vertexSetIt) << " , " << (*subIt) << " , " << (*listIt) << " ) to tupl set!\n";
					(*tupl).insert(make_tuple((*vertexSetIt), (*subIt), (*listIt)));
				}
				/*r = rand() % (distance(V_sub.begin(), V_sub.end()));
				rIt = next(V_sub.begin(), r);
				cout << "Add ( " << (*vertexSetIt) << " , " << (*rIt) << " , " << (*listIt) << " ) to tupl set!\n";
				(*tupl).insert(make_tuple((*vertexSetIt), (*rIt), (*listIt)));*/
			}
			else
			{
				cout << "ther is no item in v_sub!\n";
				if (base == _both && g_r_destVertex.size() == 0)
				{
					cout << "g_R: " << (*vertexSetIt) << " ---> \n";
					cout << "There is no arc with " << (*vertexSetIt) << " source in g_R!!!\n";
					cout << "So, " << (*vertexSetIt) << " --> " << (*listIt) << " has no label!!!\n";
					V_sub.clear();
					g_toSerachVertex = g_toSerach.getVertex(All);
					set_difference(g_toSerachVertex.begin(), g_toSerachVertex.end(), g_q_destVertex.begin(), g_q_destVertex.end(), insert_iterator< set<int> >(V_sub, V_sub.end()));
					cout << "v_sub(g_toSerachVertex,g_q_destVertex): \n";
					print(V_sub);
					V_sub.erase((*listIt));
					if (V_sub.size() > 0)
					{
						for (subIt = V_sub.begin();subIt != V_sub.end();subIt++)
						{
							cout << "Add ( " << (*vertexSetIt) << " , " << (*subIt) << " , " << (*listIt) << " ) to tupl set!\n";
							(*tupl).insert(make_tuple((*vertexSetIt), (*subIt), (*listIt)));
						}
						/*r = rand() % (distance(V_sub.begin(), V_sub.end()));
						rIt = next(V_sub.begin(), r);
						cout << "Add ( " << (*vertexSetIt) << " , " << (*rIt) << " , " << (*listIt) << " ) to tupl set!\n";
						(*tupl).insert(make_tuple((*vertexSetIt), (*rIt), (*listIt)));*/
					}
				}

					
			

			}
			}
			
		}
}

void do_step9(Graph sTree_gamma_ze,set<int> sTree_gamma_ze_leaves,Graph g_Q,Graph g_R ,set<std::tuple<int, int, int>> *tupl)
{
	LabelArcs(_both, sTree_gamma_ze, sTree_gamma_ze_leaves, g_Q, g_R, tupl);

}

void do_step10(Graph sTree_gamma_ze, set<int> sTree_gamma_ze_leaves, Graph g_Q, Graph g_R, set<std::tuple<int, int, int>> *tupl)
{
	/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
	cout << "step 10: sTree_gamma_ze before remove leaf outgoing Edges:\n";
	000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
	(sTree_gamma_ze).print();
	sTree_gamma_ze.removeEdges(sTree_gamma_ze_leaves);
	/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
	cout << "step 10: sTree_gamma_ze after remove leaf outgoing Edges:\n";
	000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
	(sTree_gamma_ze).print();
	set<int> otherVertex=sTree_gamma_ze.getVertex(no_isolated);
	LabelArcs(_g_Q, sTree_gamma_ze, otherVertex, g_Q, g_R, tupl);
}
bool compareTuple(set<std::tuple<int, int, int>>::iterator tupIt1,set<std::tuple<int, int, int>>::iterator tupIt2)
{
	return (get<0>(*tupIt1) <= get<0>(*tupIt2)) && (get<2>(*tupIt1) <= get<2>(*tupIt2));
}
enum varType
{
	pre,
	current
};

void decodeVar(int dSize,int val)
{
	//val++;
	//fOutLeadsTo << "val= " << val << endl;
	//fOutLeadsTo.open("synLeadToOut.txt");
	fOutLeadsTo << (dSize-(val+1) > 0 ? std::string(dSize-(val+1), '0') : "") << "1" << (dSize -((dSize-(val+1))+1) > 0 ? std::string(dSize -((dSize-(val+1))+1), '0') : "");
	//fOutLeadsTo.close();
}
void convertAction(varType vType, int tVal,string eq,string del,string begin_sr,string end_sr,bool cons)
{
	list<int>::iterator dSizesIt;
	list<string>::iterator namesIt;
	namesIt = varsNames.begin();
	int M = 0;
	for (dSizesIt = domainsSizes.begin(); dSizesIt != domainsSizes.end(); dSizesIt++)
	{
		int preVarsSep = pow(2.0, M);
		int varSep = pow(2.0, ((floor(log2((*dSizesIt) - 1)) + 1)));
		int varVal = (preVarsSep > 1 ? ((tVal / preVarsSep) % varSep) : (tVal%varSep));
		
		string name = (*namesIt);
		switch (vType)
		{
		 case pre:replaceAll(name, "_i", "[dec(i)]"); break;
		 case current: replaceAll(name, "_i", "[i]"); 
		 decodeVar((*dSizesIt),varVal); 
		 if (next(dSizesIt,1) != domainsSizes.end())
		 {
			fOutLeadsTo << "|";
		 }
		 else
			fOutLeadsTo << "|1"<<endl;

		 break;
		}
		if(cons)
		{
			 cout << begin_sr << name << eq << varVal <<end_sr;
			if (next(dSizesIt,1) != domainsSizes.end())
			{
				cout << del;
			
			}
		}
			
		M += (floor(log2((*dSizesIt) - 1)) + 1);
		namesIt++;
	}
	
	
	
}
void parseDecoded()
{
	finDecoded.open("fOutDecoded.txt");
	string encodedStr;
	string s;
	for (int i = 1; i <= 2; i++)
          std::getline(finDecoded, s);
	//stdout = fdopen(0, "w");

	std::getline(finDecoded, s);
	list<string>::iterator namesIt;
	list<int>::iterator dSizesIt;
	string varCond;
	size_t n0,n1;
	char on_set;
	int c,i;
	bool firstVar;
	int lineNum = 3;
	string varName ;
	while(s != ".e")
	{
		stringstream ss(s);
		ss >> varCond; //1
		dSizesIt = domainsSizes.begin();
		i=0;
		for(namesIt = varsNames.begin();namesIt != varsNames.end();namesIt++)
		{

			ss >> varCond;
			//////////////////////////////////////////////////cout << varCond << endl;
			n0 = std::count(varCond.begin(), varCond.end(), '0');
			n1 = std::count(varCond.begin(), varCond.end(), '1');
			if(n0 < n1) on_set='0';
			else on_set = '1';
			//////////////////////////////////////////////////cout << " n0 = " << n0 << endl;
			//////////////////////////////////////////////////cout << " n1 = " << n1 << endl;
			//////////////////////////////////////////////////cout << "var_dSize=" << (*dSizesIt) <<endl;
			if(n0 != (*dSizesIt) && n1 != (*dSizesIt))
			{ 
				i++;
				encodedStr = encodedStr + (encodedStr == "" ? " " : (i==1 ? " )) ||  " : ") && "));
				firstVar = false;
				c=1;
				while(c <= varCond.length())
				{

				  if(varCond.at(c-1) == on_set)
				  {
					varName=*namesIt;
					replaceAll(varName, "_i", "[i] ");
					encodedStr = encodedStr + (firstVar ? (encodedStr.at(encodedStr.length()-3)=='=' ? " || " : " && ") : (i==1 ? " (( " : " ( ") );
					encodedStr = encodedStr + varName  + std::string((on_set=='1' ? "==" : "!=")) + std::to_string(((*dSizesIt)-c));
					firstVar = (!firstVar ? true : firstVar);
				   }
				   c++;
				}
			}
			dSizesIt++;
		}
		std::getline(finDecoded, s);
	}
	cout <<encodedStr + ")";
	finDecoded.close();

}
void do_step11(set<std::tuple<int, int, int>> *tupl)
{
	set<std::tuple<int, int, int>>::iterator tupIt;
	set<std::tuple<int, int, int>>::iterator tupIt1;
	/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
	cout << "Synthesized parameterized actions : \n";
	fOutLeadsTo.open("synLeadToOut.txt");
	000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/

	streambuf* stream_buffer_cout;
	streambuf* stream_buffer_cin;
	streambuf* stream_buffer_file;
	string line; 
	int stdout_dupfd;
    FILE *temp_out;
fstream file; 
int outdes;
//fout.close();

	int tupleSize = distance((*tupl).begin(), (*tupl).end());
	bool print[4000];
	/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
	fOutLeadsTo<<".mv "<< distance(domainsSizes.begin(),domainsSizes.end())+2 << " 1 ";
	000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
	list<int>::iterator dSizesIt;
	list<string>::iterator namesIt;
	namesIt = varsNames.begin();
	int M = 0;
	/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
	for (dSizesIt = domainsSizes.begin(); dSizesIt != domainsSizes.end(); dSizesIt++)
	{
		fOutLeadsTo<< (*dSizesIt)<< " ";
	}
	fOutLeadsTo<<"1"<<endl<<"1|";
	000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
	//fOutLeadsTo.close();
	
	for (int i = 0; i < 4000;i++)
			{
				print[i] = false;
			}

	for (tupIt = (*tupl).begin(); tupIt != (*tupl).end(); tupIt++)
	{
		//string act = "";
		if (print[distance((*tupl).begin(), tupIt)] == false)
		{
			print[distance((*tupl).begin(), tupIt)] = true;
			//++ cout << ":: atomic{( ";
			//++ convertAction(pre, get<0>(*tupIt)," == ", " && ","(",")");
			//++ cout << ") &&  (( ";
			//convertAction(current, get<1>(*tupIt));
			/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
                         convertAction(current, get<1>(*tupIt)," == ", " && ","(",")",false);
			000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
			//++ cout << " )";
			//act= "Xi-1 = " + std::to_string(get<0>(*tupIt)) + " ^ (Xi = " + std::to_string(get<1>(*tupIt));
			for (tupIt1 = next(tupIt, 1); tupIt1 != (*tupl).end(); tupIt1++)
			{
				if (get<0>(*tupIt1) == get<0>(*tupIt) && get<2>(*tupIt1) == get<2>(*tupIt))
				{
					//++ cout << " || ( ";
					/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
					fOutLeadsTo << "1|";
					
					//convertAction(current, get<1>(*tupIt1));
					 convertAction(current, get<1>(*tupIt1)," == ", " && ","(",")",false);
					000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
					//++ cout << " )";
					
					//act += " v Xi = " + std::to_string(get<1>(*tupIt1));
					print[distance((*tupl).begin(), tupIt1)] = true;
				}
			}
			//++ cout << " ) -> ";
			/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
			fOutLeadsTo.close();
			
			convertAction(current, get<2>(*tupIt)," = ", " ; ","","",false);
			000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
			//convertAction(current, get<2>(*tupIt));
			//++cout << " ;}\n";
			//outdes = dup( 1 );

			
			///**if(get<0>(*tupIt)==3)
			///**{
			///**fOutLeadsTo.open("synLeadToOut.txt", std::ofstream::out | std::ofstream::trunc);
			///**fOutLeadsTo<<".mv "<< distance(domainsSizes.begin(),domainsSizes.end())+2 << " 1 ";
			///**for (dSizesIt = domainsSizes.begin(); dSizesIt != domainsSizes.end(); dSizesIt++)
			///**{
				///**fOutLeadsTo<< (*dSizesIt)<< " ";
			///**}
			///**fOutLeadsTo<<"1"<<endl<<"1|";
			///**}


			
			
			///**if(get<0>(*tupIt)==4 )
   			///**{
			/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
				freopen("fOutDecoded.txt","w",stdout);
				system("./espresso synLeadToOut.txt");
				freopen("/dev/tty", "w", stdout);
			
				//++ cout << "\n\nMinimization output: -->> :: atomic{( ";
				cout << "\n:: atomic{( ";
				convertAction(pre, get<0>(*tupIt)," == ", " && ","(",")",true);
				cout << ") &&  ( ";
			
				parseDecoded();
				cout << " )) -> ";
				convertAction(current, get<2>(*tupIt)," = ", " ; ","","",true);
				cout << " ;}\n";

				fOutLeadsTo.open("synLeadToOut.txt", std::ofstream::out | std::ofstream::trunc);
				fOutLeadsTo<<".mv "<< distance(domainsSizes.begin(),domainsSizes.end())+2 << " 1 ";
				for (dSizesIt = domainsSizes.begin(); dSizesIt != domainsSizes.end(); dSizesIt++)
				{
					fOutLeadsTo<< (*dSizesIt)<< " ";
				}
				fOutLeadsTo<<"1"<<endl<<"1|";
			000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
			///**}
			
			
		}
		

	}
	

	
}

void synLeadsTo(string r_predicate, string q_predicate,int M)
{
	struct timeval tstart, tstop;
	Graph g_R(M);
	Graph g_Q(M);
	gettimeofday(&tstart, NULL);
	gettimeofday(&start, NULL);
	do_step1(r_predicate, &g_R, q_predicate, &g_Q);//create GR and GQ graphs based on related predicates
	gettimeofday(&stop, NULL);

	cout << "step1 (Time) --->> " << (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec << endl;
	//return;
	cout << "g_R:\n";
	g_R.print();
	cout << "g_Q:\n";
	g_Q.print();
//return;
	gettimeofday(&start, NULL);
	int Gamma = do_step2(&g_Q,false);
	gettimeofday(&stop, NULL);
	cout << "step2 (Time) --->> " << (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec << endl;
	while (Gamma>-1)
	{
		Graph g_Q_prime(g_Q.V);
		set<int> V_q_prime;
		cout<<"Gamma="<<Gamma<<endl;
		//return;
		gettimeofday(&start, NULL);
		do_step3(&g_Q, Gamma, &g_Q_prime,&V_q_prime);
		gettimeofday(&stop, NULL);
		cout << "step3 (Time) --->> " << (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec << endl;
		//return;
		Graph sTree_gamma(g_Q_prime.V);
		gettimeofday(&start, NULL);
		do_step4(&g_Q_prime, Gamma, &sTree_gamma);
		gettimeofday(&stop, NULL);
		cout << "step4 (Time) --->> " << (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec << endl;
		set<int> V_R_prime;
		gettimeofday(&start, NULL);
		do_step5(&g_R, &V_R_prime);
		gettimeofday(&stop, NULL);
		cout << "step5 (Time) --->> " << (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec << endl;
		Graph sTree_gamma_prime(sTree_gamma.V);
		set<int>  VR_leaf_prime;
		gettimeofday(&start, NULL);
		do_step6(&sTree_gamma, &V_R_prime, &sTree_gamma_prime,&VR_leaf_prime,Gamma);
		gettimeofday(&stop, NULL);
		cout << "step6 (Time) --->> " << (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec << endl;
		set<int> V_q;
		gettimeofday(&start, NULL);
		do_step7(&g_Q, &g_R, &sTree_gamma_prime, &V_q_prime, &VR_leaf_prime,&V_q);
		gettimeofday(&stop, NULL);
		cout << "step7 (Time) --->> " << (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec << endl;
		Graph sTree_gamma_ze(sTree_gamma_prime.V);
		sTree_gamma_prime.copyTo(&sTree_gamma_ze);
		set<int> sTree_gamma_prime_leaves;
		if (V_q_prime.size() == 1 && (*V_q_prime.begin()) == Gamma)
		{
			
				sTree_gamma_prime_leaves = sTree_gamma_prime.getVertex(source_restrict);
		}
		else
		{
			sTree_gamma_prime_leaves = sTree_gamma_prime.getVertex(source);
		}
		set<int> sTree_gamma_ze_leaves;
		gettimeofday(&start, NULL);
		int step8_output=do_step8(&g_R, Gamma, V_q, sTree_gamma_prime_leaves, &sTree_gamma_ze, &sTree_gamma_ze_leaves);
		gettimeofday(&stop, NULL);
		cout << "step8 (Time) --->> " << (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec << endl;
		if (step8_output == -1)
		{
			Gamma = do_step2(&g_Q, true);
			continue;
		}
		set<std::tuple<int, int, int>> tupl;
		gettimeofday(&start, NULL);
		do_step9(sTree_gamma_ze, sTree_gamma_ze_leaves,g_Q,g_R,&tupl);
		gettimeofday(&stop, NULL);
		cout << "step9 (Time) --->> " << (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec << endl;

		gettimeofday(&start, NULL);
		do_step10(sTree_gamma_ze, sTree_gamma_ze_leaves, g_Q, g_R, &tupl);
		gettimeofday(&stop, NULL);
		cout << "step10 (Time) --->> " << (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec << endl;


		gettimeofday(&start, NULL);
		do_step11(&tupl);
		gettimeofday(&stop, NULL);

		cout << "step11 (Time) --->> " << (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec << endl;
		
		break;
	}
	if (Gamma == -1)
	{
		cout << "No solution exists\n";
	}
	gettimeofday(&tstop, NULL);
	cout << "Total (Time) --->> " << (tstop.tv_sec - tstart.tv_sec) * 1000000 + tstop.tv_usec - tstart.tv_usec << endl;

}
string convertBooleanToCStyle(string booleanStr)
{
	list<string>::iterator it,it1;
	it1 = varsNames.begin();
	for (it = varTrans.begin(); it != varTrans.end(); it++)
	{
		replaceAll(booleanStr, (*it1), (*it));
		string tranVarRep = *it;
		string varRep = *it1;
		replaceAll(tranVarRep, "curr", "pre");
		replaceAll(varRep, "i", "{i-1}");
		replaceAll(booleanStr, varRep, tranVarRep);
		replaceAll(booleanStr, "and", "&&");
		replaceAll(booleanStr, "or", "||");
		it1++;
	}
	return booleanStr;
}
//struct tupleComp
//{
    //bool operator()(const std::tuple<int, int, int> & lhs, const std::tuple<int, int, int> & rhs) const
    //{
        //return get<1>(lhs) == get<1>(rhs);
    //}
//};
void print(set<std::tuple<int, int, int>> *tupl)
{
	set<std::tuple<int, int, int>>::iterator tupIt;
	for (tupIt = (*tupl).begin(); tupIt != (*tupl).end(); tupIt++)

	{
		//if(get<1>(*tupIt) != -1)
			cout << "(" << get<0>(*tupIt) << ","<< get<1>(*tupIt) << "," << get<2>(*tupIt)<< ")"<<endl;
	}
}
typedef void* (*arbitrary1)(set<std::tuple<int, int, int>> *,list<int> );
void FillTuple(/*string action_dec,string action_gaurd,string action_update,*/ set<std::tuple<int, int, int>> *tupl ,list<int> ValidVal,string fileName)
{
	//arbitrary1 FillTuple;
	// create own program
	ofstream f;
	arbitrary1 FillTuple;
	string formatedStr = "#include <iostream>\n #include <sstream>\n #include <fstream>\n #include<set>\n #include<stdlib.h>\n #include<list>\n #include<tuple>\n #include<string>\n using namespace std;\n"
		"extern \"C\" void FillTuple(set<std::tuple<int, int, int>> *tupl, list<int> ValidVal){\n"
		"    int pre_x= 0;\n"
		"    int curr_x=0;\n"
		"    set<std::tuple<int, int, int>>::iterator tupIt;\n"
		"     list<int>::iterator it;\n"
		"    for (tupIt = (*tupl).begin(); tupIt != (*tupl).end(); tupIt++)\n"
		"    {\n"
		"	pre_x= get<0>(*tupIt);\n"
		"	curr_x= get<2>(*tupIt);\n"
		"	//cout<<\"pre_x=\" << pre_x<<endl;\n"
		"	//cout<<\"guard=\" << get<1>(*tupIt)<<endl;\n"
		"	//cout<<\"curr_x=\" << curr_x<<endl;\n"
		"	cout <<  \"%s\" << endl << \"%s\" << endl;\n"
		"	if (get<1>(*tupIt) == -1 && (%s) && (%s))\n"
		"	{\n"
		"		cout<<\"1111111\";\n"
		"		for(it = ValidVal.begin(); it!= ValidVal.end(); it++)\n"
		"		{\n"
		"			curr_x=(*it);\n"
		"     			if(%s)\n"
		"     			{\n"
		"       			//get<1>(*tupIt) = curr_x;\n"
		"				cout << \"tupIt = (\" <<get<0>(*tupIt) << \",\" << get<1>(*tupIt) << \",\"<< get<2>(*tupIt) << \")\" <<endl;\n"
		"				std::tuple<int, int, int> cur(get<0>(*tupIt),curr_x,get<2>(*tupIt));\n"
		"				if(get<1>(*tupIt) == -1) (*tupl).erase(*tupIt);\n"
		"				(*tupl).insert(cur);\n"
		"				cout << \"tupIt = (\" <<get<0>(*tupIt) << \",\" << get<1>(*tupIt) << \",\"<< get<2>(*tupIt) << \")\" <<endl;\n"
		"				//break;\n"	
		"     			}\n"
		"		}\n"
                "     	 }\n"
		"   }\n"
                "}\n";
		string actionStr, action_gaurd, action_update, action_dec;
		int arrowPos, semiPos;
		ifstream fInActions;
		fInActions.open(fileName+".txt");
		print(tupl);
		string s;
		while(std::getline(fInActions,actionStr))
		{
			s = formatedStr;
			//cout << actionStr<<endl;\n"
			actionStr=convertBooleanToCStyle(actionStr);
				//cout << actionStr<<endl;\n"
			semiPos = actionStr.find(";");
			action_dec=actionStr.substr (0,semiPos);
			arrowPos = actionStr.find("->");
			action_gaurd = actionStr.substr (semiPos+1,arrowPos-semiPos-1);
			if(action_gaurd == " ") action_gaurd = "true";
			if(action_dec == "") action_dec = "true";
			action_update = actionStr.substr (arrowPos+2,actionStr.length()-2);
			cout<<"action_dec: "<< action_dec<<endl<<"action_gaurd: " << action_gaurd<<endl<<"action_update: "<<action_update<<endl;
			//FillTuple(action_dec,action_gaurd,action_update,&tupl, ValidVal);
		f.open("tmp.cpp");  
		
		f <<format(s.c_str(), action_dec.c_str(),action_update.c_str(), action_dec.c_str(),action_update.c_str(),action_gaurd.c_str());
		f.close();
		// create library
		remove("libtmp.so");

		system("g++ -shared graph.cpp tmp.cpp -o libtmp.so");
		//remove("tmp.cpp");
		//remove("libtmp.so");
		// load library        
		void * fLib  = dlopen("./libtmp.so", RTLD_LAZY);
		if (!fLib) {
			cerr << "Cannot open library: " << dlerror() << '\n';
		}
		if (fLib) {
			
			*(void**)(&FillTuple) = dlsym(fLib, "FillTuple");
			if (FillTuple) {
				// use function
				(void)FillTuple(tupl,ValidVal);
			}
		}
		print(tupl);
		cout<<"finish--> " << " action_dec: "<< action_dec<<endl<<"action_gaurd: " << action_gaurd<<endl<<"action_update: "<<action_update<<endl;
		dlclose(fLib);
	}
}
int get0_pred=-2;
int get1_pred=-2;
bool uniPred(std::tuple<int, int, int> t)
{
	return (get0_pred == -2 ? true : get<0>(t) == get0_pred) && (get1_pred == -2 ? true : get<1>(t) == get1_pred);

}
set<std::tuple<int, int, int> > tupl;
void verifier(int M,string fileName)
{
	std::size_t arrowPos,semiPos;
	std::string action_dec,action_gaurd,action_update;

	 //0000000000000
	list<int> ValidVal;

	//0000000000000
	cout << 1;
	for(int i = 0; i<M; i++)
		if(isValidVertex(i))
		{
			cout << "valid vertex! -->" << i << endl;
			ValidVal.push_back(i);
			for(int j = 0; j<M; j++)
				if(isValidVertex(j))
				{
					cout << "valid vertex! -->" << j << endl;
					tupl.insert(make_tuple(i, -1, j));
				}
		}
	
	

	print(&tupl);
	
	cout << 2;
	Graph g_V(M);
	cout << "g_V:\n";
	g_V.print();
	string actionStr;
	FillTuple(&tupl, ValidVal,fileName);
	set<std::tuple<int, int, int>>::iterator tupIt;
	set<std::tuple<int, int, int>>::iterator tupIt1;
	set<std::tuple<int, int, int>>::iterator tupIt2;
	print(&tupl);
	for (tupIt2 = (tupl).begin(); tupIt2 != (tupl).end(); tupIt2++)
	{
		if(get<1>(*tupIt2) == -1) 
			(tupl).erase(*tupIt2);
	}
	
print(&tupl);
	bool isDeadlock = false;
	for (tupIt = (tupl).begin(); tupIt != (tupl).end(); tupIt++)
	{
		
		for (tupIt1 = (tupl).begin(); tupIt1 != (tupl).end(); tupIt1++)	
			if(get<1>(*tupIt1) != -1 && get<1>(*tupIt) != -1)
			
				if(get<0>(*tupIt) == get<1>(*tupIt1) and (get<0>(*tupIt) != get<0>(*tupIt1) and get<1>(*tupIt) != get<1>(*tupIt1) and get<2>(*tupIt) != get<2>(*tupIt1)))
				{
					get0_pred = get<2>(*tupIt1);
					get1_pred = get<1>(*tupIt);
					if(count_if(tupl.begin(),tupl.end(),uniPred) == 0)
					{
						if(!isDeadlock) cout << "Deadlock!"<<endl;
						cout<< "(" << get<0>(*tupIt) << "," << get<1>(*tupIt) << "," << get<2>(*tupIt) << ") --> " << "(" << get<0>(*tupIt1) << "," << get<1>(*tupIt1) << "," << get<2>(*tupIt1) << ")\n";
						isDeadlock = true;
						//break;
					}
				}
		//if(isDeadlock)
			//break;
			//cout << "("<< get<0>(*tupIt) << ","<< get<1>(*tupIt) << "," << get<2>(*tupIt) << ") : " << count_if(tupl.begin(),tupl.end(),uniPred)<<endl;

	}
	if(!isDeadlock)
		cout << "Deadlock free!"<<endl;

	
}

// Driver code 

int main()
{
	
	
	//list<int> domainsSizes;
	string name;
	string varRep;
	int domainSize;
	int M=0, num;
	double preVarsSep, varSep;
	int opt=0;
	string fileName;
	while(opt != -1)
	{
		cout << "Please enter the number of variables:\n";
		cout << "The number of variables=";
		cin >> num;
		cout << "Please enter the name and domain size of the variables:\n";
		for (int i = 0; i < num; i++)
		{
			cout << "The name of the variable" << (i == 0 ? " 1st" : (i == 1 ? " 2nd" : (i == 2 ? " 3rd" : (i + 1) + " th"))) << " is : ";
			cin >> name;
			varsNames.push_back(name);
			cout << "The domain size of the variable" << (i == 0 ? " 1st" : (i == 1 ? " 2nd" : (i == 2 ? " 3rd" : (i + 1) + " th"))) << " is : ";
			cin >> domainSize;
			domainsSizes.push_back(domainSize);
		
			preVarsSep = pow(2.0, M);
		
			varSep = pow(2.0, floor(log2(domainSize - 1)) + 1);
			varRep = preVarsSep > 1 ? "((curr_x/" + to_string((int)preVarsSep) + ")%" + to_string((int)varSep) + ")" :"(curr_x%"+ to_string((int)varSep)+")";
			varTrans.push_back(varRep);
			M += (floor(log2(domainSize-1))+1);
		}
		M = pow(2, M );
		cout << "\nM=" << M <<endl;
		list<string>::iterator it,it1;
		list<int>::iterator it2;
		it1 = varsNames.begin();
		it2 = domainsSizes.begin();
		for (it = varTrans.begin(); it != varTrans.end(); it++)
		{
			cout << *it1 << "( " << *it2 <<") : " << *it << endl;
			it1++;
			it2++;
		}
		/*cout << "Please enter domain size of xi(M):\n";
		cout << "M=";
		cin >> M;*/
		std::string r_predicate, q_predicate;
		cout << "Please enter r:\n";
		cout << "r=";
		cin.ignore();
		std::getline(std::cin, r_predicate);
		cout << "Please enter q:\n";
		cout << "q=";
		std::getline(std::cin, q_predicate);
		//it1 = varsNames.begin();
		//for (it = varTrans.begin(); it != varTrans.end(); it++)
		//{
			//replaceAll(q_predicate, (*it1), (*it));
			//replaceAll(r_predicate, (*it1), (*it));
			//string tranVarRep = *it;
			//string varRep = *it1;
			//replaceAll(tranVarRep, "curr", "pre");
			//replaceAll(varRep, "i", "{i-1}");
			//replaceAll(r_predicate, varRep, tranVarRep);
			//replaceAll(q_predicate, varRep, tranVarRep);
			//replaceAll(r_predicate, "and", "&&");
			//replaceAll(r_predicate, "or", "||");
			//replaceAll(q_predicate, "and", "&&");
			//replaceAll(q_predicate, "or", "||");
			//it1++;
		//}
		q_predicate=convertBooleanToCStyle(q_predicate);
		r_predicate=convertBooleanToCStyle(r_predicate);
		cout << "r: " << r_predicate << endl;
		cout << "q: " << q_predicate << endl;
		it1 = varsNames.begin();
		it2 = domainsSizes.begin();
		for (it = varTrans.begin(); it != varTrans.end(); it++)
		{
			cout << *it1 << "( " << *it2 << ") : " << *it << endl;
			it1++;
			it2++;
		}
	
			cout << "\nPlease choose an option to continue by entering 1 or 2 \n" << "1.synLeadsTo\n" << "2.Verifier\n"<<"-1:Exit\n";
	
			cin>>opt;
			if(opt == 1)
				synLeadsTo(r_predicate, q_predicate, M);
			else if(opt == 2)
			{
				cout << "\nPlease enter actions file name (with out extension):\nActions file name: ";
				cin.ignore();
				std::getline(std::cin, fileName);
				verifier(M,fileName);
			}
	}

	system("Pause");
	return 0;
	
}
