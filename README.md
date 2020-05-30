
# C++ Logger
Simple and lightweight logger for c++ projects that uses namespace `logger`

## How to use
Initialize `Log` instance and call for the log methods.
***Example:*** 
```C++
logger::Log log;
log.error("Something is going wrong...");
```
#### Constructor parameters:
- **std::string path** - the path to the file the data will be append to
- **std::string name** - name you specify for your purposes (to recognize different modules that could write to the same file location for example)
- **int allowance** - *(optional)* number that specify which types of logs records will be wrote to the log file. `0` bit of this number  responsible for the `error` logs, `1`'st bit responsible for the `warning` records and `2`'nd but responsible for the `info` records.
	***Example:*** 
	```C++
	logger::Log log(5); // (101) Will pass only Error and Info records
	```
	There is also local variables that corresponds to each log record type available. The next expression works the same as previous one
	***Example:***
	```C++
	logger::Log log(logger::L_ERROR | logger::L_INFO); // L_WARNING for warnings
	```

#### There is 3 possible constructors available:
- **Log([int allowance])** - default cinstructor
The `main.log` file will be created at the root folder. `name` is not specified. The name is not specified. Default value for `allowance` is `7`
- **Log(std::string path[, int allowance])** - `name` is not specified, default value for `allowance` is `7`
- **Log(std::string path, std::string name[, int allowance])** - Default value for `allowance` is `7`

## Log public methods:
- `void error(std::string message)` - Writes the error record to the log file
- `void warning(std::string message)` - Writes the warning record to the log file
- `void info(std::string message)` - Writes the info record to the log file
- `void log(int type, std::string message)` - Write the corresponded record type to the log file.
- `void flush()` - flush the buffer for the owning log file
	***Example:***
	```C++
	logger::Log log;
	log.error("Error message");
	log.info("Some info message");
	log.log(logger::L_WARNING, "Warning message");
	```

## Public properties:
- `int logger::L_ERROR` - corresponds to the `error` log record type
- `int logger::L_WARNING` - corresponds to the `warning` log record type
- `int logger::L_INFO` - corresponds to the `info` log record type

## Tips and tricks:
- **record format:** Default record format is `"{"name":"${name}","thread":"${thread}","date":"${date}","type":"${type}","message":"${message}"}\n";`
	- `${name}` will be replaced with the `name` parameter you provided to the `logger::Log` constructor
	- `${date}` will be replaced with the date in ISO format in UTC timezome with microseconds (`2020-05-30T09:53:15.535422Z`)
	- `${thread}` will be replaced with the `std::this_thread::get_id()`
	- `${type}` will be replaced with the corresponding log record type
	- `${message}` will be replaced with the message to provided for the record
	
	If you want to change the format you could inherit the `Log` class and change the corresponding `format` variable.
	There is a bunch of other `virtual private` methods you may want to modify to adjust the logger for your preferences.

- **One log file:** If you would like to mix logs from different modules, you could define `Log` instance with the same `path` parameter, so they both will use shared file stream to write log records.
	***Example:***
	```C++
	// module1.cpp
	logger::Log log("everything.log", "module1");
	log.info("some module one information");

	// module2.cpp
	logger::Log log("everything.log", "module2");
	log.info("some another module information that will be pushed to the same file as module 1");
	```

## License
MIT

Have fun :)
