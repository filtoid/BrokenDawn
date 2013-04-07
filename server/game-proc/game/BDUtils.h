#include <string>
// Note: stdlib appears to require string.h not just string
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

struct MemoryStruct {
  char *memory;
  size_t size;
};

class BDUtils{

public:

	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
	{
  		size_t realsize = size * nmemb;
  		struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  		mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
  		if(mem->memory == NULL) {
    			/* out of memory! */ 
			cout << "not enough memory (realloc returned NULL)\n";
    			return 0;
  		}
 
  		memcpy(&(mem->memory[mem->size]), contents, realsize);
  		mem->size += realsize;
  		mem->memory[mem->size] = 0;
 
  		return realsize;
	}

	static std::string getRevision( std::string in ){
		
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

	static std::string getItemFromJson( std::string in, std::string itemName ){
		
		ostringstream item;
		item << "\"" << itemName << "\"";		
		size_t start = in.find(item.str());

		if(start==-1)
			return "";

		start = in.find(":",start+item.str().length())+1;
		
		if(start==-1)
			return "";
		
		size_t comma = in.find(",",start);

		// Are we looking at a number?		
		if(comma!=-1 && comma<in.find("\"",start))
			return in.substr(start,comma-start);
		
		size_t bracketStart = in.find("[",start);		
		
		if(bracketStart!=-1 && bracketStart<comma)
		{	
			// We have an array
			size_t bracketEnd = in.find("]",start);
			return in.substr(bracketStart,(bracketEnd-bracketStart)+1);		
		}
		
		// else we have a simple string
		start = in.find("\"",start)+1;
		size_t end = in.find("\"",start);
		
		if(end==-1)
			return in.substr(start, in.length()-start);

		return in.substr(start,end-start);		
	}
	
	static std::vector<std::string> getVecFromAry( std::string ary ){
		
		std::vector<std::string> retVec;
				
		// Take the json array and make a vector of strings
		size_t start = ary.find("\"");

		if(start==-1)
			return retVec;
	
		while(start!=-1)
		{
			start += 1;
			size_t end = ary.find("\"",start);
			
			// Debug statment
			//cout << "Start = " << start << ", End = " << end;

			if(end==-1)
				start=end;

			std::string next = ary.substr(start,end-start);		
			retVec.push_back(next);

			start=ary.find("\"",end+1);
			
		}

		return retVec;
	}

	static std::string getItemFromDatabase(std::string db, std::string item);
public:
	//static std::string 
	void BDUTils(){}

};
