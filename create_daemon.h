/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   create_daemon.h
 * Author: yogiraj
 *
 * Created on July 3, 2017, 4:01 PM
 */

#ifndef CREATE_DAEMON_H
#define CREATE_DAEMON_H


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

#define EVENT_SIZE          ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN       ( 1024 * ( EVENT_SIZE + NAME_MAX + 1) )
#define WATCH_FLAGS         ( IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_TO | IN_MOVED_FROM )

using std::map;
using std::string;
using std::cout;
using std::endl;
using namespace std;

#define DAEMON_NAME "vdaemon"

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

// Keep going  while run == true, or, in other words, until user hits ctrl-c
static bool run = true;

void sig_callback( int sig );
void process_check_for_update(string mount_loc);
void create_daemon(string mount_loc);

pid_t check_daemon_existence(const char* name); 

#endif /* CREATE_DAEMON_H */

