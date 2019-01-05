/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "create_daemon.h"
#include "watch.h"

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

void sig_callback( int sig )
{
    run = false;
}



void process_check_for_update(string mount_loc){
// daemon code
   syslog (LOG_NOTICE, "Writing to my Syslog");
    
  // std::map used to keep track of wd (watch descriptors) and directory names
    // As directory creation events arrive, they are added to the Watch map.
    // Directory delete events should be (but currently aren't in this sample) handled the same way.
    Watch watch;

    // watch_set is used by select to wait until inotify returns some data to 
    // be read using non-blocking read.
    fd_set watch_set;

    char buffer[ EVENT_BUF_LEN ];
    string current_dir, new_dir;
    int total_file_events = 0;
    int total_dir_events = 0;

    // Call sig_callback if user hits ctrl-c
    signal( SIGINT, sig_callback );

    // creating the INOTIFY instance
// inotify_init1 not available with older kernels, consequently inotify reads block.
// inotify_init1 allows directory events to complete immediately, avoiding buffering delays. In practice,
// this significantly improves monotiring of newly created subdirectories.
#ifdef IN_NONBLOCK
    int fd = inotify_init1( IN_NONBLOCK );
#else
    int fd = inotify_init();
#endif

    // checking for error
    if ( fd < 0 ) {
        syslog (LOG_NOTICE, "Error : fd < 0");
    }

    // use select watch list for non-blocking inotify read
    FD_ZERO( &watch_set );
    FD_SET( fd, &watch_set );

    // add “./tmp” to watch list. Normally, should check directory exists first
    const char *root = mount_loc.c_str();//"/home/yogiraj/raid1";
    int wd = inotify_add_watch( fd, root, WATCH_FLAGS );
    
    // add wd and directory name to Watch map
    watch.insert( -1, root, wd );

    // Continue until run == false. See signal and sig_callback above.
    while ( run ) {
        // select waits until inotify has 1 or more events.
        // select syntax is beyond the scope of this sample but, don't worry, the fd+1 is correct:
        // select needs the the highest fd (+1) as the first parameter.
        select( fd+1, &watch_set, NULL, NULL, NULL );

        // Read event(s) from non-blocking inotify fd (non-blocking specified in inotify_init1 above).
        int length = read( fd, buffer, EVENT_BUF_LEN ); 
        if ( length < 0 ) {
            syslog (LOG_NOTICE, "Error : length < 0");
        }  
        
        syslog(LOG_NOTICE, "Event");

        // Loop through event buffer
        for ( int i=0; i<length; ) {
            struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
            if ( event->len ) {
                if ( event->mask & IN_IGNORED ) {
                    syslog(LOG_NOTICE, "IN_IGNORED");
                }
                
                if ( event->mask & IN_CREATE ) {
                    current_dir = watch.get(event->wd);
                    if ( event->mask & IN_ISDIR ) {
                	new_dir = current_dir + "/" + event->name;
                	wd = inotify_add_watch( fd, new_dir.c_str(), WATCH_FLAGS );
                	watch.insert( event->wd, event->name, wd );
                        total_dir_events++;
                     	//string s = "New directory " + new_dir + " created";
                        syslog(LOG_NOTICE, "New directory created");
                    } 
                    else {
                        total_file_events++;
                        //string s = "New file " + current_dir + "/" + event->name + " created.";
                        syslog(LOG_NOTICE, "New file created");
                    }
                }
                
                 else if ( event->mask & IN_DELETE ) {
                    if ( event->mask & IN_ISDIR ) {
                        new_dir = watch.erase( event->wd, event->name, &wd );
                        inotify_rm_watch( fd, wd );
                        total_dir_events--;
                      	//string s = "Directory " + new_dir + " deleted";
                        syslog(LOG_NOTICE, "Directory deleted" );
                    } 
                    else {
                        current_dir = watch.get(event->wd);
                        total_file_events--;
			//string s = "File " + current_dir + "/" + event->name + " deleted.";                        
                        syslog(LOG_NOTICE, "File deleted" );
                    }
                }
            }
            
            if(IN_MODIFY)
            	syslog(LOG_NOTICE, "File modified");
            	
            if(IN_MOVED_TO && IN_MOVED_FROM && event->mask)
            	syslog(LOG_NOTICE, "Renamed");	
            	
            if(IN_MOVED_TO && event->mask)
            	syslog(LOG_NOTICE, "Moved to thrash");
            
            // Here, accordingly, the FlexList(s) for disk(s) need(s) to be updated
            	
            i += EVENT_SIZE + event->len;
        }
    }

    // Cleanup
    watch.stats();
    watch.cleanup( fd );
    watch.stats();
    close( fd );
}


void create_daemon(string mount_loc) {
    
    //syslog(LOG_INFO, "Entering Daemon");
    
    //Set our Logging Mask and open the Log
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

    syslog(LOG_NOTICE, "Entering Daemon");
   
    pid_t pid, sid;

   //Fork the Parent Process
    pid = fork();

    if (pid < 0) { exit(EXIT_FAILURE);
    cout << "Error. pid < 0";
    //syslog(LOG_NOTICE, "Err");
    }

   // cout << "Good. pid > 0\n";
    
    //We got a good pid, Close the Parent Process
    if (pid > 0) { exit(EXIT_SUCCESS); }

    //Change File Mask
    umask(0);

    //Create a new Signature Id for our child
    sid = setsid();
    if (sid < 0) { exit(EXIT_FAILURE); syslog(LOG_NOTICE, "problem"); }

    //Change Directory
    //If we cant find the directory we exit with failure.
    if ((chdir("/")) < 0) { exit(EXIT_FAILURE); syslog(LOG_NOTICE, "problem2"); }

    //Close Standard File Descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);


    syslog(LOG_NOTICE, "here2");
    
    //----------------
    //Main Process
    //----------------
    while(true) {
        process_check_for_update(mount_loc);    // Run our Process (Daemon code to check if there are updates at file system level in the mounted raid)
        sleep(60);    //Sleep for 60 seconds
    }

    //Close the log
    closelog ();
    
  ///  return; 
    
    cout<<"done\n";
}