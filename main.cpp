
#include<regex>
#include<sstream>
#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include  <direct.h>  


#include "graph.cpp"

std::map<string,std::vector<VertexNode>>setOfconfig;
std::vector<string> switchVec;
std::map<std::string ,std::map< std::string,int>> vexMap0_bclk,vexMap1_bclk; 
map<string,int> weightMap;
map<string,string> weightMap2;

vector<string> split(const string &str, const string &pattern)
{
    vector<string> res;
    if(str == "")
        return res;
    //在字符串末尾也加入分隔符，方便截取最后一段
    string strs = str + pattern;
    size_t pos = strs.find(pattern);

    while(pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        res.push_back(temp);
        //去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos+1, strs.size());
        pos = strs.find(pattern);
    }

    return res;
}


void init_vexMap0_bclk( void )
	{
		//0:codec节点 1：开关节点 2：内部节点 3：mcu节点  4：uif节点 5:重复的开关节点
		//{'A','C','D','E','F','H','I'}
		map<string,int> temp;
	    temp.insert( {"CODEC_BCLK[0]",0});
		vexMap0_bclk["A"] = temp;
		temp.clear();
		temp.insert( {"UIF_BCLK[0]",4});
		vexMap0_bclk["C"] = temp;
		temp.clear();
		temp.insert( {"SSC_BCLK[0]",3});
		vexMap0_bclk["D"] = temp;
		temp.clear();
		temp.insert( {"CODEC_BCLK[1]",0});
		vexMap0_bclk["E"] = temp;	
		temp.clear();
		temp.insert( {"T[1]",1});
		vexMap0_bclk["F"] = temp;
		temp.clear();
		temp.insert( {"UIF_BCLK[1]",4});
		vexMap0_bclk["H"] = temp;
		temp.clear();
		temp.insert( {"SSC_BCLK[1]",3});
		vexMap0_bclk["I"] = temp;						
	}


	void init_vexMap1_bclk( void )
	{
		map<string,int> temp;
	    temp.insert( {"CODEC_BCLK[0]",0});
		vexMap1_bclk["A"] = temp;
		temp.clear();
		temp.insert( {"CODEC_BCLK[2]",0});
		vexMap1_bclk["B"] = temp;
		temp.clear();
		temp.insert( {"T[23:22]",1});
		vexMap1_bclk["C"] = temp;	
		temp.clear();
		temp.insert( {"UIF_BCLK[2]",4});
		vexMap1_bclk["F"] = temp;
		temp.clear();
		temp.insert( {"SSC_BCLK[2]",3});
		vexMap1_bclk["G"] = temp;
		temp.clear();
		temp.insert( {"CODEC_BCLK[3]",0});
		vexMap1_bclk["H"] = temp;	
		temp.clear();
		temp.insert( {"T[25:24]",1});
		vexMap1_bclk["I"] = temp;	
		temp.clear();
		temp.insert( {"UIF_BCLK[3]",4});
		vexMap1_bclk["K"] = temp;
		temp.clear();
		temp.insert( {"SSC_BCLK[3]",3});
		vexMap1_bclk["L"] = temp;		
		temp.clear();
		temp.insert( {"CODEC_BCLK[4]",0});
		vexMap1_bclk["M"] = temp;	
		temp.clear();
		temp.insert( {"T[27:26]",1});
		vexMap1_bclk["N"] = temp;
		temp.clear();
		temp.insert( {"UIF_BCLK[4]",4});
		vexMap1_bclk["P"] = temp;
		temp.clear();
		temp.insert( {"SSC_BCLK[4]",3});
		vexMap1_bclk["Q"] = temp;		
		temp.clear();
		temp.insert( {"codec_uif_bclk[2]",2});
		vexMap1_bclk["R"] = temp;
		temp.clear();
		temp.insert( {"codec_uif_bclk[3]",2});
		vexMap1_bclk["S"] = temp;	
		temp.clear();
		temp.insert( {"codec_uif_bclk[4]",2});
		vexMap1_bclk["T"] = temp;		
	}

string get_node_shortname( std::map<std::string ,std::map< std::string,int>> map,string node )
{
	std::map<std::string ,std::map< std::string,int>>::iterator multitr;  
    std::map< std::string,int>::iterator intertr;
    for(multitr=map.begin();multitr!=map.end();multitr++)
    {
        for(intertr= multitr ->second.begin(); intertr != multitr ->second.end(); intertr ++)
		{
            //std::cout<< multitr ->first<<" "<<intertr->first<<" ("<< intertr -> second <<")"<<endl;
			if(node == intertr->first)
			    return multitr ->first;
		}
					
    }

	return NULL;
}

string get_node_longname( std::map<std::string ,std::map< std::string,int>> map,string node )
{
	std::map<std::string ,std::map< std::string,int>>::iterator multitr;  
    std::map< std::string,int>::iterator intertr;
    for(multitr=map.begin();multitr!=map.end();multitr++)
    {
        for(intertr= multitr ->second.begin(); intertr != multitr ->second.end(); intertr ++)
		{
			if(node == multitr->first)
			    return intertr->first;
		}
					
    }

	return NULL;
}

int get_node_type( std::map<std::string ,std::map< std::string,int>> map,string node )
{
	std::map<std::string ,std::map< std::string,int>>::iterator multitr;  
    std::map< std::string,int>::iterator intertr;
    for(multitr=map.begin();multitr!=map.end();multitr++)
    {
        for(intertr= multitr ->second.begin(); intertr != multitr ->second.end(); intertr ++)
		{
            //std::cout<< multitr ->first<<" "<<intertr->first<<" ("<< intertr -> second <<")"<<endl;
			if(node == intertr->first)
			    return intertr->second;
		}
					
    }

	return -1;
}

int findIndex(const vector<char> &arr, char item) {

    for (auto i = 0; i < arr.size(); ++i) {
        if (arr[i] == item)
            return i;
    }

    return -1;
}

void setRegSwitch( string pathName )
{
		string path0,path1;
		string regName;
		if( setOfconfig[pathName].size() == 2 )
		{
			path0.clear();
			path1.clear();
			path0.insert(path0.end(),setOfconfig[pathName][0].data);
			path0.insert(path0.end(),setOfconfig[pathName][1].data);	
			string w = weightMap2[path0];

			switchVec.push_back(w);

		}
		else if( setOfconfig[pathName].size() == 3 ) 
		{
			path0.clear();
			path1.clear();

			path0.insert(path0.end(),setOfconfig[pathName][0].data);
			path0.insert(path0.end(),setOfconfig[pathName][1].data);	
			string w = weightMap2[path0];

			path1.insert(path1.end(),setOfconfig[pathName][1].data);
			path1.insert(path1.end(),setOfconfig[pathName][2].data);	
			string w1 = weightMap2[path1];


			switchVec.push_back(w);

			switchVec.push_back(w1);
			//cout << regName << endl;

		}
}

void setI2s23Bclk( string name)
{
	    std::smatch matchResult1,matchResult2,matchSwitch1,matchSwitch2;
		for( int i=0;i<setOfconfig[name].size()-1;i++)
		{
			string path0;
			path0.insert(path0.end(),setOfconfig[name][i].data);
			path0.insert(path0.end(),setOfconfig[name][i+1].data);
			vector<string> tempWeightVec,commSwitchVec1,commSwitchVec2;
			if( weightMap2[path0].find(":") != string::npos)
			{
				tempWeightVec = split(weightMap2[path0],":"); 
				commSwitchVec1 = split(tempWeightVec[0],","); 
				commSwitchVec2 = split(tempWeightVec[1],",");
				if( commSwitchVec1[0] != commSwitchVec2[0])
				{
					std::regex switchPattern("^T\\['\\d{1,2}'\\]");
					bool isSwitchMatch = regex_search(commSwitchVec1[0], matchSwitch1, switchPattern);
					bool isSwitchMatch2 = regex_search(commSwitchVec2[0], matchSwitch2, switchPattern);	
					string commSwitch1=matchSwitch1.str();
					string commSwitch2=matchSwitch2.str();
					if( commSwitch1 == commSwitch2 )
					{
					    for (size_t j = 0; j < switchVec.size(); j++)
					    {
						    if(switchVec[j].find(commSwitch1) != string::npos)
							{
								if(commSwitchVec1[0].find(switchVec[j]) != string::npos)
								{
									switchVec.insert(switchVec.end(),commSwitchVec1.begin(),commSwitchVec1.end());
									break;
								}
								else if(commSwitchVec2[0].find(switchVec[j]) != string::npos)
								{
									switchVec.insert(switchVec.end(),commSwitchVec2.begin(),commSwitchVec2.end());
									break;
								}
							}
					    }
					}
									
				}
			}
			else
			{
					tempWeightVec = split(weightMap2[path0],","); 
					switchVec.insert(switchVec.end(),tempWeightVec.begin(),tempWeightVec.end());
			}

		}
}

void process_sync_option( std::vector<string> configparamter )
{
	if(configparamter.size() <=5 ) return;

	vector<string> syncOption =  split(configparamter[4], ":");
	if(syncOption.size()<2) return ;
	if(  ("sync" == syncOption[0]) && ( "All" ==   syncOption[1]) )  
	{
	    switchVec.push_back("T['4']=0");
		switchVec.push_back("T['3']=0");
		switchVec.push_back("T['2']=0");

		switchVec.push_back("T['28']=1");	
		switchVec.push_back("T['29']=1");

		switchVec.push_back("T['22']=1");	
		switchVec.push_back("T['23']=1");

		switchVec.push_back("T['24']=1");	
		switchVec.push_back("T['25']=1");	

		switchVec.push_back("T['26']=1");	
		switchVec.push_back("T['27']=1");						

		switchVec.push_back("T['5']=1");	

	}





}

void process_i2s23_bclk( std::vector<string> configparamter)
{
	vector<string> parameter_i2s2,parameter_i2s3,syncOption,crossOption;
	string pathStart1,pathStart2,pathStart3;
	string pathEnd1,pathEnd2,pathEnd3;
	string mcuPoint1,mcuPoint2,mcuPoint3;

	Graph graph(setOfconfig);

	vector<char> vecNode = { 'A','B','H','M','C','I','N','R','S','T','F','K','P','G','L','Q'};
	/*
	graph.AL_GraphInitial(
	                       "C://FAB02SwitchConfigTool//vex23.txt",
						   "C://FAB02SwitchConfigTool//i2s23_config.txt",
						   0,
						   vexMap0_bclk,
						   vexMap1_bclk,
						   weightMap2,
						   setOfconfig
						   );
	*/
    char buffer[512];   
	getcwd(buffer,512);
	string rootPath(buffer);

	graph.AL_GraphInitial(
	                       rootPath+"\\vex23.conf",
						   rootPath+"\\i2s23_config.conf",
						   0,
						   vexMap0_bclk,
						   vexMap1_bclk,
						   weightMap2,
						   setOfconfig
						   );	

	graph.AL_GraphPrint();
	graph.AL_DFS(0);

	parameter_i2s2 = split(configparamter[1], ":");
	parameter_i2s3 = split(configparamter[2], ":");
	syncOption = split(configparamter[4], ":");
	crossOption = split(configparamter[5], ":");

	//vector<string> syncOption =  split(configparamter[4], ":");
	if( (syncOption.size()>=2)  && ("sync" == syncOption[0]) && ( "All" ==   syncOption[1]) )  return;

	if(parameter_i2s2.empty() && parameter_i2s3.empty()) return;

	if((!parameter_i2s2.empty()) && ( parameter_i2s3.empty()))
	{
		if(parameter_i2s2[1] =="master")
		{
			cout<<"I2S2:Master"<<endl;

			switchVec.push_back("T['3']=0");
			switchVec.push_back("T['2']=0");	

			pathStart1 = "CODEC_BCLK[2]";       //k
			pathStart2 = "CODEC_BCLK[3]";       //v
			pathStart3 = "CODEC_BCLK[4]";       //m
			pathEnd1 = "UIF_BCLK[2]";           //a
			pathEnd2 = "UIF_BCLK[3]";           //b
			pathEnd3 = "UIF_BCLK[4]";           //c
			mcuPoint1 = "SSC_BCLK[2]";          //x
			mcuPoint2 = "SSC_BCLK[3]";          //y	
			mcuPoint3 = "SSC_BCLK[4]";          //z	

			string statPoint1 = get_node_shortname( vexMap1_bclk,pathStart1);
			string statPoint2 = get_node_shortname( vexMap1_bclk,pathStart2);
			string statPoint3 = get_node_shortname( vexMap1_bclk,pathStart3);
			string endPoint1 = get_node_shortname( vexMap1_bclk,pathEnd1);
			string endPoint2 = get_node_shortname( vexMap1_bclk,pathEnd2);
			string endPoint3 = get_node_shortname( vexMap1_bclk,pathEnd3);			
			string mcu1 = get_node_shortname( vexMap1_bclk,mcuPoint1);
			string mcu2 = get_node_shortname( vexMap1_bclk,mcuPoint2);
			string mcu3 = get_node_shortname( vexMap1_bclk,mcuPoint3);

			int k = findIndex( vecNode,  statPoint1[0]) ;
			int v = findIndex( vecNode,  statPoint2[0]) ;
			int m = findIndex( vecNode,  statPoint3[0]) ;

			int a = findIndex( vecNode,  endPoint1[0]) ;
			int b = findIndex( vecNode,  endPoint2[0]) ;
			int c = findIndex( vecNode,  endPoint3[0]) ;

			int x = findIndex( vecNode,  mcu1[0]) ;
			int y = findIndex( vecNode,  mcu2[0]) ;
			int z = findIndex( vecNode,  mcu3[0]) ;					

			//i2s2 master 
			//CODEC_BCLK[3]->UIF_BCLK[4] ===> v-->c===> H-->P
			graph.AL_FindPath("codec3Touif4",v,c);
			//UIF_BCLK[4]->SSC_BCLK[3] ===> v-->c===> P-->L
			graph.AL_FindPath("uif4Tomcu3",c,y);

			setI2s23Bclk("codec3Touif4");
			setI2s23Bclk("uif4Tomcu3");
		}
		else if(parameter_i2s2[1] =="slave")
		{
			cout<<"I2S2:slave"<<endl;
			switchVec.push_back("T['4']=1");

			pathStart1 = "CODEC_BCLK[2]";       //k
			pathStart2 = "CODEC_BCLK[3]";       //v
			pathStart3 = "CODEC_BCLK[4]";       //m
			pathEnd1 = "UIF_BCLK[2]";           //a
			pathEnd2 = "UIF_BCLK[3]";           //b
			pathEnd3 = "UIF_BCLK[4]";           //c
			mcuPoint1 = "SSC_BCLK[2]";          //x
			mcuPoint2 = "SSC_BCLK[3]";          //y	
			mcuPoint3 = "SSC_BCLK[4]";          //z	

			string statPoint1 = get_node_shortname( vexMap1_bclk,pathStart1);
    		string statPoint2 = get_node_shortname( vexMap1_bclk,pathStart2);
			string statPoint3 = get_node_shortname( vexMap1_bclk,pathStart3);
    		string endPoint1 = get_node_shortname( vexMap1_bclk,pathEnd1);
    		string endPoint2 = get_node_shortname( vexMap1_bclk,pathEnd2);
    		string endPoint3 = get_node_shortname( vexMap1_bclk,pathEnd3);			
    		string mcu1 = get_node_shortname( vexMap1_bclk,mcuPoint1);
    		string mcu2 = get_node_shortname( vexMap1_bclk,mcuPoint2);
    		string mcu3 = get_node_shortname( vexMap1_bclk,mcuPoint3);

			int k = findIndex( vecNode,  statPoint1[0]) ;
			int v = findIndex( vecNode,  statPoint2[0]) ;
			int m = findIndex( vecNode,  statPoint3[0]) ;

			int a = findIndex( vecNode,  endPoint1[0]) ;
			int b = findIndex( vecNode,  endPoint2[0]) ;
			int c = findIndex( vecNode,  endPoint3[0]) ;

			int x = findIndex( vecNode,  mcu1[0]) ;
			int y = findIndex( vecNode,  mcu2[0]) ;
			int z = findIndex( vecNode,  mcu3[0]) ;	

			graph.AL_FindPath("uif4Tomcu3",c,x);
			graph.AL_FindPath("uif4Tomcu2",c,y);

			setI2s23Bclk("uif4Tomcu3");
			setI2s23Bclk("uif4Tomcu2");					
		
		}	
		
	}
	else if((!parameter_i2s3.empty())&&(parameter_i2s2.empty()) && ((parameter_i2s3[1] =="master")))
	{
		if((parameter_i2s3[1] =="master") )
		{
			cout<<"I2S3:Master"<<endl;		
			switchVec.push_back("T['4']=0");
			switchVec.push_back("T['3']=0");
			switchVec.push_back("T['2']=0");	

			pathStart1 = "CODEC_BCLK[2]";       //k
			pathStart2 = "CODEC_BCLK[3]";       //v
			pathStart3 = "CODEC_BCLK[4]";       //m
			pathEnd1 = "UIF_BCLK[2]";           //a
			pathEnd2 = "UIF_BCLK[3]";           //b
			pathEnd3 = "UIF_BCLK[4]";           //c
			mcuPoint1 = "SSC_BCLK[2]";          //x
			mcuPoint2 = "SSC_BCLK[3]";          //y	
			mcuPoint3 = "SSC_BCLK[4]";          //z	

			string statPoint1 = get_node_shortname( vexMap1_bclk,pathStart1);
			string statPoint2 = get_node_shortname( vexMap1_bclk,pathStart2);
			string statPoint3 = get_node_shortname( vexMap1_bclk,pathStart3);
			string endPoint1 = get_node_shortname( vexMap1_bclk,pathEnd1);
			string endPoint2 = get_node_shortname( vexMap1_bclk,pathEnd2);
			string endPoint3 = get_node_shortname( vexMap1_bclk,pathEnd3);			
			string mcu1 = get_node_shortname( vexMap1_bclk,mcuPoint1);
			string mcu2 = get_node_shortname( vexMap1_bclk,mcuPoint2);
			string mcu3 = get_node_shortname( vexMap1_bclk,mcuPoint3);

			int k = findIndex( vecNode,  statPoint1[0]) ;
			int v = findIndex( vecNode,  statPoint2[0]) ;
			int m = findIndex( vecNode,  statPoint3[0]) ;

			int a = findIndex( vecNode,  endPoint1[0]) ;
			int b = findIndex( vecNode,  endPoint2[0]) ;
			int c = findIndex( vecNode,  endPoint3[0]) ;

			int x = findIndex( vecNode,  mcu1[0]) ;
			int y = findIndex( vecNode,  mcu2[0]) ;
			int z = findIndex( vecNode,  mcu3[0]) ;					

			//CODEC_BCLK[4]->UIF_BCLK[3] ===> m-->b===> M-->K
			graph.AL_FindPath("codec4Touif3",m,b);
			//UIF_BCLK[4]->SSC_BCLK[4] ===> b-->z===> K-->Q
			graph.AL_FindPath("uif3Tomcu4",b,z);
			graph.AL_FindPath("codec4Touif2",m,a);		


			//setI2s23Bclk("codec3Touif4");
			//setI2s23Bclk("uif4Tomcu3");	
			setI2s23Bclk("codec4Touif3");
			setI2s23Bclk("uif3Tomcu4");		
			setI2s23Bclk("codec4Touif2");
		}
		else if(parameter_i2s3[1] =="slave")	
		{
			cout<<"I2S3:slave"<<endl;

			switchVec.push_back("T['3']=1");
		}	
			
	}
	else if((parameter_i2s2[1] =="slave") && ((parameter_i2s3[1] =="master")))
	{
		cout<<"I2S2:slave"<<endl;
		cout<<"I2S3:Master"<<endl;
	    switchVec.push_back("T['4']=1");
		switchVec.push_back("T['3']=0");
		switchVec.push_back("T['2']=0");

		pathStart1 = "CODEC_BCLK[2]";       //k
		pathStart2 = "CODEC_BCLK[3]";       //v
		pathStart3 = "CODEC_BCLK[4]";       //m
		pathEnd1 = "UIF_BCLK[2]";           //a
		pathEnd2 = "UIF_BCLK[3]";           //b
		pathEnd3 = "UIF_BCLK[4]";           //c
		mcuPoint1 = "SSC_BCLK[2]";          //x
		mcuPoint2 = "SSC_BCLK[3]";          //y	
		mcuPoint3 = "SSC_BCLK[4]";          //z	

		string statPoint1 = get_node_shortname( vexMap1_bclk,pathStart1);
    	string statPoint2 = get_node_shortname( vexMap1_bclk,pathStart2);
		string statPoint3 = get_node_shortname( vexMap1_bclk,pathStart3);
    	string endPoint1 = get_node_shortname( vexMap1_bclk,pathEnd1);
    	string endPoint2 = get_node_shortname( vexMap1_bclk,pathEnd2);
    	string endPoint3 = get_node_shortname( vexMap1_bclk,pathEnd3);			
    	string mcu1 = get_node_shortname( vexMap1_bclk,mcuPoint1);
    	string mcu2 = get_node_shortname( vexMap1_bclk,mcuPoint2);
    	string mcu3 = get_node_shortname( vexMap1_bclk,mcuPoint3);

		int k = findIndex( vecNode,  statPoint1[0]) ;
		int v = findIndex( vecNode,  statPoint2[0]) ;
		int m = findIndex( vecNode,  statPoint3[0]) ;

		int a = findIndex( vecNode,  endPoint1[0]) ;
		int b = findIndex( vecNode,  endPoint2[0]) ;
		int c = findIndex( vecNode,  endPoint3[0]) ;

		int x = findIndex( vecNode,  mcu1[0]) ;
		int y = findIndex( vecNode,  mcu2[0]) ;
		int z = findIndex( vecNode,  mcu3[0]) ;					

		//i2s3 master
		//CODEC_BCLK[4]->UIF_BCLK[3] ===> m-->b===> M-->K
		graph.AL_FindPath("codec4Touif3",m,b);
		//UIF_BCLK[4]->SSC_BCLK[4] ===> b-->z===> K-->Q
		graph.AL_FindPath("uif3Tomcu4",b,z);
		graph.AL_FindPath("codec4Touif2",m,a);

		//12s2 slave
		//CODEC_BCLK[2]->UIF_BCLK[2] ==>k-->a==>B-->F
		graph.AL_FindPath("codec2Touif2",k,a);  

		setI2s23Bclk( "codec4Touif3" );
		setI2s23Bclk( "uif3Tomcu4" );		
		setI2s23Bclk( "codec4Touif2" );	
		setI2s23Bclk( "codec2Touif2" );				
	}
	else if((parameter_i2s2[1] =="master") && ((parameter_i2s3[1] =="slave")))
	{
		cout<<"I2S2:Master"<<endl;
		cout<<"I2S3:slave"<<endl;
		switchVec.push_back("T['4']=0");
		switchVec.push_back("T['3']=1");
		switchVec.push_back("T['2']=1");	

		pathStart1 = "CODEC_BCLK[2]";       //k
		pathStart2 = "CODEC_BCLK[3]";       //v
		pathStart3 = "CODEC_BCLK[4]";       //m
		pathEnd1 = "UIF_BCLK[2]";           //a
		pathEnd2 = "UIF_BCLK[3]";           //b
		pathEnd3 = "UIF_BCLK[4]";           //c
		mcuPoint1 = "SSC_BCLK[2]";          //x
		mcuPoint2 = "SSC_BCLK[3]";          //y	
		mcuPoint3 = "SSC_BCLK[4]";          //z	

		string statPoint1 = get_node_shortname( vexMap1_bclk,pathStart1);
    	string statPoint2 = get_node_shortname( vexMap1_bclk,pathStart2);
		string statPoint3 = get_node_shortname( vexMap1_bclk,pathStart3);
    	string endPoint1 = get_node_shortname( vexMap1_bclk,pathEnd1);
    	string endPoint2 = get_node_shortname( vexMap1_bclk,pathEnd2);
    	string endPoint3 = get_node_shortname( vexMap1_bclk,pathEnd3);			
    	string mcu1 = get_node_shortname( vexMap1_bclk,mcuPoint1);
    	string mcu2 = get_node_shortname( vexMap1_bclk,mcuPoint2);
    	string mcu3 = get_node_shortname( vexMap1_bclk,mcuPoint3);

		int k = findIndex( vecNode,  statPoint1[0]) ;
		int v = findIndex( vecNode,  statPoint2[0]) ;
		int m = findIndex( vecNode,  statPoint3[0]) ;

		int a = findIndex( vecNode,  endPoint1[0]) ;
		int b = findIndex( vecNode,  endPoint2[0]) ;
		int c = findIndex( vecNode,  endPoint3[0]) ;

		int x = findIndex( vecNode,  mcu1[0]) ;
		int y = findIndex( vecNode,  mcu2[0]) ;
		int z = findIndex( vecNode,  mcu3[0]) ;	

		//CODEC_BCLK[3]->UIF_BCLK[4] ===> v-->c===> H-->P
		graph.AL_FindPath("codec3Touif4",v,c);
		//UIF_BCLK[4]->SSC_BCLK[3] ===> c-->y===> P-->L
		graph.AL_FindPath("uif4Tomcu3",c,y);
		//UIF_BCLK[4]->SSC_BCLK[2] ===> c-->x===> P-->G
		graph.AL_FindPath("uif4Tomcu2",c,y);


		setI2s23Bclk("codec3Touif4");
		setI2s23Bclk("uif4Tomcu3");
		setI2s23Bclk("uif4Tomcu2");		
	}
	else if((parameter_i2s2[1] =="master") && ((parameter_i2s3[1] =="master")))
	{
		cout<<"I2S2:Master"<<endl;
		cout<<"I2S3:Master"<<endl;		
		switchVec.push_back("T['4']=0");
		switchVec.push_back("T['3']=0");
		switchVec.push_back("T['2']=0");	
		if( ("cross" == crossOption[0]) && ( "Yes" == crossOption[1]) )
		{
		pathStart1 = "CODEC_BCLK[2]";       //k
		pathStart2 = "CODEC_BCLK[3]";       //v
		pathStart3 = "CODEC_BCLK[4]";       //m
		pathEnd1 = "UIF_BCLK[2]";           //a
		pathEnd2 = "UIF_BCLK[3]";           //b
		pathEnd3 = "UIF_BCLK[4]";           //c
		mcuPoint1 = "SSC_BCLK[2]";          //x
		mcuPoint2 = "SSC_BCLK[3]";          //y	
		mcuPoint3 = "SSC_BCLK[4]";          //z	

		string statPoint1 = get_node_shortname( vexMap1_bclk,pathStart1);
    	string statPoint2 = get_node_shortname( vexMap1_bclk,pathStart2);
		string statPoint3 = get_node_shortname( vexMap1_bclk,pathStart3);
    	string endPoint1 = get_node_shortname( vexMap1_bclk,pathEnd1);
    	string endPoint2 = get_node_shortname( vexMap1_bclk,pathEnd2);
    	string endPoint3 = get_node_shortname( vexMap1_bclk,pathEnd3);			
    	string mcu1 = get_node_shortname( vexMap1_bclk,mcuPoint1);
    	string mcu2 = get_node_shortname( vexMap1_bclk,mcuPoint2);
    	string mcu3 = get_node_shortname( vexMap1_bclk,mcuPoint3);

		int k = findIndex( vecNode,  statPoint1[0]) ;
		int v = findIndex( vecNode,  statPoint2[0]) ;
		int m = findIndex( vecNode,  statPoint3[0]) ;

		int a = findIndex( vecNode,  endPoint1[0]) ;
		int b = findIndex( vecNode,  endPoint2[0]) ;
		int c = findIndex( vecNode,  endPoint3[0]) ;

		int x = findIndex( vecNode,  mcu1[0]) ;
		int y = findIndex( vecNode,  mcu2[0]) ;
		int z = findIndex( vecNode,  mcu3[0]) ;					

		//M->P,M->G,M->L
		//CODEC_BCLK[4]->UIF_BCLK[4] ===> m-->c===> M-->P
		//graph.AL_FindPath("codec4Touif4",m,c);  
		//CODEC_BCLK[4]->SSC_BCLK[2] ===> m-->x===> M-->G
		//graph.AL_FindPath("codec4mcu2",m,x); 
		//CODEC_BCLK[4]->SSC_BCLK[3] ===> m-->y===> M-->L	
		//graph.AL_FindPath("codecTomcu3",m,y);
		//CODEC_BCLK[3]->SSC_BCLK[4] ===> v-->z===> H-->Q	
		//graph.AL_FindPath("codec3Tomcu4",v,z);

		//i2s2 master 
		//CODEC_BCLK[3]->UIF_BCLK[4] ===> v-->c===> H-->P
		graph.AL_FindPath("codec3Touif4",v,c);
		//UIF_BCLK[4]->SSC_BCLK[3] ===> c-->y===> P-->L
		graph.AL_FindPath("uif4Tomcu3",c,y);
		//UIF_BCLK[4]->SSC_BCLK[2] ===> c-->x===> P-->G
		graph.AL_FindPath("uif4Tomcu2",c,y);

		//CODEC_BCLK[4]->UIF_BCLK[3] ===> m-->b===> M-->K
		graph.AL_FindPath("codec4Touif3",m,b);
		//UIF_BCLK[4]->SSC_BCLK[4] ===> b-->z===> K-->Q
		graph.AL_FindPath("uif3Tomcu4",b,z);
		//CODEC_BCLK[4]->UIF_BCLK[2] ===> m-->a===> M-->F	  //由于fab02的rx、tx必须使用同一个时钟，所以不需要再从uif将时钟供给ssc2了。	
		graph.AL_FindPath("codec4Touif2",m,a);

		setI2s23Bclk("codec3Touif4");
		setI2s23Bclk("uif4Tomcu3");
		setI2s23Bclk("uif4Tomcu2");				
		setI2s23Bclk("codec4Touif3");
		setI2s23Bclk("uif3Tomcu4");			
		setI2s23Bclk("codec4Touif2");	
		}
		else 
		{
			switchVec.push_back("T['28']=1");
			switchVec.push_back("T['29']=1");
			switchVec.push_back("T['24']=1");
		}			
	}
	else
	{
		switchVec.push_back("T['4']=1");
		switchVec.push_back("T['3']=1");
		switchVec.push_back("T['2']=1");

		switchVec.push_back("T['28']=1");
		switchVec.push_back("T['29']=1");		
	}

	weightMap2.clear();
}

void process_i2s1_bclk( std::vector<string> configparamter)
{
	vector<string> parameter;
	string pathStart1,pathStart2;
	string pathEnd1,pathEnd2;
	string mcuPoint1,mcuPoint2;

    //A(int &x1,int z1) :x(x1), y(z), z(z1) {}
	Graph graph(setOfconfig);

	vector<char> vecNode = {'A','C','D','E','F','H','I'};

    char buffer[512];   
	getcwd(buffer,512);
	string rootPath(buffer);

	/*
	graph.AL_GraphInitial(
	                       "C://FAB02SwitchConfigTool//vex1.txt",
						   "C://FAB02SwitchConfigTool//i2s1_config.txt",
						   0,
						   vexMap0_bclk,
						   vexMap1_bclk,
						   weightMap2,
						   setOfconfig

						   );
	*/
	graph.AL_GraphInitial(
	                       rootPath+"\\vex1.conf",
						   rootPath+"\\i2s1_config.conf",
						   0,
						   vexMap0_bclk,
						   vexMap1_bclk,
						   weightMap2,
						   setOfconfig

						   );	
	graph.AL_GraphPrint();
	//graph.AL_BFS(0);
	graph.AL_DFS(0);

	///TODO:取消大小写
	if((configparamter[3] == "pdm:UIF"))
		    switchVec.push_back("T['31']=1");

	///TODO
	if((configparamter[3] == "pdm:FM36")&& (configparamter[4] == "sync"))
	    {
			cout << "Configuration conflict between:"<<endl;
			cout <<configparamter[3]<<configparamter[4]<<endl;
		}

	parameter = split(configparamter[0], ":");
	if( parameter.empty())  return;

    vector<string> syncOption =  split(configparamter[4], ":");
	if( (syncOption.size() >= 2) && ("sync" == syncOption[0]) && ( "All" ==   syncOption[1]) )  return;

	if( "i2s1" != parameter[0]) return;

	if( "rx_master/tx_master" == parameter[1] ) 
	{
		cout<< "I2S1:[RX:M-TX:M]"<<endl;

		pathStart1 = "CODEC_BCLK[0]";         //k
		pathStart2 = "CODEC_BCLK[1]";         //v
		pathEnd1 = "UIF_BCLK[0]";             //m
		pathEnd2 = "UIF_BCLK[1]";             //n
		mcuPoint1 = "SSC_BCLK[1]";            //x
		mcuPoint2 = "SSC_BCLK[0]";	          //y

		string statPoint1 = get_node_shortname( vexMap0_bclk,pathStart1);
    	string statPoint2 = get_node_shortname( vexMap0_bclk,pathStart2);
    	string endPoint1 = get_node_shortname( vexMap0_bclk,pathEnd1);
    	string endPoint2 = get_node_shortname( vexMap0_bclk,pathEnd2);	
    	string mcu1 = get_node_shortname( vexMap0_bclk,mcuPoint1);
    	string mcu2 = get_node_shortname( vexMap0_bclk,mcuPoint2);		

		int k = findIndex( vecNode,  statPoint1[0]) ;
		int v = findIndex( vecNode,  statPoint2[0]) ;

   		int m = findIndex( vecNode,  endPoint1[0]) ;
		int n = findIndex( vecNode,  endPoint2[0]) ;

		int x = findIndex( vecNode,  mcu1[0]) ;
		int y = findIndex( vecNode,  mcu2[0]) ;

    	graph.AL_FindPath("uif0",k,m);  
		graph.AL_FindPath("uif1",v,n); 
		graph.AL_FindPath("mcu0",k,x); 
		graph.AL_FindPath("mcu1",v,y); 

        setRegSwitch( "uif0" );
		setRegSwitch( "uif1" );

        setRegSwitch( "mcu0" );
		setRegSwitch( "mcu1" );	   

		if((configparamter.size()>=5)  &&( (configparamter[4] == "sync:ALL")||(configparamter[4] == "sync:I2s1")) )
		{
			for( int i = 0; i<switchVec.size();i++)
			{
				if( "T['5']=0" == switchVec[i])
				{
					auto iter = switchVec.erase(switchVec.begin() + i);
				}
			}
			switchVec.push_back("T['5']=1");
		}


	}
	else if("rx_slave/tx_slave" == parameter[1] )
	{
		//BCLKTX 和PDMCLKI是复用的
		cout<< "I2S1:[RX:S-TX:S]"<<endl;

		if((configparamter.size()>=4)  && (configparamter[3] == "pdm:FM36"))
		{
			cout << "Error:No clock provided to FM36";
			return;
		}
	}
	else if("rx_master/tx_slave" == parameter[1] )
	{
		//I2s1_TX_S_RX_M
		cout<< "I2S1:[TX:S-RX:M]"<<endl;

		pathStart1 = "CODEC_BCLK[0]";
		pathEnd1 = "UIF_BCLK[0]";
		pathEnd2 = "SSC_BCLK[1]";

		pathStart2 = "UIF_BCLK[1]";
		mcuPoint1 = "SSC_BCLK[0]";


		string statPoint1 = get_node_shortname( vexMap0_bclk,pathStart1);
		string endPoint1 = get_node_shortname( vexMap0_bclk,pathEnd1);		
    	string endPoint2 = get_node_shortname( vexMap0_bclk,pathEnd2);

    	string statPoint2 = get_node_shortname( vexMap0_bclk,pathStart2);
    	string endPoint3 = get_node_shortname( vexMap0_bclk,mcuPoint1);	

		int k = findIndex( vecNode,  statPoint1[0]) ;
		int v = findIndex( vecNode,  endPoint1[0]) ;
		int r = findIndex( vecNode,  endPoint2[0]) ;

   		int m = findIndex( vecNode,  statPoint2[0]) ;
		int n = findIndex( vecNode,  endPoint3[0]) ;	

		graph.AL_FindPath("codec0touif",k,v);  
		graph.AL_FindPath("codec0tomcu",k,r); 

		graph.AL_FindPath("uif1tossc0",m,n); 	

		setRegSwitch( "codec0touif" );
		setRegSwitch( "codec0tomcu" );
		setRegSwitch( "uif1tossc0" );		

	}
	else if("rx_slave/tx_master" == parameter[1] )
	{
		cout<< "I2S1:[RX:S-TX:M]"<<endl;

		pathStart1 = "UIF_BCLK[0]";
		pathEnd1 = "SSC_BCLK[1]";

		pathStart2 = "CODEC_BCLK[1]";
		pathEnd2 = "UIF_BCLK[1]";
		mcuPoint1 = "SSC_BCLK[0]";

		if((configparamter.size()>=4)  && (configparamter[3] == "pdm:FM36"))
		{
			cout << "Error:No clock provided to FM36";
			return;
		}

		string statPoint1 = get_node_shortname( vexMap0_bclk,pathStart1);
		string endPoint1 = get_node_shortname( vexMap0_bclk,pathEnd1);		


    	string statPoint2 = get_node_shortname( vexMap0_bclk,pathStart2);
		string endPoint2 = get_node_shortname( vexMap0_bclk,pathEnd2);
    	string endPoint3 = get_node_shortname( vexMap0_bclk,mcuPoint1);	

   		int m = findIndex( vecNode,  statPoint1[0]) ;
		int n = findIndex( vecNode,  endPoint1[0]) ;

		int k = findIndex( vecNode,  statPoint2[0]) ;
		int v = findIndex( vecNode,  endPoint2[0]) ;
		int r = findIndex( vecNode,  endPoint3[0]) ;

		graph.AL_FindPath("codec1touif",k,v);  
		graph.AL_FindPath("codec1tomcu",k,r); 

		graph.AL_FindPath("uif0tocodec0",m,n); 			

		setRegSwitch( "codec1touif" );
		setRegSwitch( "codec1tomcu" );
		setRegSwitch( "uif0tocodec0" );	

	}

	weightMap2.clear();

}

void process_dublicate(std::vector<string>& switchVec)
{
	std::vector<string> newSubSwitch;
	set<string> int_set(switchVec.begin(), switchVec.end());
    switchVec.assign(int_set.begin(), int_set.end());

	for( int j=0;j<switchVec.size(); j++ )
	{
		std::regex switchPattern("\\d{2}");
		std::regex resultPattern("=\\d{1}");
		std::smatch matchResult1,matchResult2;

		bool isSwitchMatch1 = regex_search(switchVec[j], matchResult1, switchPattern);
        bool isSwitchMatch2 = regex_search(switchVec[j], matchResult2, resultPattern);

		if( isSwitchMatch1 )
		{
			int switchIndex = atoi(matchResult1[0].str().c_str());
			if( ( switchIndex >= 22 ) && ( switchIndex <= 27 ) )
			{
				switchIndex -= 10;
				string newSwitchName="T['";
				newSwitchName = newSwitchName+std::to_string(switchIndex)+"']";
				newSwitchName += matchResult2[0].str(); 
				newSubSwitch.push_back(newSwitchName);
			}
		}		

	}
	switchVec.insert(switchVec.end(),newSubSwitch.begin(),newSubSwitch.end());

	set<string>switchSet(switchVec.begin(), switchVec.end());
    switchVec.assign(switchSet.begin(), switchSet.end());

	if( switchVec.size() == 0 )
	{
	    cout << "This case only needs to use the default switch configuration"<<endl;
		
	}
	switchVec.push_back("T['21']=1");
}

void save_process_result(string outfileName)
{
	vector<string>::iterator it;
	ofstream outfile(outfileName, ios::app);
	for (it = switchVec.begin(); it != switchVec.end(); )
	{
		std::string s("X"); 

		if (*it == s)
		{
			it = switchVec.erase(it);
		}
		else
		{
			std::regex filterSwitchPattern("=1");
			std::smatch filterMatchResult;

		    bool isFilterSwitchMatch = regex_search(*it, filterMatchResult, filterSwitchPattern);
			if( isFilterSwitchMatch )
			{
			    cout<<*it<<endl;
			    outfile << *it; 
        	    outfile << endl;
			}
			++it;
		}
	}
	outfile.close();
}

std::vector<string> process_config_parameter(string s)
{
	std::vector<string> paramter;
	
	string blank("");
	std::regex parameterPattern1("i2s\\d{1}");
	std::smatch matchParameter1,matchParameter2,matchParameter3,matchParameter4,matchParameter5,matchParameter6;
    paramter = split(s, ",");
	bool isParameterMatch1 = regex_search(paramter[0], matchParameter1, parameterPattern1);
	string item = matchParameter1[0].str();
	if((!isParameterMatch1) || matchParameter1[0] != "i2s1" )
	  paramter.insert(paramter.begin(), blank); 

	if(paramter.size() < 2)
		paramter.insert(paramter.end(), blank); 
	bool isParameterMatch2 = regex_search(paramter[1], matchParameter2, parameterPattern1);
	if((!isParameterMatch2) || matchParameter2[0] != "i2s2" )
	  paramter.insert(paramter.begin()+1, blank); 

	if(paramter.size() < 3) paramter.insert(paramter.end(), blank ); 
	bool isParameterMatch3 = regex_search(paramter[2], matchParameter3, parameterPattern1);
	if((!isParameterMatch3) || matchParameter3[0] != "i2s3" )
	  paramter.insert(paramter.begin()+2, blank ); 

	if(paramter.size() < 4) paramter.insert(paramter.end(), blank ); 
	std::regex parameterPattern2("pdm:");
	bool isParameterMatch4 = regex_search(paramter[3], matchParameter4, parameterPattern2);
	if((!isParameterMatch4) || matchParameter4[0] != "pdm:" )
	  paramter.insert(paramter.begin()+3, blank ); 

	if(paramter.size() < 5) paramter.insert(paramter.end(), blank ); 
	std::regex parameterPattern3("sync");
	bool isParameterMatch5 = regex_search(paramter[4], matchParameter5, parameterPattern3);
	if((!isParameterMatch5) || matchParameter5[0] != "sync" )
	  paramter.insert(paramter.begin()+4, blank ); 	

	if(paramter.size() < 6) paramter.insert(paramter.end(), blank ); 
	std::regex parameterPattern4("cross");
	bool isParameterMatch6 = regex_search(paramter[5], matchParameter6, parameterPattern4);
	if((!isParameterMatch6) || matchParameter6[0] != "cross" )
	  paramter.insert(paramter.begin()+5, blank ); 	

	return  paramter; 
}

#define RELEASE 0
int main( int   argc, char*   argv[] )
{
	#if RELEASE
	if( argc < 2)
	{
			cout << "Usage:" << endl;
			cout << "The I2S port number starts from 1, i2s1, i2s2, i2s3"<<endl;
			cout << "[i2s1:{{rx_master|rx_slave}/{tx_master|tx_slave}],"<<endl;
			cout << "[i2s2:{master|slave}],"<<endl;
			cout << "[i2s3:{master|slave}],"<<endl;
			cout << "[pdm:{UIF|FM36}],"<<endl;
			cout << "[sync:{ALL|I2S1}}],"<<endl;
			cout << "[cross:{Yess|No}]"<<endl;
			cout << "RX and TX of i2s1 need to be configured separately, while i2s2 and i2s3 do not."<<endl;
			cout << "i2s1 rx config option:rx_master or rx_slave. tx config option:tx_master or tx_slave."<<endl;
			cout << "i2s2 config option:master or slave"<<endl;
			cout << "pdm config option:UIF or FM36" <<endl;
			cout << "sync config options:ALL or I2s1.ALL:Both i2s1/i2s2/i2s3 use BCLK and FCLK output by codec0"<<endl;
			cout << "                    I2s1:i2s1 Rx/TX of i2s1 uses the same clock"<<endl;
			cout << "cross config option: Yes or No.Set whether i2s2 and i2s3 intersect"<<endl;
			cout << "example1:i2s1:rx_master/tx_slave,i2s2:master,i2s3:master,pdm:UIF,cross:Yes"<<endl;
			cout << "example2:i2s1:rx_slave/tx_master,i2s3:master,pdm:UIF"<<endl;		
			exit( 1 );
	}
	string s=string(argv[1]);
	#else
	//string s=string("i2s1:rx_slave/tx_master,i2s2:master,i2s3:master,pdm:UIF,sync:I2s1,cross:No");
	//string s=string("sync:All");
	string s=string("i2s1:rx_master/tx_slave,i2s2:master,i2s3:master,pdm:UIF,cross:Yes");
	#endif

	char buffer[512];   
	getcwd(buffer,512);
	string rootPath(buffer);
	cout << rootPath <<endl;	

	std::vector<string> configparamter;

	if(s.empty())  
	{
		cout << "Without parameter,input it please."<<endl;
		return 0;
	}


	configparamter = process_config_parameter(s);

	init_vexMap0_bclk();		
	init_vexMap1_bclk();	


	process_sync_option( configparamter );
	process_i2s1_bclk( configparamter );
	process_i2s23_bclk( configparamter );
	process_dublicate( switchVec );
	save_process_result("C://FAB02SwitchConfigTool//switchConf.txt");
}
