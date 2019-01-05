/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Data.h"
#include <iostream>
#include <fstream>
#include "NTL/ZZ.h"


using namespace std;

NTL_CLIENT


Data::Data() {
    //blockName = (char*)malloc(128 * sizeof(char));
    length = 0;
    orderNumber = 0;
}

Data::Data(int len)
{
    length = len;
}

Data::Data(char * bN, int order) {
    blockName = (char*)malloc(128 * sizeof(char));
    blockName = bN;
    blockNameString = string(bN);
    orderNumber = order;
    
    ifstream theBlock;
    long size;
    theBlock.open (blockName);
    theBlock.seekg (0, ios::end);
    size = theBlock.tellg();
    //		size--;
    char *memblock = new char[size];
    theBlock.seekg (0, ios::beg);
    theBlock.read (memblock, size);
    //This check is done for manually created files in which at the end there is \n charachter
    if(memblock[size-1] == '\n')
    {
        size--;
    }
    delete[] memblock;
    
    length = size;
    
}

Data::Data(vector<char> block, char* bN, int order)
{
    blockName = (char*)malloc(128 * sizeof(char));
    blockNameString = string(bN);
    blockName = bN;
    orderNumber = order;
    
    ofstream outfile(bN, ios::out | ios::binary);
    outfile.write(&block[0], block.size());
    length = block.size();    //FILE UZUNLUGU ILE BLOCK UZUNLUGU AYNI OLMAYABILIR MI? -ayni
    //when this is written by code, there is no \n at the end already
    
    //cout<<"Length of data--"<<length<<endl;
    
}

Data::~Data() {
    value = 0;
}

Data::Data(ZZ d, int len) {
    blockName = (char*)malloc(128 * sizeof(char));
    value = d; length = len;
}

ZZ Data::getValue() { return value; }

ZZ Data::getValuePers()
{
    try
    {
        ZZ value;
        ifstream theBlock;
        long size;
        theBlock.open (blockName);
        theBlock.seekg (0, ios::end);
        size = theBlock.tellg();
        
        //		size--;
        char *memblock = new char[size];
        theBlock.seekg (0, ios::beg);
        theBlock.read (memblock, size);
        
        //This check is done for manually created files in which at the end there is \n charachter
        if(memblock[size-1] == '\n')
        {
            string toTrim = string(memblock);
            toTrim = toTrim.substr(0, size -1);
            memblock = new char[size -1];
            strcpy(memblock, toTrim.c_str());
        }
        
        theBlock.close();
        ZZFromBytes(value, (unsigned char*)memblock, size);
        delete[] memblock;
        
    }
    catch (std::exception& e)
    {
        return to_ZZ(1);
    }
    
    return value;
}

ZZ Data::getValuePers(char* fileNameBase)
{
    
    try
    {
        
        
        char isim[80] = "";
        char str[100];
        sprintf(str, "%d", orderNumber);
        strcat(isim, fileNameBase);
        strcat(isim, str);
        strcat(isim, ".txt");// = fileName + blockCounter + ".txt";
        
        //cout<<"order: "<<orderNumber<<"  "<<isim<<endl;
        
        ZZ value;
        ifstream theBlock;
        long size;
        theBlock.open (isim);
        theBlock.seekg (0, ios::end);
        size = theBlock.tellg();
        //		size--;
        char *memblock = new char[size];
        theBlock.seekg (0, ios::beg);
        theBlock.read (memblock, size);
        
        //This check is done for manually created files in which at the end there is \n charachter
        //XXX:DELETE THIS SEGMENT WHEN THE CODE IS DONE
        /*if(memblock[size-1] == '\n')
         {
         string toTrim = string(memblock);
         toTrim = toTrim.substr(0, size -1);
         memblock = new char[size -1];
         strcpy(memblock, toTrim.c_str());
         }*///////////
        
        theBlock.close();
        ZZFromBytes(value, (unsigned char*)memblock, size);
        delete[] memblock;
        returnVal = value;
    }
    catch (std::exception& e)
    {
        //cout<<"WARNING: a block is missing!"<<endl;
        return to_ZZ(1);
    }
    
    return returnVal;
}
ZZ Data::getValuePersByStringName()
{
    
    try
    {
        
        ZZ value;
        ifstream theBlock;
        long size;
        theBlock.open (blockNameString.c_str());
        theBlock.seekg (0, ios::end);
        size = theBlock.tellg();
        //		size--;
        char *memblock = new char[size];
        theBlock.seekg (0, ios::beg);
        theBlock.read (memblock, size);
        
        //This check is done for manually created files in which at the end there is \n charachter
        //XXX:DELETE THIS SEGMENT WHEN THE CODE IS DONE
        /*if(memblock[size-1] == '\n')
         {
         string toTrim = string(memblock);
         toTrim = toTrim.substr(0, size -1);
         memblock = new char[size -1];
         strcpy(memblock, toTrim.c_str());
         }*///////////
        
        theBlock.close();
        ZZFromBytes(value, (unsigned char*)memblock, size);
        delete[] memblock;
        returnVal = value;
    }
    catch (std::exception& e)
    {
        cout<<"WARNING: getValuePersByStringName() called inside GenMultiProof() failed.. This block is missing!"<<endl;
        return to_ZZ(1);
    }
    
    return returnVal;
}

vector<char> Data::getBytesFromFile() {
    
    std::ifstream testFile(blockName, std::ios::binary);
    vector<char> fileContents((std::istreambuf_iterator<char>(testFile)),
                              std::istreambuf_iterator<char>());
    return fileContents;
}

char* Data::getDataVal(long* s)
{
    ZZ value;
    ifstream theBlock;
    long size;
    theBlock.open (blockNameString.c_str());
    theBlock.seekg (0, ios::end);
    size = theBlock.tellg();
    char *memblock = new char[size];
    theBlock.seekg (0, ios::beg);
    theBlock.read (memblock, size);
    theBlock.close();
    
    //This check is done for manually created files in which at the end there is \n charachter
    if(memblock[size-1] == '\n')
    {
        string toTrim = string(memblock);
        toTrim = toTrim.substr(0, size -1);
        memblock = new char[size -1];
        strcpy(memblock, toTrim.c_str());
    }
    if(s != NULL){
        *s = size;
    }
    return memblock;
}

char* Data::getDataValByStringName(long* s)
{
    ZZ value;
    ifstream theBlock;
    long size;
    theBlock.open (blockNameString.c_str());
    theBlock.seekg (0, ios::end);
    size = theBlock.tellg();
    char *memblock = new char[size];
    theBlock.seekg (0, ios::beg);
    theBlock.read (memblock, size);
    theBlock.close();
    
    //This check is done for manually created files in which at the end there is \n charachter
    if(memblock[size-1] == '\n')
    {
        string toTrim = string(memblock);
        toTrim = toTrim.substr(0, size -1);
        memblock = new char[size -1];
        strcpy(memblock, toTrim.c_str());
    }
    if(s != NULL){
        *s = size;
    }
    return memblock;
}

int Data::getLength() { return length; }
void Data::setLength(int len) { length = len; }

char* Data::getName(){
    return blockName;
}
string Data::getNameAsString(){
    return blockNameString;
}

int Data::readCounterInt(char* inname)
{
    int i;
    ifstream infile(inname);
    if (!infile) {
        return 1;
    }
    
    while (infile >> i) {
    }
    infile.close();
    return i;
}