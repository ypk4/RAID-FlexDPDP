#ifndef _logger_H_
#define _logger_H_

#include <ctime>
#include <time.h>
#include <fstream> 
#include <string> 

using namespace std;


class logger {
public:
	static void log(const char*msg)
	{
		//time_t now = time(0);
		//struct tm* tm = localtime(&now);
		ofstream out( "logFile.txt" ,ios::app);
		/*out << tm->tm_year 
			<< "." 
			<< tm->tm_mon + 1 
			<< "." 
			<< tm->tm_mday
			<< " " 
			<< tm->tm_hour 
			<< ":" 
			<< tm->tm_min 
			<< ":" 
			<< tm->tm_sec 
			<< ": "; */
		out << msg << endl;
		out.close();
	}
};
#endif /*_logger_H_*/