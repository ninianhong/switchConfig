
#include<fstream>
#include<regex>
#include<sstream>
#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>

#include "graph.h"
#include "switchConfig.h"

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

	switchSet sSet;
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

	  	  

	sSet.process_i2s1_bclk( configparamter );
	//weightMap2.clear();
	sSet.process_i2s23_bclk( configparamter );

	set<string> int_set(sSet.switchVec.begin(), sSet.switchVec.end());
    sSet.switchVec.assign(int_set.begin(), int_set.end());

	for( int j=0;j<sSet.switchVec.size(); j++ )
	{
		std::regex switchPattern("\\d{2}");
		std::regex resultPattern("=\\d{1}");
		std::smatch matchResult1,matchResult2;

		bool isSwitchMatch1 = regex_search(sSet.switchVec[j], matchResult1, switchPattern);
        bool isSwitchMatch2 = regex_search(sSet.switchVec[j], matchResult2, resultPattern);

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
	sSet.switchVec.insert(sSet.switchVec.end(),newSubSwitch.begin(),newSubSwitch.end());

	set<string>switchSet(sSet.switchVec.begin(), sSet.switchVec.end());
    sSet.switchVec.assign(switchSet.begin(), switchSet.end());

	if( sSet.switchVec.size() == 0 )
	    cout << "This case only needs to use the default switch configuration"<<endl;

	vector<string>::iterator it;
	ofstream outfile("C://FAB02SwitchConfigTool//switchConf.txt", ios::app);
	for (it = sSet.switchVec.begin(); it != sSet.switchVec.end(); )
	{
		std::string s("X"); 

		if (*it == s)
		{
			it = sSet.switchVec.erase(it);
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
