#ifndef _SWITCH_CONFIG_H_
#define _SWITCH_CONFIG_H_

#include "graph.h"

class switchSet
{
public:
    std::map<string,std::vector<VertexNode>>setOfconfig;
    std::vector<string> switchVec;
    
    map<string,int> weightMap;
private:
    Graph graph;

    std::map<std::string ,std::map< std::string,int>> vexMap0_bclk,vexMap1_bclk,vexMap0_fclk,vexMap1_fclk;        //0:codec节点 1：开关节点 2：内部节点 3：mcu节点  4：uif节点 5:重复的开关节点
public:
    switchSet();
    ~switchSet();
    string get_node_shortname( std::map<std::string ,std::map< std::string,int>> map,string node );
    string get_node_longname( std::map<std::string ,std::map< std::string,int>> map,string node );
    int get_node_type( std::map<std::string ,std::map< std::string,int>> map,string node );
    int findIndex(const vector<char> &arr, char item);
    void setRegSwitch( string pathName );
    void setI2s23Bclk( string name);
    void process_i2s23_bclk( std::vector<string> configparamter);
    void process_i2s1_bclk( std::vector<string> configparamter);
};

#endif