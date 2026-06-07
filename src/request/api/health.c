#include "api.h"

Response postHealth(CURL* curl) {
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	Request req = {.location = SERVERURL "/health", .content = ""};

	Response r = sendPost(curl, &req);

	if (r.code != CURLE_OK) {
		fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(r.code));
	} else {
		printf("Got: %s\n", r.content);
	}

	curl_slist_free_all(headers);

	return r;
}
