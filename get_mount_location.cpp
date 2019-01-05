/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
//#include<stdio>
#include<iostream>
#include<cstring>

using namespace std;


string get_mount_location(string dev_name) {
    
    string dev_name2 = "/dev/" + dev_name;
    string line, name, mount_loc;
    
    ifstream infile("/proc/mounts");

    while(getline(infile, line))
    {
        stringstream ss(line);
        
        ss >> name;
        
        if(name != dev_name2)
        {   continue;
        }
        
        ss >> mount_loc;
        return mount_loc;
    }
    
    mount_loc = "RAID not mounted on file system. Mount it first and then run this application to monitor\n";
    return mount_loc;
}