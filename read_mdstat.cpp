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
#include"mdstat_entry.h"
#include<ctype.h>

using namespace std;



static bool is_blank_line(string line, int n)
{	int i;
	for(i = 0; i < n; i++)
	{	if(isalnum(line[i]))
		{	return false;
		}
	}
	return true;
}


 mdstat_entry* get_device_detail(string dev_name) {
    
    ifstream infile("/proc/mdstat");
	
    string line, line2;
   // bool found = false;
    
    bool file_ended = false, do_read = true;
        
	
    getline(infile, line);

    while(!file_ended)
    {	if(!do_read)
    	{   //cout<<"in if\n";
		
            //cout<<"b4 line ="<< line << endl;
			
            
            line = line2;
			
            //cout<<"after line =" << line << endl;

            do_read = true;
		
	/*		if(file_ended)
			{	done = 1;
			}
				
			else {*/
            while(do_read)
            {	if(!getline(infile, line2))
		{   file_ended = true;
                    do_read = false;					
		}
				
		//cout<<"line2 = " << line2 << endl;
				
		//cout<<"file_ended = " << file_ended << endl;
				
		if(!file_ended)
		{   //cout<< "line2 now= " << line2 << endl;
					
				//	strcpy(tmp, line2);
                    if(is_blank_line(line2, line2.length()))
                    {	//cout<<"Skipped here"<< endl;				// blank line
                       	continue;
                    }
					
                  //  cout<<"tmp= " << tmp << endl;

                    //cout<<"line here=" << line <<endl<<" line2 here= " << line2 << endl;
					
                    //cout<<"line here= " << line <<endl<< " line2 here= " << line2 << endl;
					
                    if(line2[0] == '\t' || line2[0] == ' ')
                    {	line += line2;
                        //cout<<"test\n";
                    
			//cout<<"Before strcat, line= " << line << " Before strcat, line2= " << line2 << endl;
			//cout<<"After strcat, line= " << line << endl;
                    }
                    else
                    	do_read = false;
		}
            }
			//}
        }	

        else if(do_read)
	{   //cout<<"in else if\n";
		
            while(do_read)
            {	if(!getline(infile, line2))
            	{   file_ended = true;
                    do_read = false;
		}
				
		if(!file_ended)
		{	//strcpy(tmp, line2);
                    if(is_blank_line(line2, line2.length()))
                    {	//cout<<"Skipped here\n";
                    	continue;				// blank line
                    }
					
                    if(line2[0] == '\t' || line2[0] == ' ')
                        line += line2;
                    else
			do_read = 0;
		}
            }
	}
		
                
	/*while (getline(infile, line))
	{
            if(line.compare(0, 13, "Personalities") == 0)
                continue;

            if(line.compare(0, 6, "unused") == 0)
                continue;
            
            if(line.compare(0, 10, "read_ahead") == 0)
                continue;
            
            //string str = line.substr(0, 13);
          
            //if(str == "Personalities")

    //        cout << line << endl;
	    
	}*/
        
        if(line.compare(0, 13, "Personalities") == 0)
            continue;

        if(line.compare(0, 6, "unused") == 0)
            continue;
            
        if(line.compare(0, 10, "read_ahead") == 0)
            continue;
            
        
     //   cout<<endl<<"LINE = " << line << endl;
        
        
        
        size_t pos = 0;
        string token;
        
        if(pos = line.find(" : "))
        {
            token = line.substr(0, pos);
       //     cout<<token<<"_done"<<endl;
            
            if(dev_name != token)
                continue;           /* Not this md device. Try on next line */
            
            
            /* Found the md array device - Now, fill in the details and return object */
            
            vector<string> devices;
            string empty = "";
            bool in_devices_list = false;
            int devcnt = 0;

            
            mdstat_entry* ent = new mdstat_entry(dev_name, -1, empty, empty, RESYNC_NONE, 0, 0, 0, empty, 0, devices);
            
            line.erase(0, pos + 3);
            
            cout<<line<<endl;
            
            stringstream ss(line);
            
            while(ss >> token)
            { //   cout << token << endl;
            
                if(token == "active")
                    ent->SetActive(1);
            
                else if(token == "inactive")
                {   ent->SetActive(0);
                    in_devices_list = true;                                                 // list of devices entries to be seen next				
                }
            
                else if (ent->GetActive() > 0 /**/ && ent->GetLevel() == "" /*level not set yet*/ && token[0] != '(' /*readonly*/)
                {   ent->SetLevel(token);
                    in_devices_list = true;                                                 // list of devices entries to be seen next				
                } 
            
                else if (in_devices_list && token == "blocks")
                {	in_devices_list = false;						// devices entries done
                        ent->SetMember_devices(devices);
                        ent->SetDevcnt(devcnt);
                }
	
                else if (in_devices_list)
                {   if(int n = atoi(token.c_str()))
                        ent->SetNo_of_blocks(n);
                
                    else
                    {    size_t ep = token.find('[');
                        if(ep > 0)
                        {   devcnt++;
                            string name = token.substr(0, ep);
                            devices.push_back(name);
                        }
                    }
                } 

                else if (token == "super")
			//	   dl_next(w) != line) 
		{	ss >> token;
		//	w = dl_next(w);
			ent->SetMetadata_version(token);
		}
            
                else if (token[0] == '[' && isdigit(token[1]) && (pos = token.find('/')) != string::npos)           // [n/m] field
                {   ent->SetRaid_disks(atoi(token.substr(1, pos-1).c_str()));                      // 'n' in [n/m] 
                    //cout << token.substr(1, pos-1).c_str() << endl;
		}
            
                else if (ent->GetPattern() == "" && token[0] == '[' && (token[1] == 'U' || token[1] == '_'))         // [UU_] field
		{   
                    size_t pos = token.find(']');
                    ent->SetPattern(token.substr(1, pos-1));
                    
                //    printf("reached\n"); 
		}
            
            
            // Not frequently found in mdstat :-
            
                else if (ent->GetPercent() == RESYNC_NONE && token.find("re") && token[token.length() - 1] == '%' && (pos = token.find('=')) )
                {   ent->SetPercent(atoi(token.substr(pos, token.length() - pos).c_str()));
                    if (token.find("resync"))
			ent->SetResync(1);
                    else if (token.find("reshape"))
			ent->SetResync(2);
                    else
			ent->SetResync(0);
		}
            
            
                else if (ent->GetPercent() == RESYNC_NONE && (token[0] == 'r' || token[0] == 'c'))
                {   if (token.find("resync"))
			ent->SetResync(1);
                    if (token.find("reshape"))
			ent->SetResync(2);
                    if (token.find("recovery"))
			ent->SetResync(0);
                    if (token.find("check"))
			ent->SetResync(3);

                    if (token.length() > 8 && token.find("=DELAYED"))
			ent->SetPercent(RESYNC_DELAYED);
                    if (token.length() > 8 && token.find("=PENDING"))
			ent->SetPercent(RESYNC_PENDING);
		} 
			
		else if (ent->GetPercent() == RESYNC_NONE && token[0] >= '0' &&   token[0] <= '9' &&  token[token.length()-1] == '%') {
                    ent->SetPercent(atoi(token.c_str()));
		}
            
            }
            return ent;
        }
     
        
    }
    
   // if(!found)
    return NULL;
    
}