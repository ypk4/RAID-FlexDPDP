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



using std::map;
using std::string;
using std::cout;
using std::endl;
using namespace std;


class Watch {
    struct wd_elem {
        int pd;
        string name;
        bool operator() (const wd_elem &l, const wd_elem &r) const
            { return l.pd < r.pd ? true : l.pd == r.pd && l.name < r.name ? true : false; }
    };
    map<int, wd_elem> watch;
    map<wd_elem, int, wd_elem> rwatch;
    
public:
    // Insert event information, used to create new watch, into Watch object.
    void insert( int pd, const string &name, int wd );
    
    // Erase watch specified by pd (parent watch descriptor) and name from watch list.
    // Returns full name (for display etc), and wd, which is required for inotify_rm_watch.
    string erase( int pd, const string &name, int *wd );
    
    // Given a watch descriptor, return the full directory name as string. Recurses up parent WDs to assemble name, 
    // an idea borrowed from Windows change journals.
    string get( int wd ) ;
    
    // Given a parent wd and name (provided in IN_DELETE events), return the watch descriptor.
    // Main purpose is to help remove directories from watch list.
    int get( int pd, string name ) ;
    
    void cleanup( int fd ) ;
        
    void stats();
};

