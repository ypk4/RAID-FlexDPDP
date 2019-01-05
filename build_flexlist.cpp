/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include<iostream>

#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <sys/stat.h>

#include "GroupRSA.h"
#include "Group.h"
#include "Data.h"

#include "RankBasedSkipList.h"
#include"build_flexlist.h"

#define MAX_TIMERS 20

using namespace std;

// Checks if FlexLists have already been created for the raid
// Checks if the directory /etc/raid-dpdp/FlexLists/<devnm> exists
bool check_flexlist_created(string devnm)
{
    string path = "/etc/raid-dpdp/FlexLists/" + devnm;

    if(opendir(path.c_str()))
        return true;
    
    else
        return false;
}


// Builds FlexList for a disk in raid 
Node* build_flexlist(string devnm, string diskname, ZZ N, ZZ g, ZZ p, ZZ q, int *pno_of_blocks)
{
    // Node* SkipList::build(vector<Data> Blocks, vector<int> Levels, vector<ZZ> Tags, int maxLevel, int order, int partCount;
    
 //   ZZ N, g;
    vector<Data> fileBlocks;
    vector<ZZ> tags;
    vector<int> levels;
    int maxLevel;
    int no_of_blocks;
    ifstream file;
        
   // double* timers = new double[MAX_TIMERS];
   // int timer = 0;
    SetSeed(to_ZZ(rand() % 10000));	
	
    // Generating N, g
   // GroupRSA* group = new GroupRSA("Client", 1024, 80);
   // N = group->getModulus();
  //  g = group->addNewGenerator();
        
  //  *pN = N;
   // *pg = g;

    RankBasedSkipList *rbsl = new RankBasedSkipList(true, N, g);         // true indicates it is a FlexList
    
    string filename = /*"/home/yogiraj/4kbBlock.txt";*/ "/dev/" + diskname;
        
    cout<< "filename=" << filename << endl;
    
    file.open(filename.c_str(), ios :: binary);
    
    if (file.fail())                // check if file is opened successfully
    {  // file opening failed
        cout << "Error Opening file ... " << endl;
        return NULL;
    }
    else 
    {   // proceed with calculating fileBlocks
        char buf[DPDPCONST::blockSize];
              
        ZZ exp = to_ZZ(1); 
        
        while( file.read( &buf[0], DPDPCONST::blockSize) )      // reading 2047 characters (2047+1 bytes = 2kb data) at a time
        {   //buf[readCount] = '\0';
            int readCount;
            
            if((readCount = file.gcount()) != 0)
            {   //buf[readCount] = '\0';
                
                // calculate fileBlocks
                ZZ zzdata;
                ZZFromBytes(zzdata, (unsigned char*)buf, DPDPCONST::blockSize);
            
                Data blockdata(zzdata, readCount);
                fileBlocks.push_back(blockdata);

                //calculate tag for each block
                mpz_mod(MPZ(exp), MPZ(zzdata), MPZ((to_ZZ((p-1) * (q-1)))));
                ZZ tag = PowerMod(g, exp, N);
                
               // cout << "Tag :- " << tag << endl;
                
                tags.push_back(tag);
            }
        }
        
        if(file.eof())
	{
            cout << "file ended\n";
            int readCount;
            if ((readCount = file.gcount()) > 0)
            {
                // Still a few bytes left to write
                cout << "Still a few bytes left to read\n";
                buf[readCount] = '\0';
               
                // calculate fileBlocks
                ZZ zzdata;
                ZZFromBytes(zzdata, (unsigned char*)buf, DPDPCONST::blockSize);
            
                Data blockdata(zzdata, readCount);
                fileBlocks.push_back(blockdata);

                //calculate tag for each block
                mpz_mod(MPZ(exp), MPZ(zzdata), MPZ((to_ZZ((p-1) * (q-1)))));
                ZZ tag = PowerMod(g, exp, N);
                tags.push_back(tag);
            }
	}
        
        file.close();
    }
        
    
    //cout << "here";
    //ZZ data = d->getValue();
    
    no_of_blocks = fileBlocks.size();
    cout << "no of data blocks sent to build FlexList = " << no_of_blocks << endl;
    
    *pno_of_blocks = no_of_blocks;
    
    //levels
    maxLevel= 0;
    int coin = 0;
    srand( DPDPCONST::Seed );
    
    for(int i = 0; i < no_of_blocks; i++) {
    	coin = 0;
        
        while((rand() % 2) == 1) {
            coin++;
        }
        
        if(coin > maxLevel) 
            maxLevel = coin;
        
        levels.push_back(coin);
    }
    
    cout << "\nBuilding FlexList ..\n";
    Node *root = rbsl->build(fileBlocks, levels, tags, maxLevel);
    
    // SkipList::build() somehow calculates an incorrect root hash value.. So, next statement is used
    
    rbsl->getRoot()->calculateHash(N, g);                   
    
    cout << "After calculateHash(), root hash value = " << rbsl->getRoot()->getHash() << endl;
    
   // rbsl->printRB();
    
    cout<<"size of list :  " << rbsl->getSize() <<endl;//    rbsl->printRB();
    
//    cout << "Hash: " << rbsl->getRoot()->getHash() << endl;
    cout << "Hash: " << root->getHash() << endl;
    cout << "N "<< N << endl;
    cout << "0th Tag "<< tags[0] << endl;
    //cout << "Hash "<<hashOfsl <<endl;
    cout << "size of N " << sizeof(N) << endl;
    cout << "size of Tag " << sizeof(tags[0]) << endl;
    cout << "size of Hash " << sizeof(root->getHash()) << endl;
    
    cout << endl;
    
    /*cout << "While Building, file blocks  : " << endl;
    
    for(int i = 0; i < fileBlocks.size(); i++)
    {   cout << fileBlocks[i].getValue() << endl;
        cout << endl;
    }
    cout << endl;
    
    cout << "While Building, tags  : " << endl;
    
    for(int i = 0; i < tags.size(); i++)
    {   cout << tags[i] << endl;
        cout << endl;
    }
    cout << endl;*/
    
    return root;
}


// Build FlexLists for all disks in raid
/*vector<Node *> build_flexlists_for_disks(string devnm, vector<string> member_devices)
{
    long unsigned int length = member_devices.size(), i;
    vector<Node *> roots;
    
    for(i = 0; i < length; i++)
    {
        string diskname = member_devices[i].substr(0, member_devices[i].find('['));
        cout << "\ndiskname = " << diskname << endl;
//        Node* root = build_flexlist(devnm, diskname);
 //       roots.push_back(root);
    }
    
    return roots;
}*/


// Write FlexList root hash value as metadata in file /etc/raid-dpdp/RootMetaData/<raid-name>/<diskname>
void write_flexlist_root_hash_value(string devnm, string member_device, Node * root)
{
    ofstream file;
    //int no_of_lists = roots.size();
    
    if(opendir("/etc/raid-dpdp") == NULL)
    {    if(mkdir("/etc/raid-dpdp", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
            cout << "Error creating directory raid-dpdp\n";
    }
        
    if(opendir("etc/raid-dpdp/RootMetaData") == NULL)
    {    if(mkdir("/etc/raid-dpdp/RootMetaData", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
            ;//cout << "Error creating directory RootMetaData\n";
    }
        
    const string dir_path = "/etc/raid-dpdp/RootMetaData/" + devnm;
    if(mkdir(dir_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
    {
        ;//cout<<"Error creating directory for raid-name\n";
      //  return;
    }
        
    
    string filename = "/etc/raid-dpdp/RootMetaData/" + devnm + "/" + member_device;
    file.open(filename.c_str());
        
    ZZ rootHashValue = root->getHash();

    file << rootHashValue;

    cout << "Wrote " << rootHashValue << " in file " << filename << endl;
        
    file.close();
}
