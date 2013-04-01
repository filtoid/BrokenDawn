#include <string>
// Note: stdlib appears to require string.h not just string
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>

using namespace std;

struct bdstring{
	char *ptr;
	size_t len;	
};

class BDUtils{

public:
	static size_t curl_write( void *ptr, size_t size, size_t nmemb, void *stream){
		struct bdstring *result = (struct bdstring *)stream;

		/* Copy curl's stream buffer into our own buffer */
		memcpy(result->ptr + result->len, ptr, size * nmemb);

		/* Advance the position */
		result->len += size * nmemb;

		return size * nmemb;
	}

	static std::string getRevision( std::string in ){
		cout << in;
		size_t start = in.find("rev");
		if(start==-1)
			return "";

		start = in.find(":",start);
		start = in.find("\"",start)+1;
		size_t end = in.find("\"",start);
		if(end==-1)
			return "";		
		//ostringstream dbg;
		//dbg << "st: " << start << " en: " << end << std::endl;
		//cout << "debug string: " << dbg.str();

		return in.substr(start,end-start);	
	}	
public:
	//static std::string 
	void BDUTils(){}

};
