#ifndef REQUESTS_H
#define REQUESTS_H

#define SERVERURL "http://localhost:6702"

#include <stdlib.h>
#include <curl/curl.h>

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
