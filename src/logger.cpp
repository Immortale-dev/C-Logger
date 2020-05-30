#include "logger.hpp"

namespace logger{
	std::unordered_map<std::string, std::ofstream> _fds;
    std::unordered_map<std::string, int> _fds_cnts;
	std::mutex m;
	const int L_ERROR=1, L_WARNING=2, L_INFO=4;
}

logger::Log::Log() : allowance(7)
{
	path = "./main.log";
	name = "";
	open_stream(path);
}

logger::Log::Log(std::string path, int allowance) : allowance(allowance), path(path) 
{
	name = "";
	open_stream(path);
};

logger::Log::Log(std::string path, std::string name, int allowance) : allowance(allowance), path(path), name(name)
{
	open_stream(path);
}

logger::Log::~Log()
{
	close_stream(path);
}

void logger::Log::log(int type, std::string str)
{
	if(is_allowed(type)){
		write_log(form_log_string(type,str));
	}
}

void logger::Log::error(std::string str)
{
	if(is_allowed(L_ERROR)){
		write_log(form_log_string(L_ERROR,str));
	}
}

void logger::Log::warning(std::string str)
{
	if(is_allowed(L_WARNING)){
		write_log(form_log_string(L_WARNING,str));
	}
}

void logger::Log::info(std::string str)
{
	if(is_allowed(L_INFO)){
		write_log(form_log_string(L_INFO,str));
	}
}

void logger::Log::flush()
{
	std::ofstream& st = get_stream(path);
	st.flush();
}

void logger::Log::create_path(std::string str)
{
	//TODO: create nested folders
}

bool logger::Log::is_allowed(int type)
{
	return (allowance & type);
}

std::string logger::Log::form_log_string(int type, std::string message)
{
	std::string ltype, res, date, thread;
	
	// Define text type
	switch(type){
		case L_ERROR:
			ltype = "error";
			break;
		case L_WARNING:
			ltype = "warning";
			break;
		case L_INFO:
			ltype = "info";
			break;
	}
	
	// Form Date
	date = get_time_str();
	
	// Form thread id
	std::stringstream ss;
	ss << std::this_thread::get_id();
	thread = ss.str();
	
	// Build result string
	res = "";
	int fsize = format.size();
	for(int i=0;i<fsize;i++){
		// Start of the pattern variable
		if(format[i] == '$'){
			int start = i+2; // ${
			int end = start;
			while(format[++end] != '}');
			std::string variable = format.substr(start, end-start);
			
			// Replace variable
			if(variable == "name"){
				res.append(name);
			} else if(variable == "message"){
				res.append(escape(message));
			} else if(variable == "type"){
				res.append(ltype);
			} else if(variable == "date"){
				res.append(date);				
			} else if(variable == "thread") {
				res.append(thread);
			} else {
				res.append("${"+variable+"}");				
			}
			i=end;
			continue;
		}
		res.push_back(format[i]);
	}
	return res;
}

std::string logger::Log::get_time_str()
{
	auto t = std::chrono::system_clock::now();
	auto seconds = std::chrono::system_clock::to_time_t(t);
	std::stringstream ss;
	ss << std::put_time(gmtime(&seconds), "%Y-%m-%dT%H:%M:%S");
	auto t_seconds = std::chrono::system_clock::from_time_t(seconds);
	auto micro_seconds = std::chrono::duration_cast<std::chrono::microseconds>(t - t_seconds).count();
	ss << "." << micro_seconds << "Z";
	return ss.str();
}

std::string logger::Log::escape(std::string str)
{
	std::string res = "";
	for(int i=0;i<(int)str.size();i++){
		if(str[i] == '"'){
			res.push_back('\\');
		}
		if(str[i] == '\n'){
			res.push_back('\\');
			res.push_back('n');
			continue;
		}
		res.push_back(str[i]);
	}
	return res;
}

void logger::Log::open_stream(std::string path)
{
	std::lock_guard<std::mutex> lock(m);
	if(!_fds.count(path)){
		_fds[path] = std::move(std::ofstream(path, std::ios::app));
		_fds_cnts[path] = 1;
	}
}

void logger::Log::close_stream(std::string path)
{
	std::lock_guard<std::mutex> lock(m);
	if(--_fds_cnts[path] == 0){
		_fds.erase(path);
		_fds_cnts.erase(path);
	}
}

std::ofstream& logger::Log::get_stream(std::string path)
{
	std::lock_guard<std::mutex> lock(m);
	return _fds[path];
}

void logger::Log::write_log(std::string str)
{
	std::ofstream& st = get_stream(path);
	std::lock_guard<std::mutex> lock(m);
	st << str;
}
