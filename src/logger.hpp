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
    
    extern std::unordered_map<std::string, std::ofstream> _fds;
    extern std::unordered_map<std::string, int> _fds_cnts;
    extern std::mutex m;
    
    enum class T{ ERROR=1, WARNING=2, INFO=4 };
    
    class Log{
        
        public:
            Log();
            Log(std::string path, int allowance=7);
            Log(std::string path, std::string name, int allowance=7);
            virtual ~Log();
            virtual void log(T type, std::string str);
            void error(std::string str);
            void warning(std::string str);
            void info(std::string str);
        
        private:
			virtual void create_path(std::string path);
			virtual bool is_allowed(T type);
			virtual std::string form_log_string(T type, std::string str);
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
