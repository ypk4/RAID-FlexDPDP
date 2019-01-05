/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: yogiraj
 *
 * Created on June 21, 2017, 11:49 AM
 */

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include<iostream>
#include<cstring>

#include"mdstat_entry.h"
#include "create_daemon.h"
#include "watch.h"
#include "build_flexlist.h"
#include "Node.h"
#include "ProofNode.h"

#include"build_challenge_prove_verify.h"

using namespace std;


int main(int argc, char *argv[])
{
    
    if(argc < 2)
    {	cout << "Error : Specify the raid device to monitor\nUsage :- ./raid_dpdp <raid_device_name>\n";
	return 0;
    }
    
    string devnm;
    vector<string> member_devices;
    
    // Getting details of raid from /proc/mdstat file :-
    
    mdstat_entry* md_device = get_device_detail(argv[1]);
    
    if(md_device == NULL)
    {   cout << "Error : could not find raid device.. Provide valid raid device name\n Note:- Do not use /dev/md0, use only md0 instead" << endl;
        return 0;
    }
       
    else
    {   //cout << "Found\n";
    
        cout << "Name=" << md_device->GetDevnm() << " " << "Dev cnt="<<md_device->GetDevcnt() << " " << "Active="<<md_device->GetActive() << " " << "Level="<<md_device->GetLevel() << endl 
                << "Metadata ver="<<md_device->GetMetadata_version() << " " << "Disks=" << md_device->GetRaid_disks() << " " << "Percent=" << md_device->GetPercent() << " " << 
                "Resync=" << md_device->GetResync() << " " << "Pattern =" << md_device->GetPattern() << " " << "Blocks=" << md_device->GetNo_of_blocks() << endl;
        
        cout << "Member devices : " ;
        for(int i= 0; i < md_device->GetMember_devices().size(); i++)
            cout << md_device->GetMember_devices().at(i) << " ";
    
        devnm = md_device->GetDevnm();
        member_devices = md_device->GetMember_devices();
    
        int i = 0, no_of_disks = member_devices.size();

    
        // Check to see if FlexLists for disks in the RAID are already created :-
        
        bool flexlists_created = check_flexlist_created(devnm);
       
        if(!flexlists_created)
        // i) If FlexLists are not already created :-
        {               
            for(i = 0; i < no_of_disks; i++)            // For each disk
            {
                cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";
                cout << "Disk : " << member_devices[i] << endl;

                // Building FlexList, creating challenges, proof generation and verification for that disk
                bool test_result = build_challenge_prove_verify(devnm, member_devices[i], false);           // 3rd argument is are_flexlists_created = false
                
                if(test_result)
                  cout << member_devices[i] << " Verified correctly" << endl;
        
                else
                  cout << member_devices[i] << " Failure" << endl;
            }
        }
        
        else    
        // ii) If FlexLists are already created and stored :-
        {
            cout << "\n\nFlexLists already created and stored as files.. De-serializing the files to objects..\n";
            
            
            
            for(i = 0; i < no_of_disks; i++)
            {
                cout << "\nDisk : " << member_devices[i] << endl;

                bool test_result = build_challenge_prove_verify(devnm, member_devices[i], true);    // not implemented fully for are_flexlists_created = true case
                
                if(test_result)
                  cout << member_devices[i] << " Verified correctly" << endl;
        
                else
                  cout << member_devices[i] << " Failure" << endl;
            }
        }
                
        cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";

        
        // Finding mount location of raid :-
            
        cout << "\nChecking if daemon for updating FlexLists is already running.. \n";
        
        string mount_loc = get_mount_location(devnm);
        cout << "\nMount location of " << devnm << " : " << mount_loc << endl;
               
       
        // Check if daemon that watches mount location directory of raid is already running :-
        
        pid_t pid = check_daemon_existence("project");
        
        if (pid == -1) {
            printf("\nDaemon not running\nCreating daemon..\n" );
            // Create daemon
            create_daemon(mount_loc); 
        } else {
            printf("\nDaemon already running with pid = %d\n",  pid);
        }
        
        cout << "\nIn main\n";
        
        delete md_device;
    }
        
    return 0;
}