#ifndef MFG_CLIENT_H
#define MFG_CLIENT_H

#include <curl/curl.h>

typedef struct {
	CURL* client;
	CURLcode request;
} client;

CURL* initClient();
void cleanupClient(CURL* curl);

#endif
