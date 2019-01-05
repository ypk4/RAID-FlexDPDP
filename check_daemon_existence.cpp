/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <dirent.h>
#include<unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include<string>

using namespace std;


pid_t check_daemon_existence(const char* name) 
{
    DIR* dir;
    struct dirent* ent;
    char buf[512];

    long  pid;
    char pname[100] = {0,};
    char state;
    FILE *fp=NULL; 

    printf("Process name :%s\n", name);

    if (!(dir = opendir("/proc"))) {
        perror("can't open /proc");
        return -1;
    }

    while((ent = readdir(dir)) != NULL) {
        long lpid = atol(ent->d_name);
        int ppid;
        
        if(lpid < 0)
            continue;
        
        snprintf(buf, sizeof(buf), "/proc/%ld/stat", lpid);
        fp = fopen(buf, "r");

        if (fp) {
            if ( (fscanf(fp, "%ld (%[^)]) %c %d", &pid, pname, &state, &ppid)) != 4 ) {
                printf("fscanf failed \n");
                fclose(fp);
                closedir(dir);
                return -1; 
            }
            //if ( fscanf(fp, "%s", pname) ){
             //   printf("fscanf failed \n");
             //   fclose(fp);
              //  closedir(dir);
            //    return -1; 
            //}
      //      printf("$%s$\n", pname);
            
            if (!strcmp(pname, name)) {
                printf("inside\n");
                
                if(ppid == 1) {             // ppid (parent pid) = 1 means it is a system process (daemons are children of init process (pid=1)), so it is our daemon
                    fclose(fp);
                    closedir(dir);
                    return (pid_t)lpid;
                }
            }
            
            fclose(fp);
        }
    }


closedir(dir);
return -1;
}
