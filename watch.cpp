/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


// Watch class keeps track of watch descriptors (wd), parent watch descriptors (pd), and names (from event->name).
// The class provides some helpers for inotify, primarily to enable recursive monitoring:
// 1. To add a watch (inotify_add_watch), a complete path is needed, but events only provide file/dir name with no path.
// 2. Delete events provide parent watch descriptor and file/dir name, but removing the watch (infotify_rm_watch) needs a wd.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/inotify.h>

#include <signal.h>
#include <limits.h>
#include <sys/inotify.h>
#include <iostream>
#include <string>
#include <map>

#include "watch.h"

using std::map;
using std::string;
using std::cout;
using std::endl;
using namespace std;


// Insert event information, used to create new watch, into Watch object.
void Watch::insert( int pd, const string &name, int wd ) {
    wd_elem elem = {pd, name};
    watch[wd] = elem;
    rwatch[elem] = wd;
}
    
// Erase watch specified by pd (parent watch descriptor) and name from watch list.
// Returns full name (for display etc), and wd, which is required for inotify_rm_watch.
string Watch::erase( int pd, const string &name, int *wd ) {
    wd_elem pelem = {pd, name};
    *wd = rwatch[pelem];
    rwatch.erase(pelem);
    const wd_elem &elem = watch[*wd];
    string dir = elem.name;
    watch.erase(*wd);
    return dir;
}
    
// Given a watch descriptor, return the full directory name as string. Recurses up parent WDs to assemble name, 
// an idea borrowed from Windows change journals.
string Watch::get( int wd ) {
    const wd_elem &elem = watch[wd];
    return elem.pd == -1 ? elem.name : this->get(elem.pd) + "/" + elem.name;
}
    
// Given a parent wd and name (provided in IN_DELETE events), return the watch descriptor.
// Main purpose is to help remove directories from watch list.
int Watch::get( int pd, string name ) {
    wd_elem elem = {pd, name};
    return rwatch[elem];
}
    
void Watch::cleanup( int fd ) {
    for (map<int, wd_elem>::iterator wi = watch.begin(); wi != watch.end(); wi++) {
        inotify_rm_watch( fd, wi->first );
        watch.erase(wi);
    }
    rwatch.clear();
}
    
void Watch::stats() {
        ;// cout << "number of watches=" << watch.size() << " & reverse watches=" << rwatch.size() << endl;
}

