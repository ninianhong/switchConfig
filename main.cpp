
#include<regex>
#include<sstream>
#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>

#include "graph.cpp"



vector<string> split3(const string &str, const char pattern)
{
    vector<string> res;
    stringstream input(str);   
    string temp;

    while(getline(input, temp, pattern))
    {
        res.push_back(temp);
    }
    return res;
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

void process_i2s23_bclk( std::vector<string> configparamter)
{
	vector<string> parameter_i2s2,parameter_i2s3;
	string pathStart1,pathStart2,pathStart3;
	string pathEnd1,pathEnd2,pathEnd3;
	string mcuPoint1,mcuPoint2,mcuPoint3;

	Graph graph;

	vector<char> vecNode = { 'A','B','H','M','C','I','N','R','S','T','F','K','P','G','L','Q'};

	graph.AL_GraphInitial(
	                       "C://FAB02SwitchConfigTool//vex23.txt",
						   "C://FAB02SwitchConfigTool//i2s23_config.txt",
						   0
						   );
	graph.AL_GraphPrint();
	//graph.AL_BFS(0);
	graph.AL_DFS(0);

	parameter_i2s2 = split(configparamter[1], ":");
	parameter_i2s3 = split(configparamter[2], ":");

	if(parameter_i2s2.empty() && parameter_i2s3.empty()) return;

	//if(( "i2s2" != parameter_i2s2[0]) && ( "i2s3" != parameter_i2s2[0]))
	//	  return;
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
		switchVec.push_back("T['4']=1");
		switchVec.push_back("T['3']=1");
		switchVec.push_back("T['2']=1");		
	}
}

void process_i2s1_bclk( std::vector<string> configparamter)
{
	vector<string> parameter;
	string pathStart1,pathStart2;
	string pathEnd1,pathEnd2;
	string mcuPoint1,mcuPoint2;

	Graph graph;

	vector<char> vecNode = {'A','C','D','E','F','H','I'};

	graph.AL_GraphInitial(
	                       "C://FAB02SwitchConfigTool//vex1.txt",
						   "C://FAB02SwitchConfigTool//i2s1_config.txt",
						   0
						   );
	graph.AL_GraphPrint();
	//graph.AL_BFS(0);
	graph.AL_DFS(0);


	if((configparamter[3] == "pdm:UIF"))
		    switchVec.push_back("T['31']=1");

	if((configparamter[3] == "pdm:FM36")&& (configparamter[4] == "sync"))
	    {
			cout << "Configuration conflict between:"<<endl;
			cout <<configparamter[3]<<configparamter[4]<<endl;
		}

	parameter = split(configparamter[0], ":");
	if( parameter.empty())  return;

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

		if((configparamter.size()==5)  && (configparamter[4] == "sync"))
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

}

#define RELEASE 0
int main( int   argc, char*   argv[] )
{
	#if RELEASE
	if( argc < 2)
	{
			cout << "Usage:" << endl;
			cout << "[i2s1:{{rx_master|rx_slave}/{tx_master|tx_slave}],"<<endl;
			cout << "[i2s2:{master|slave}],"<<endl;
			cout << "[i2s3:{master|slave}],"<<endl;
			cout << "[pdm:{UIF|FM36}],"<<endl;
			cout << "[SYNC]"<<endl;
			cout << "RX and TX of i2s1 need to be configured separately, while i2s2 and i2s3 do not."<<endl;
			cout << "i2s1 rx config option:rx_master or rx_slave. tx config option:tx_master or tx_slave."<<endl;
			cout << "i2s2 config option:master or slave"<<endl;
			cout << "pdm config option:UIF or FM36" <<endl;
			cout << "Additional options:sync or no"<<endl;
			exit( 1 );
	}
	string s=string(argv[1]);
	#else
	string s=string("i2s1:rx_slave/tx_master,i2s2:slave,i2s3:master,pdm:UIF");
	#endif

	if(s.empty())  
	{
		cout << "Without parameter,input it please."<<endl;
		return 0;
	}

	std::vector<string> configparamter;
	std::vector<string> newSubSwitch;

	string blank("");
	std::regex parameterPattern1("i2s\\d{1}");
	std::smatch matchParameter1,matchParameter2,matchParameter3,matchParameter4,matchParameter5;
    configparamter = split(s, ",");
	bool isParameterMatch1 = regex_search(configparamter[0], matchParameter1, parameterPattern1);
	string item = matchParameter1[0].str();
	if((!isParameterMatch1) || matchParameter1[0] != "i2s1" )
	  configparamter.insert(configparamter.begin(), blank); 

	if(configparamter.size() < 2)
		configparamter.insert(configparamter.end(), blank); 
	bool isParameterMatch2 = regex_search(configparamter[1], matchParameter2, parameterPattern1);
	if((!isParameterMatch2) || matchParameter2[0] != "i2s2" )
	  configparamter.insert(configparamter.begin()+1, blank); 

	if(configparamter.size() < 3) configparamter.insert(configparamter.end(), blank ); 
	bool isParameterMatch3 = regex_search(configparamter[2], matchParameter3, parameterPattern1);
	if((!isParameterMatch3) || matchParameter3[0] != "i2s3" )
	  configparamter.insert(configparamter.begin()+2, blank ); 

	if(configparamter.size() < 4) configparamter.insert(configparamter.end(), blank ); 
	std::regex parameterPattern2("pdm:");
	bool isParameterMatch4 = regex_search(configparamter[3], matchParameter4, parameterPattern2);
	if((!isParameterMatch4) || matchParameter4[0] != "pdm:" )
	  configparamter.insert(configparamter.begin()+3, blank ); 

	if(configparamter.size() < 5) configparamter.insert(configparamter.end(), blank ); 
	std::regex parameterPattern3("sync");
	bool isParameterMatch5 = regex_search(configparamter[4], matchParameter5, parameterPattern3);
	if((!isParameterMatch5) || matchParameter5[0] != "sync" )
	  configparamter.insert(configparamter.begin()+4, blank ); 	  	  

	process_i2s1_bclk( configparamter );
	weightMap2.clear();
	process_i2s23_bclk( configparamter );

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
			switchIndex -= 10;
			string newSwitchName="T['";
			newSwitchName = newSwitchName+std::to_string(switchIndex)+"']";
			newSwitchName += matchResult2[0].str(); 
			newSubSwitch.push_back(newSwitchName);
		}		

	}
	switchVec.insert(switchVec.end(),newSubSwitch.begin(),newSubSwitch.end());

	set<string>switchSet(switchVec.begin(), switchVec.end());
    switchVec.assign(switchSet.begin(), switchSet.end());

	if( switchVec.size() == 0 )
	    cout << "This case only needs to use the default switch configuration"<<endl;

	vector<string>::iterator it;
	ofstream outfile("C://FAB02SwitchConfigTool//switchConf.txt", ios::app);
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
	//system ("pause");

}
