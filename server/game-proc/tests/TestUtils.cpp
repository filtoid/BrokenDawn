#include "./TestUtils.h"

#include "BDUtils.h"

#include <iostream>
#include <string>
#include <sstream>

#include <curl/curl.h>

using namespace std;

/*Static Setup/Tear down methods*/
// Vars to store the revision numbers of the table we have updated	
static std::string gamesRev="";
static std::string usersRev="";

/*static*/ void TestUtils::CreateDatabase()
{
	CURL *curl;
 
  	/* get a curl handle */ 
  	curl = curl_easy_init();
  	
	if(curl) {	
		/* HTTP PUT */ 
		curl_easy_setopt(curl, CURLOPT_PUT, 1L);
		 
		/* specify URL for creating the test_db */ 
		curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5984/test_db");
		 
		CURLcode res = curl_easy_perform(curl);    
		if(res != CURLE_OK)
			cout << "curl failure: " << curl_easy_strerror(res)<<endl;
			//fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
		 
		curl_easy_cleanup(curl);
	}else
		cout<<"Failed to init curl" << endl;

}

/*static*/ void TestUtils::CreateUsers()
{
	// Create the users doc in the test db
	CURL *curl;
 	curl = curl_easy_init();
  	
	if(curl) {	
		/* HTTP PUT */ 
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"users\": [\"1stUser\",\"2ndUser\"]}");
 
		/* specify URL for creating the test_db */ 
		curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5984/test_db/bdusers");
		 
		CURLcode res = curl_easy_perform(curl);    
		if(res != CURLE_OK)
			cout << "curl failure: " << curl_easy_strerror(res)<<endl;
		 
		curl_easy_cleanup(curl);
	}else
		cout<<"Failed to init curl" << endl;
}

/*static*/ void TestUtils::CreateGames(int num)
{
	// Create the games doc in the test db
	CURL *curl;
 	curl = curl_easy_init();
  	
	if(curl) {	
		/* HTTP PUT */ 
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		ostringstream jsonStream;
		jsonStream << "{";
		
		if(gamesRev.compare(""))
			jsonStream << "\"_rev\":\""<< gamesRev << "\",";
 
		jsonStream << "\"games\": [";
		
		for(int i=0;i<num;i++){
			jsonStream << "\"Game" << i << "\"";
			if(i!=num-1)
				jsonStream<<",";						
		}

		jsonStream << "]}";
		string json = jsonStream.str();
		//cout << "json => " << json << endl;
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
 
		/* specify URL for creating the test_db */ 
		curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5984/test_db/bdgames");
		
		/*and the returned values*/
		struct bdstring s;
		s.len = 0;
  		s.ptr = (char *)malloc(s.len+1);
  		if (s.ptr == NULL) {
    			fprintf(stderr, "malloc() failed\n");
    			exit(EXIT_FAILURE);
  		}
  		s.ptr[0] = '\0';		
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, BDUtils::curl_write );
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

		CURLcode res = curl_easy_perform(curl);    
		if(res != CURLE_OK){
			cout << "curl failure: " << curl_easy_strerror(res)<<endl;
			return;		
		}

		std::string str(s.ptr);

		gamesRev = BDUtils::getRevision(str);
		//cout << "Games rev: " << gamesRev << endl;

		curl_easy_cleanup(curl);
	}else
		cout<<"Failed to init curl" << endl;
}

/*static*/ void TestUtils::DeleteDatabase()
{
	CURL *curl;
 
  	/* get a curl handle */ 
  	curl = curl_easy_init();
  	
	if(curl) {	
		/* HTTP DELETE is a custom request */ 
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE"); 
		/* use local couchdb*/
		curl_easy_setopt(curl, CURLOPT_URL,  "http://127.0.0.1:5984/test_db");
		 
		CURLcode res = curl_easy_perform(curl);
		    
		/* Check for errors */ 
		if(res != CURLE_OK)
			cout << "curl error:" << curl_easy_strerror(res) << endl;
		 
		curl_easy_cleanup(curl);
	}
	
	// These revision numbers are no longer valid - because
	// we've just deleted the database.
	gamesRev = "";
	usersRev = "";
}


