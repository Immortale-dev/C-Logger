#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <mutex>
#include <chrono>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <iostream>
	
namespace logger{
    
	class Log{
		
		public:
			Log(int allowance=7);
			Log(std::string path, int allowance=7);
			Log(std::string path, std::string name, int allowance=7);
			virtual ~Log();
			virtual void log(int type, std::string str);
			void error(std::string str);
			void warning(std::string str);
			void info(std::string str);
			void flush();
            
            inline static const int L_ERROR=1, L_WARNING=2, L_INFO=4;
            inline static std::unordered_map<std::string, std::ofstream> _fds;
            inline static std::unordered_map<std::string, int> _fds_cnts;
            inline static std::mutex m;
		
		private:
			virtual void create_path(std::string path);
			virtual bool is_allowed(int type);
			virtual std::string form_log_string(int type, std::string str);
			virtual std::string escape(std::string);
			virtual std::string get_time_str();
			void open_stream(std::string path);
			void close_stream(std::string path);
			std::ofstream& get_stream(std::string path);
			void write_log(std::string str);
			
			int allowance;
			std::string path;
			std::string name;
			std::string format = "{\"name\":\"${name}\",\"thread\":\"${thread}\",\"date\":\"${date}\",\"type\":\"${type}\",\"message\":\"${message}\"}\n";
	};
}

#endif // LOGGER_H
