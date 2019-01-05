#ifndef _Data_H_
#define _Data_H_
#include <iostream>
#include <fstream>
#include "NTL/ZZ.h"


using namespace std;

NTL_CLIENT

class Data {
    
public:
    
    Data();
    Data(int len);
    Data(char * bN, int order = 0);
    Data(vector<char> block, char* bN, int order = 0);
    ~Data();
    Data(ZZ d, int len = 1);
    
    ZZ getValue();
    ZZ getValuePers();
    ZZ getValuePers(char* fileNameBase);
    ZZ getValuePersByStringName();
    vector<char> getBytesFromFile();
    char* getDataVal(long* s = NULL);
    char* getDataValByStringName(long* s = NULL);
    
    int getLength();
    void setLength(int len);
    char* getName();
    string getNameAsString();
    
    static int readCounterInt(char* inname);
    
    int length;
    
private: 
    
    ZZ returnVal;
    ZZ value;
    char* blockName;
    string blockNameString;
    int orderNumber;
    
};

#endif /*_Data_H_*/