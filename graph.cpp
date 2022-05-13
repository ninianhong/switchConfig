/*图的存储和相关算法*/
#pragma once
#include<iostream>
#include<string>
#include<iomanip>
#include<Queue>
#include<Stack>
#include<fstream>
#include <map> 


using namespace std;
/*使用邻接矩阵时无需声明结构体，使用邻接表时需要声明顶点和边结点的结构体*/

//邻接表顶点和边结点的结构体
struct EdgeNode {                                       //边结点结构
	char adjVex;                                        //该边结点指向的顶点
	EdgeNode* next;                                     //指向的下一边结点
	map<string,int> weight;                             //存储每个节点对边要求

	EdgeNode(char data) {
		adjVex = data;
		next = NULL;
		//weight = -1;
	}
};

struct VertexNode {  //顶点结构
	char data;       //顶点信息
	int  type;
	EdgeNode* first; //指向第一条依附该顶点的边的指针

	map<string,int> value;
	VertexNode() {
		first = NULL;
	}

	bool isConflict(std::map<string,int> v)
	{
		return true;
	}
};



extern vector<string> split(const string &str, const string &pattern);

class Graph 
{
	int VexNum, ArcNum;  //顶点数和边数
//邻接矩阵所需成员变量
	int** AM_Edge;  //邻接矩阵边集
	char* AM_Node;  //邻接矩阵顶点集
//邻接表所需成员变量
	VertexNode* AL_Node; //邻接表顶点集
//深度和广度优先搜索中，用于标记结点是否被访问的数组，按序号对应每个顶点
	bool *visited;

	std::map<string,std::vector<VertexNode>>& setOfconfig;
	string graphName;
public:
	
	std::vector<VertexNode> fullPathVect;
public:
	//A(int &x1,int z1) :x(x1), y(z), z(z1) {}
	Graph(std::map<string,std::vector<VertexNode>>& setOfconfig):setOfconfig(setOfconfig) {
		VexNum = ArcNum = 0;
		AM_Node = NULL;
		AM_Edge = NULL;
		AL_Node = NULL;
		visited = NULL;
	}

	~Graph() {
		//邻接矩阵析构
		//delete[] AM_Node;
		//for (int i = 0; i < VexNum; i++)
		//	delete[] AM_Edge[i];
		//delete[] AM_Edge;
		//邻接表析构
		//1.先析构边结点
		/**/
		EdgeNode* tmpCurr, *tmpNext;
		for (int i = 0; i < VexNum; i++)
		{
			tmpCurr = AL_Node[i].first;
			while (tmpCurr) {
				tmpNext = tmpCurr->next;
				delete tmpCurr;
				tmpCurr = tmpNext;
			}
		}
		//2.再析构顶点
		delete[] AL_Node;
	}

	

///1 使用邻接矩阵法储存图  相关成员函数
	//创建邻接矩阵存储的图
	void AM_GraphInitial() {  //初始化邻接矩阵并构造该图

	    ifstream infile;
		infile.open("C://Users//freeb//Desktop//autoswitch//vex.txt");
	    if (!infile.is_open())
	    {
		    cout << "读取文件失败" << endl;
		    return;
	    }
	    //第五种读取方法
	    string s,tempVex;
	    vector<char>v1;
	    while (getline(infile,tempVex))
	    {
            if( tempVex != "")
		        s=s+tempVex;
			else
			    break;
	    }
		infile.close();
		
		VexNum = s.length(); //确定顶点总数
		AM_Node = new char[VexNum];  //创建存储顶点的数组
		for (int i = 0; i < VexNum; i++)  //给顶点数组赋值
			AM_Node[i] = s[i];

		AM_Edge = new int*[VexNum];  //申请邻接矩阵存储空间
		for (int i = 0; i < VexNum; i++)  //对邻接矩阵初始化
		{
			AM_Edge[i] = new int[VexNum];
			for (int j = 0; j < VexNum; j++)
				AM_Edge[i][j] = 0;
		}

		string edge,tempEdge;
		vector<string>edgeVec;
        infile.open("C://Users//freeb//Desktop//autoswitch//edge-v2-i2s0.txt");
	    if (!infile.is_open())
	    {
		    cout << "读取文件失败" << endl;
		    return;
	    }
	    while (getline(infile,tempEdge))
	    {
            if( tempEdge != "")
		        edgeVec.push_back(tempEdge);
			else
			    break;
	    }
		infile.close();

		for( int k = 0; k<edgeVec.size();k++)
		{
		    int i = AM_locate(edgeVec[k].at(0));
			int j= AM_locate(edgeVec[k].at(1));
			if (i != -1 && j != -1)
			{
				AM_Edge[i][j] = 1;
				ArcNum++;  //统计边数
			}		
		}
	}
	int AM_locate(char vex) {   //寻找顶点在顶点数组中的下标
		for (int i = 0; i < VexNum; i++)
			if (AM_Node[i] == vex)
				return i;
		//cout << vex << " is not exit in this graph!" << endl;
		return -1;
	}
	void AM_GraphPrint() {  //以邻接矩阵的形式输出该图
		//cout << "adjacency matrix of this graph:" << endl;
		//cout << "  ";
		for (int i = 0; i < VexNum; i++)
			//cout << AM_Node[i]<<" ";
		//cout << endl;
		for (int i = 0; i < VexNum; i++) {
			//cout << AM_Node[i] << " ";
			//for (int j = 0; j < VexNum; j++)
				//cout << AM_Edge[i][j] << " ";
			//cout << endl;
		}

		//输出所有点的入度出度;
		cout << "vertex in-degree out-degree" << endl;
		int d1, d2;
		for (int i = 0; i < VexNum; i++)
		{
			AM_degree(AM_Node[i], d1, d2);
			cout << setw(6) << AM_Node[i] << setw(10) << d1 << setw(11) << d2 << endl;
		}

	}
	//求某个顶点的入度和出度
	void AM_degree(char vex,int &d1,int &d2) {
		d1 = d2 = 0;
		int loc = AM_locate(vex);
		for (int i = 0; i < VexNum; i++)
			if (AM_Edge[i][loc] != 0)  //入度，即顶点所在列非零项个数
				d1++;
		for (int i = 0; i < VexNum; i++)
			if (AM_Edge[loc][i] != 0)  //出度，即顶点所在行非零项个数
				d2++;
	}

	//广度优先搜索，输入为起始顶点在数组中的位置(针对一个连通分量)
	//使用队列实现，类似于二叉树的层次遍历
	void AM_BFS(int startLoc) {
		if (startLoc > VexNum - 1)
		{
			cout << "AL_BFS start location is error!" << endl;
			return;
		}

		//cout << "The BFS of this graph(stored by adjacency matrix):" << endl;
		InitialVisited();  //初始化标记数组
		queue<int> bfs;    
		bfs.push(startLoc);  
		visited[startLoc] = true;  //将起始结点加入队列并标记为已访问
		while (!bfs.empty()) {  //每次取出队列最前的顶点，并将其相连且未标记过的顶点加入队列尾部
			int j = bfs.front();
			//cout << AM_Node[j]<<" ";
		//	visited[j] = true;
			bfs.pop();
			//寻找该结点为起始结点，为结点未访问过的边；
			for (int i = 0; i < VexNum; i++) {
				if (AM_Edge[j][i] != 0 && visited[i] != true) //存在指向顶点i的边且该点未被访问则加入队列
				{
					bfs.push(i);
					visited[i] = true;  //应该在进入队列的时候标记为已经访问，而不能是输出时，否则可能会多次入队
				}
			}
		}
		//cout << endl;
	}

	//深度优先遍历（DFS），输入为起始点在数组中的下标(针对一个连通分量)
	void AM_DFS(int startLoc) {
		if (startLoc > VexNum - 1)
		{
			//cout << "AM_BFS start location is error!" << endl;
			return;
		}

		//cout << "The DFS of this graph(stored by adjacency matrix):" << endl;

		cout << "use recursion: ";
		InitialVisited();  //切记要先初始化标记数组
		AM_RecursiveDFS(startLoc);  //使用递归；
		//cout << endl;

		cout << "use stack: ";
		InitialVisited();
		AM_StackDFS(startLoc);    //使用栈
		//cout << endl;
	}

	//使用递归实现DFS(针对一个连通分量)
	void AM_RecursiveDFS(int startLoc) {
		cout << AM_Node[startLoc]<<" ";
		visited[startLoc] = true;
		for (int i = 0; i < VexNum; i++) {
			//如果发现存在相连且未标记的顶点，则递归为对该顶点的访问
			if (AM_Edge[startLoc][i] != 0 && visited[i] != true) 
				AM_RecursiveDFS(i);
		}
	}

	//使用栈实现DFS(针对一个连通分量)
	void AM_StackDFS(int startLoc) {
		if (startLoc > VexNum - 1)
		{
			cout << "AM_BFS start location is error!" << endl;
			return;
		}
		InitialVisited();
		stack<int> dfsStack;
		dfsStack.push(startLoc);//将起始点压栈并输出，标记该点已访问
		cout << AM_Node[startLoc] << " ";  
		visited[startLoc] = true;

		int* visitedPos;  //visitedPos用来记录栈中每个顶点在矩阵对应行中已经访问到的位置，
		                           // 避免回溯时while中的for循环每次都从0开始从而提高效率
		visitedPos = new int[VexNum];
		for (int i = 0; i < VexNum; i++) {
			visitedPos[i] = 0;  //初始化为0
		}

		while (!dfsStack.empty()) { //每次取栈顶元素，寻找与其相连且未标记过的顶点
			int j = dfsStack.top();
			for (int i = visitedPos[j]; i < VexNum; i++) //每次从visitedPos[j]开始，提高效率
			{
				if (AM_Edge[j][i] != 0 && visited[i] != true)  //寻找到未访问过的结点则加入栈中并输出
				{				
					dfsStack.push(i);
					cout << AM_Node[i] << " ";
					visitedPos[j] = i;  //更新该顶点在矩阵对应行中已查询到的位置
					visited[i] = true;  //将顶点i标记为已访问
					break;              //找到符合要求的点则退出for循环(进入while下一轮，进行更深的搜索)
				}
				if (i == VexNum-1)   //能执行到此说明与该点相关的点全部被访问，将该结点其退栈
					dfsStack.pop();				
			}
		}
		//注意销毁其空间
		delete visitedPos;
		visitedPos = NULL;
	}

	//借助栈，使用DFS实现寻找i到j的简单路径（此为逆序输出）
	void AM_FindPath(int i,int j) {
		cout << "path between " << AM_Node[i] << " and " << AM_Node[j]<< " : ";
		stack<int> pathStack;
		InitialVisited();
		if (AM_RecursiveDFS(i, j, pathStack))  //存在路径，则输出栈中保存路径的所有顶点
			while (!pathStack.empty()) {
				cout << AM_Node[pathStack.top()] << " ";
				pathStack.pop();
			}
		else {
			cout << "not existed！";
		}
		//cout << endl;
	}
	bool AM_RecursiveDFS(int startLoc,int endLoc,stack<int> &p) {
		p.push(startLoc);
		visited[startLoc] = true;
		if (startLoc == endLoc)  //栈顶顶点为要寻找的目标顶点则中止，直接返回
			return true;
		
		for (int i = 0; i < VexNum; i++) {
			//如果发现存在相连且未标记的顶点，则递归为对该顶点的访问
			if (AM_Edge[startLoc][i] != 0 && visited[i] != true)
				return AM_RecursiveDFS(i, endLoc, p); //返回与startLoc相连的顶点i
			                                         //是否存在到endLoc路径的搜索结果
		}
		p.pop();//执行到此说明startLoc无法到达endLoc,将其退栈
		return false;
	}


///使用邻接表法储存图--相关成员函数
	//初始化并创建该图
	void AL_GraphInitial(string vexFile,
						 string edgeFile,
						 /*string weightFile,*/
						 int paternIndex,
						 std::map<std::string ,std::map< std::string,int>>& vexMap0_bclk,
						 std::map<std::string ,std::map< std::string,int>>& vexMap1_bclk,
						 map<string,string>& weightMap2,
						 std::map<string,std::vector<VertexNode>> setOfconfig
						 ) { 

        graphName = graphName;
		//init_vexMap0_bclk();		
		//init_vexMap1_bclk();

		ifstream infile;
		infile.open(vexFile);
	    if (!infile.is_open())
	    {
		    cout << "读取文件失败" << endl;
		    return;
	    }

	    string s,tempVex;
	    vector<char>v1;
	    while (getline(infile,tempVex))
	    {
            if( tempVex != "")
		        s=s+tempVex;
			else
			    break;
	    }
		infile.close();
		VexNum = s.length();               
		AL_Node = new VertexNode[VexNum];  
		for (int i = 0; i < VexNum; i++)   
		{
			string t;
			std::map< std::string,int> tempType;

			AL_Node[i].data = s[i];
			t.push_back(s[i]);

			if( 1 == paternIndex )
			{
			    AL_Node[i].type = vexMap1_bclk[t].begin()->second;	
			}
			else
			{
				AL_Node[i].type = vexMap0_bclk[t].begin()->second;
			}

		}	

		string edge,tempEdge;
		vector<string>edgeVec;
		infile.open(edgeFile);
	    if (!infile.is_open())
	    {
		    cout << "读取文件失败" << endl;
		    return;
	    }
	    while (getline(infile,tempEdge))
	    {
            if( tempEdge != ""){
				vector<string> tmp = split(tempEdge, " ");
		        edgeVec.push_back(tmp[0]);
				weightMap2.insert({tmp[0],tmp[1]});	

			}
			else
			    break;
	    }
		infile.close();

		for( int k = 0; k<edgeVec.size();k++)
		{
			int i = AL_Locate(edgeVec[k].at(0));
			if (i == -1)
				continue;
			if (AL_Node[i].first == NULL)
				AL_Node[i].first = new EdgeNode(edgeVec[k].at(1));
			else { //找到最后一个边结点
				EdgeNode *tmp = AL_Node[i].first;
				while (tmp->next) {
					tmp = tmp->next;
				}
				tmp->next = new EdgeNode(edgeVec[k].at(1));
			}
		}

		setOfconfig = setOfconfig;

		//initialized weight 
		/*
		string tempWeight;
		vector<string> weightVec;

		infile.open(weightFile);
	    if (!infile.is_open())
	    {
		    cout << "读取文件失败" << endl;
		    return;
	    }

		while (getline(infile,tempWeight))
	    {
            if( tempWeight != "")
		        weightVec.push_back(tempWeight);
			else
			    break;
	    }
		infile.close();

		
		if( weightVec.size() != edgeVec.size())
		    cout<<"Config error...."<<endl;

		for( int i = 0; i < weightVec.size(); i++ )
		{
			int w = std::stoi(weightVec[i]);
		    weightMap.insert({edgeVec[i],w});	
		}
		*/



	}


	int AL_Locate(char vex) {  
		for (int i = 0; i < VexNum; i++)
			if (AL_Node[i].data == vex)
				return i;
		cout << vex << " is not exit in this graph!" << endl;
		return -1;
	}

	void AL_GraphPrint() {  
		//cout << "adjacency list of this graph:" << endl;
		EdgeNode *tmp = NULL;
		for (int i= 0; i < VexNum; i++)
		{
			//cout << AL_Node[i].data;
			tmp = AL_Node[i].first;
			while (tmp) {
				//cout << " -> " << tmp->adjVex;
				tmp = tmp->next;
			}
			//cout << endl;
		}

		//cout << "vertex in-degree out-degree" << endl;
		int d1, d2;
		for (int i = 0; i < VexNum; i++) {
			AL_degree(AL_Node[i].data, d1, d2);
			//cout << setw(6) << AL_Node[i].data << setw(10) << d1 << setw(11) << d2 << endl;
		}
	}


	void AL_degree(char vex,int &s1,int &s2) {
		s1 = s2 = 0;
		EdgeNode* tmp = NULL;
		int loc = AL_Locate(vex);
		//入度 ，即寻找vex出现在边结点中的次数
		for (int i = 0; i < VexNum; i++)
		{
			if (i == loc)  //vex对应顶点的边结点无需查找
				continue;
			tmp = AL_Node[i].first;
			while (tmp) {
				if (tmp->adjVex == vex)
				{
					s1++;
					break; //只存在一次，找到即可退出，进入for的下一轮循环
				}
				tmp = tmp->next;
			}
		}
		//出度
		tmp = AL_Node[loc].first;  //即vex对应顶点的边结点个数
		while (tmp) {  
			s2++;
			tmp = tmp->next;
		}		
	}

	//使用队列实现广度优先遍历(针对一个连通分量)
	void AL_BFS(int startLoc) {
		if (startLoc > VexNum - 1)
		{
			cout << "AL_BFS start location is error!" << endl;
			return;
		}
		//cout << "The BFS of this graph(stored by adjacency list):" << endl;
		InitialVisited(); //初始化访问标记数组
		queue<int> bfs;    
		bfs.push(startLoc);  //标记初始结点已访问并入队
		visited[startLoc] = true;  
		EdgeNode *tmp=NULL;

		while (!bfs.empty()) {  
			int j = bfs.front();
			bfs.pop();  //队列最前元素出队并输出
			//cout << AL_Node[j].data<<" ";
			tmp = AL_Node[j].first;
			while (tmp) {  //寻找与队列最前顶点相连，且未被标记的所有边结点，将其入队并标记
				int i = AL_Locate(tmp->adjVex);  //边结点在顶点数组中的位置
				if (visited[i] != true) {  //第i个结点未被标记，将其入队并标记
					bfs.push(i);
					visited[i] = true;
				}
				tmp = tmp->next;		
			}
		}
		//cout << endl;
	}

	void AL_DFS(int startLoc) {
		if (startLoc > VexNum - 1)
		{
			//cout << "AL_BFS start location is error!" << endl;
			return;
		}
		//cout << "The DFS of this graph(stored by adjacency list):" << endl;

		InitialVisited();
		//cout << "use Recursion: ";
		AL_RecursiveDFS(startLoc);  //递归DFS
		//cout << endl;

		InitialVisited();
		//cout<<"use Stack: ";
		AL_StackDFS(startLoc);  //栈DFS
		cout << endl;
	}

	void AL_RecursiveDFS(int startLoc) {
		//cout << AL_Node[startLoc].data<<" ";
		visited[startLoc] = true;  //输出当前点并标记
		EdgeNode* tmp;
		tmp = AL_Node[startLoc].first;
		while (tmp) {
			int i = AL_Locate(tmp->adjVex); //找到第一个邻接点在顶点数组中的位置
			if (visited[i] != true)
				AL_RecursiveDFS(i);//邻接点i未被标记，开始递归访问该结点
			tmp = tmp->next;
		}
	}


	//使用栈实现深度优先遍历(DFS)(针对一个连通分量)
	void AL_StackDFS(int startLoc) {
		stack<int> dfsStack;
		dfsStack.push(startLoc);  //将起始顶点压栈，并输出其值
		visited[startLoc] = true;  //标记该节点
		//cout << AL_Node[startLoc].data << " ";

		EdgeNode** visitedPos;   //用来记录每个顶点下一个将查询的边结点的位置，
		 //避免while内层循环while(1)中每个顶点每次都从头开始查询未标记的边结点，从而提高效率
		visitedPos = new EdgeNode*[VexNum];
		for (int i = 0; i < VexNum; i++) {  //每个顶点对应的访问位置都初始化第一个边结点；
			visitedPos[i] = AL_Node[i].first;
		}

		while (!dfsStack.empty()) {
			int j = dfsStack.top();

			while (1) {  //不断查询第j个顶点下一个将查询的边结点
				if (visitedPos[j] != NULL) { //若存在未查询过的边结点
					int i = AL_Locate(visitedPos[j]->adjVex); //获取该边结点对应的顶点在顶点数组中的序号
					if (visited[i] != true) {  //如果该边结点未被标记，则将其压栈输出然后退出内层循环
						dfsStack.push(i);
						//cout << AL_Node[i].data << " ";
						visited[i] = true;
						visitedPos[j] = visitedPos[j]->next;  //更新结点j的下一个将查询的边结点指针
						break;  //退出内层while循环，进入外层while下一轮循环从而进行更深的访问
					}
					else //仍存在未查询过的边结点，visitedPos[j]指向下一边结点
					{
						visitedPos[j] = visitedPos[j]->next;
					}
						
				} //如果该顶点的可查询的边结点为空，则将该顶点退出
				else {
					dfsStack.pop();
					break;
				}				
			}
		}
		delete[] visitedPos; //把指针数组销毁即可，不要把每个数组元素指向的空间销毁
		visitedPos = NULL;
	}

	//借助栈，使用深度遍历（DFS），寻找i到j的简单路径（此为逆序输出）
	void AL_FindPath(int i, int j) {
		cout << "path between " << AL_Node[i].data << " and " << AL_Node[j].data << " : ";
		stack<int> pathStack;
		InitialVisited();
		if (AL_RecursiveDFS(i, j, pathStack))  //存在路径，则输出栈中保存路径的所有顶点
		{
			while (!pathStack.empty()) {
				cout << AL_Node[pathStack.top()].data<<" ";
				fullPathVect.insert(fullPathVect.begin(),AL_Node[pathStack.top()]);
				pathStack.pop();
			}
		}

		else {
			cout << "not existed！";
		}
		//cout<< endl;
	}

	void AL_FindPath(string name,int i, int j) {
		//cout << "path between " << AL_Node[i].data << " and " << AL_Node[j].data << " : ";
		stack<int> pathStack;
		InitialVisited();
		if (AL_RecursiveDFS(i, j, pathStack))  //存在路径，则输出栈中保存路径的所有顶点
		{
			while (!pathStack.empty()) {
				//cout << AL_Node[pathStack.top()].data<<" ";
				fullPathVect.insert(fullPathVect.begin(),AL_Node[pathStack.top()]);
				pathStack.pop();
			}
			setOfconfig.insert(pair<string , std::vector<VertexNode>>(name , fullPathVect));
			fullPathVect.clear();
		}

		else {
			cout << "not existed！";
		}
		//cout<< endl;
	}

	bool AL_RecursiveDFS(int startLoc,int endLoc, stack<int> &p) {
		p.push(startLoc);
		visited[startLoc] = true;  //输出当前点并标记
		if (startLoc == endLoc)  //栈顶顶点等于要寻找的顶点j时，说明存在路径i-j，沿途顶点都存在栈中,此时直接返回
			return true;
		EdgeNode* tmp;
		tmp = AL_Node[startLoc].first;
		while (tmp) {
			int i = AL_Locate(tmp->adjVex); //找到第一个邻接点在顶点数组中的位置
			if (visited[i] != true) {
				if (true == AL_RecursiveDFS(i, endLoc, p))//如果顶点i存在到endLoc的路径，则返回true
					return true;
			}			
			tmp = tmp->next;
		}
		p.pop();  //执行到此说明startLoc无法到达endLoc,将其退栈
		return false;
	}

	//初始化标记数组，全部设为未被访问（false）
	void InitialVisited() {
		if (visited == NULL)
			visited = new bool[VexNum];

		for (int i = 0; i < VexNum; i++)
			visited[i] = false;
	}
};

