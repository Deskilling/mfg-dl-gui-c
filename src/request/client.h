#ifndef CLIENT_H
#define CLIENT_H

#include <curl/curl.h>

typedef struct {
	CURL* client;
	CURLcode request;
} client;

CURL* initClient();
void cleanupClient(CURL* curl);

#endif
