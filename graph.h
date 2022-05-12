#ifndef _GRAPH_H_
#define _GRAPH_H_

#include<string>

using namespace std;

vector<string> split(const string &str, const string &pattern);

struct EdgeNode {                                       
	char adjVex;                                        
	EdgeNode* next;                                     
	map<string,int> weight;                             

	EdgeNode(char data) {
		adjVex = data;
		next = NULL;
		//weight = -1;
	}
};

struct VertexNode {  
	char data;       
	int  type;
	EdgeNode* first; 

	map<string,int> value;
	VertexNode() {
		first = NULL;
	}

	bool isConflict(std::map<string,int> v)
	{
		return true;
	}
};

class Graph 
{
	int VexNum, ArcNum;  
	int** AM_Edge;  
	char* AM_Node;  
	VertexNode* AL_Node; 
	bool *visited;
	string graphName;
	map<string,string> weightMap2;
public:
	
	std::vector<VertexNode> fullPathVect;
public:
	Graph(); 
	~Graph(); 
	void AM_GraphInitial() ;
	int AM_locate(char vex) ;
	void AM_GraphPrint() ;
	void AM_degree(char vex,int &d1,int &d2) ;
	void AM_BFS(int startLoc) ;
	void AM_DFS(int startLoc) ;
	void AM_RecursiveDFS(int startLoc) ;
	void AM_StackDFS(int startLoc) ;
	void AM_FindPath(int i,int j) ;
	bool AM_RecursiveDFS(int startLoc,int endLoc,stack<int> &p) ;
	void AL_GraphInitial(string vexFile,string edgeFile,/*string weightFile,*/int paternIndex) ;
	int AL_Locate(char vex) ;
	void AL_GraphPrint() ;
	void AL_degree(char vex,int &s1,int &s2);
    void AL_BFS(int startLoc) ;
	void AL_DFS(int startLoc);
	void AL_RecursiveDFS(int startLoc);
	void AL_StackDFS(int startLoc) ;
	void AL_FindPath(int i, int j) ;
	void AL_FindPath(string name,int i, int j) ;
	bool AL_RecursiveDFS(int startLoc,int endLoc, stack<int> &p) ;
	void InitialVisited() ;
};
#endif