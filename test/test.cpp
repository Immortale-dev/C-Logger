#include "logger.hpp"
#include "qtest.hpp"
#include "helpers.cpp"
#include <iostream>
#include <thread>
#include <vector>

using namespace std;


DESCRIBE("Logger", {
	
	DESCRIBE("Can initialize with default parameters", {
		logger::Log log;
		
		DESCRIBE("AND we can put errors, warnings and infos to the log", {
			BEFORE_ALL({
				log.error("12345");
				log.warning("54321");
				log.info("44444");
			});
			
			IT("main.log file should exists in root folder", {
				EXPECT(file_exists("./main.log")).to_be(true);
			});
			
			IT("main.log should contain 3 log lines with specific messages", {
				log.flush();
				vector<string> content = read_file("./main.log");
				EXPECT(content.size()).to_be(3);
				EXPECT(content[0].find("12345")).to_be_greater_than_or_equal(0);
				EXPECT(content[0].find("error")).to_be_greater_than_or_equal(0);
				EXPECT(content[1].find("54321")).to_be_greater_than_or_equal(0);
				EXPECT(content[1].find("warning")).to_be_greater_than_or_equal(0);
				EXPECT(content[2].find("44444")).to_be_greater_than_or_equal(0);
				EXPECT(content[2].find("info")).to_be_greater_than_or_equal(0);
			});
		});
		
		DESCRIBE("AND we can write to the same log file from different instances of different threads", {
			BEFORE_ALL({
				vector<thread> threads;
				for(int i=0;i<1000;i++){
					thread t([&log](){
						log.info("COOL");
					});
					threads.push_back(move(t));
				}
				
				for(auto& it : threads){
					it.join();
				}
			});
			
			IT("log file should contain not less than 1000 records and they should not be corrupted", {
				log.flush();
				vector<string> content = read_file("./main.log");
				for(auto &it : content){
					EXPECT(it[0]).to_be('{');
					EXPECT(it[it.size()-1]).to_be('}');
				}
			});
		});
	});
	
	DESCRIBE("Can initialize with spefifiing parameters", {
		logger::Log log_ie("./tmp/ie.log", "infos_and_errors", logger::L_ERROR | logger::L_INFO);
		
		DESCRIBE("AND we can put log records to the file", {
			BEFORE_ALL({
				log_ie.error("12345");
				log_ie.warning("54321");
				log_ie.info("44444");
			});
			
			IT("Log file should exists", {
				EXPECT(file_exists("./tmp/ie.log")).to_be(true);
			});
			
			IT("Log file should contains only error and info records", {
				log_ie.flush();
				vector<string> content = read_file("./tmp/ie.log");
				EXPECT(content.size()).to_be(2);
				EXPECT(content[0].find("infos_and_errors")).to_be_greater_than_or_equal(0);
				EXPECT(content[1].find("infos_and_errors")).to_be_greater_than_or_equal(0);
				EXPECT(content[0].find("error")).to_be_greater_than_or_equal(0);
				EXPECT(content[1].find("info")).to_be_greater_than_or_equal(0);
			});
		});
	});
	
	DESCRIBE("After everything is destructed", {
		IT("All streams should be closed", {
			EXPECT(logger::_fds.size()).to_be(0);
			EXPECT(logger::_fds_cnts.size()).to_be(0);
		});
	});
	
	AFTER_ALL({
		delete_file("./main.log");
		delete_file("./tmp/ie.log");
	});
});


int main(){return 0;}
