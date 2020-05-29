#include "logger.hpp"


int main()
{
	
	logger::Log lgr("forest.log", "forest", 5);

	lgr.info("Cool here");
	lgr.error("Error mtf \"really\"");
	lgr.warning("WRNG");

	for(int i=0;i<100;i++){
		lgr.error("str_error фыв"+std::to_string(i));
	}
	
}
