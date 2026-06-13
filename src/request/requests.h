#ifndef MFG_REQUESTS_H
#define MFG_REQUESTS_H

#define SERVERURL "http://localhost:6702"

#include <curl/curl.h>
#include <cjson/cJSON.h>

typedef struct {
	const char* readptr;
	size_t sizeleft;
} Writer;

typedef struct {
	char* location;
	char* content;
	size_t size;
} Request;

typedef struct {
	char* content;
	size_t size;
	CURLcode code;
} Response;

Response sendPost(CURL* curl, Request* request);
void freeResponse(Response* response);

#endif
