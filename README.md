# RAID-FlexDPDP
A project in C++ that intends to adapt the idea of verifying data integrity using DPDP in the scenario of a RAID system


## Preliminaries :-

Raid array should be created using mdadm before using this application.
The Raid array should be mounted after creation.
(For raid creation and mounting on file system using mdadm, refer https://www.youtube.com/watch?v=IWXBRy3Gg6Y)
Before adding any file/folder to the raid, this application needs to be run once. This will start a daemon (background process) that watches for modifications in the raid at file system level.
For C++ boost library compatibility, it is required to use this application in Linux Ubuntu 12.04.


## Dependencies :-

Install the following packages using terminal with any extra dependencies accepted 
during installation.
1. libssl-dev 
2. libboost-all 
3. libgmp-dev 
4. libantlr-dev 
5. git 

The exact library package names might contain version numbers, and hence may differ 
slightly from what is provided above. 

On Ubuntu 12.04,

sudo apt-get install libssl-dev
sudo apt-get install libboost-all-dev
sudo apt-get install libgmp-dev
sudo apt-get install libantlr-dev
sudo apt-get install git


For “inotify” :-
Make sure libc6 2.3.6 module is installed on your system. If you have a previous version of libc module installed, you will get the following error message while compiling project :-
error: linux/inotify.h: No such file or directory
Check the libc6 version on your system and upgrade it if required.
> sudo dpkg -l libc6


## Usage :-

Go inside main directory of project.

> make

> cd dist/Debug/GNU-Linux

> sudo ./project <raid-name>


## Files in the project :-

1. main.cpp
Main driver program file

2. mdstat_entry.h	mdstat_entry.cpp	
Class for information in /proc/mdstat file

3. read_mdtat.cpp
To read metadata of raid from /proc/mdstat file

4. build_flexlist.h	build_flexlist.cpp	
check_flexlist_created() checks if FlexList for that disk is already created by checking existence of directory /etc/raid-dpdp/FlexLists/<raid_name>
build_flexlist() builds FlexList for the disk
write_flexlist_root_hash_value() writes root hash value as metadata in file /etc/raid-dpdp//RootMetaData/<raid-name>/<diskname>

5. build_challenge_prove_verify.h	build_challenge_prove_verify.cpp
	build_challenge_prove_verify() is called for each disk. This eventually calls function build_flexlist() in build_flexlist.cpp to build flexlist, then creates challenges, generates proof and verifies the proof.
	This function uses SkipList::build(), DPDPFlexVerifier::generateChallenges(), DPDPFlexProver::GenMultiProof(), DPDPFlexProver::gemBlockSum(), DPDPFlexVerifier::verifyTags() and DPDPFlexVerifier::verifyMultiProof().
All these functions are in “cashlib” directory and are part of Ertem's FlexDPDP library. These have been modified wherever required.

6. get_mount_location.cpp
Has function to get the mount location of the raid array. It is used to create daemon to notify whenever there are updates in the raid at file system level.

7. check_daemon_existence.cpp
It checks if daemon for notifying for updates in raid is already running by using the name of process that created it.
It checks for all processes running. Inside /proc directory, there are directories for all running processes with directory name same as pid of process. It checks in all directories' “stat” file. Stat file has the name of process. So, if the process name in stat matches with process name provided, it means it is the updating daemon or it is the same user application program that is executing. To distinguish between them, we check its ppid (parent pid). Ppid = 1 means that process's parent is init process (pid=1). Daemons have ppid = 1, so we check that too.

Note : A process can become a child of the init process, (NOTE: that init process is PID 1), when their parent is killed or disowns them, these processes are not necessarily daemons, but will still show up as having their PPID equal to 1.

8. create_daemon.h		create_daemon.cpp
create_daemon() :- This creates daemon if check_daemon_existence() returns false. It then calls process_check_for_update() as the daemon code

9. process_check_for_update() :- This function uses “inotify” on mount location to check for updates. If update (file/directory creation/deletion/modification) is there, event is triggered. 
Currently, those events are logged in “syslog file” (/var/log/syslog in ubuntu 12.04) and can be seen by “tail -n 10 /var/log/syslog”.  In this function, there code for FlexList updation on appropriate event (insertion/deletion/modification in list).


All other files in the project are either part of Cashlib Cryptographic library or part of FlexDPDP library.

