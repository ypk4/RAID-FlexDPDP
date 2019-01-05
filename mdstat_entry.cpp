/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"mdstat_entry.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;



mdstat_entry::mdstat_entry(string devnm, int active, string level, string pattern, int percent, int resync, int devcnt, int raid_disks, string metadata_version, int no_of_blocks, 
            vector<string> member_devices) :
    devnm(devnm), active(active), level(level), pattern(pattern), percent(percent), resync(resync), devcnt(devcnt), raid_disks(raid_disks), 
            metadata_version(metadata_version), no_of_blocks(no_of_blocks), member_devices(member_devices) {
    }

        
int mdstat_entry::GetActive() const {
        return active;
}

void mdstat_entry::SetActive(int active) {
        this->active = active;
}

int mdstat_entry::GetDevcnt() const {
        return devcnt;
}

void mdstat_entry::SetDevcnt(int devcnt) {
        this->devcnt = devcnt;
}

string mdstat_entry::GetDevnm() const {
        return devnm;
}

void mdstat_entry::SetDevnm(string devnm) {
        this->devnm = devnm;
}

string mdstat_entry::GetLevel() const {
        return level;
}

void mdstat_entry::SetLevel(string level) {
        this->level = level;
}

vector<string> mdstat_entry::GetMember_devices() const {
        return member_devices;
}

void mdstat_entry::SetMember_devices(vector<string> member_devices) {
        this->member_devices = member_devices;
}

string mdstat_entry::GetMetadata_version() const {
        return metadata_version;
}

void mdstat_entry::SetMetadata_version(string metadata_version) {
        this->metadata_version = metadata_version;
}

string mdstat_entry::GetPattern() const {
        return pattern;
}

void mdstat_entry::SetPattern(string pattern) {
        this->pattern = pattern;
}

int mdstat_entry::GetPercent() const {
        return percent;
}

void mdstat_entry::SetPercent(int percent) {
        this->percent = percent;
}

int mdstat_entry::GetRaid_disks() const {
        return raid_disks;
}

void mdstat_entry::SetRaid_disks(int raid_disks) {
        this->raid_disks = raid_disks;
}

int mdstat_entry::GetNo_of_blocks() const {
        return no_of_blocks;
}

void mdstat_entry::SetNo_of_blocks(int no_of_blocks) {
    this->no_of_blocks = no_of_blocks;
}


int mdstat_entry::GetResync() const {
        return resync;
}

void mdstat_entry::SetResync(int resync) {
        this->resync = resync;
}

