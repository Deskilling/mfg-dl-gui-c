#include <stdio.h>

#include "client.h"

CURL* initClient() {
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr, "Failed to init curl\n");
		return NULL;
	}
	return curl;
}

void cleanupClient(CURL* curl) {
	curl_easy_cleanup(curl);
	curl_global_cleanup();
}
