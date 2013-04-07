#include "./BDUtils.h"

#include <string>
#include <curl/curl.h>

/*static*/ std::string BDUtils::getItemFromDatabase(std::string db, std::string item)
{
	string retString;

	CURL *curl;
 	curl = curl_easy_init();
  	
	if(curl) {	
		std::ostringstream stringStream;
		stringStream <<  db << "/" << item;
  		std::string url = stringStream.str();
		
		/* HTTP GET */ 
		curl_easy_setopt(curl, CURLOPT_URL, (char*)url.c_str());
	 	
  		struct MemoryStruct chunk;
 
  		chunk.memory = (char *)malloc(1);  /* will be grown as needed by the realloc above */ 
  		chunk.size = 0;    /* no data at this point */ 	
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, BDUtils::WriteMemoryCallback );
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
		CURLcode res = curl_easy_perform(curl);  
		
		if(res != CURLE_OK)
			cout << "curl failure: " << curl_easy_strerror(res)<<endl;
		
		// Now copy the output to the return value
		std::string str(chunk.memory);
		retString = str;
  		curl_easy_cleanup(curl);
		
	}else
		cout<<"Failed to init curl" << endl;

	return retString;	
}
