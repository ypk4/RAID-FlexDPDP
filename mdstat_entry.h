/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mdstat_entry.h
 * Author: yogiraj
 *
 * Created on June 21, 2017, 11:54 AM
 */

#ifndef MDSTAT_ENTRY_H
#define MDSTAT_ENTRY_H

#include <iostream>
#include <vector>

#define RESYNC_NONE -1
#define RESYNC_DELAYED -2
#define RESYNC_PENDING -3
#define RESYNC_UNKNOWN -4

using namespace std;

class mdstat_entry;

mdstat_entry* get_device_detail(string dev_name);

string get_mount_location(string dev_name);


class mdstat_entry {

private:
	string		devnm;
	int		active;
	string		level;
	string		pattern; 	/* U or up, _ for down */
	int		percent; 	/* -1 if no resync */
	int		resync; 	/* 3 if check, 2 if reshape, 1 if resync, 0 if recovery */
	int		devcnt;
	int		raid_disks;
	string 		metadata_version;
        int             no_of_blocks;

	vector<string> member_devices;

public:

  //  mdstat_entry();
    
    mdstat_entry(string devnm, int active, string level, string pattern, int percent, int resync, int devcnt, int raid_disks, string metadata_version, int no_of_blocks, 
            vector<string> member_devices);

    int GetActive() const ;

    void SetActive(int active);

    int GetDevcnt() const;

    void SetDevcnt(int devcnt);

    string GetDevnm() const ;
    
    void SetDevnm(string devnm);

    string GetLevel() const ;

    void SetLevel(string level);

    vector<string> GetMember_devices() const;

    void SetMember_devices(vector<string> member_devices);

    string GetMetadata_version() const ;

    void SetMetadata_version(string metadata_version);

    string GetPattern() const ;

    void SetPattern(string pattern);

    int GetPercent() const ;

    void SetPercent(int percent);

    int GetRaid_disks() const ;

    void SetRaid_disks(int raid_disks) ;

    int GetResync() const ;

    void SetResync(int resync) ;
    
    int GetNo_of_blocks() const ;
    
    void SetNo_of_blocks(int no_of_blocks);


};





#endif /* MDSTAT_ENTRY_H */

