#include "api.h"

#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/request/requests.h"

char* parseHealth(char* json) {
	cJSON* parsedJson = cJSON_Parse(json);

	cJSON* object = cJSON_GetObjectItem(parsedJson, "status");
	if (object == NULL) {
		fprintf(stderr, "Failed parsing status in response");
		cJSON_Delete(parsedJson);
		exit(-1);
	}

	char* result = malloc(strlen(object->valuestring) + 1);
	if (result == NULL) {
		cJSON_Delete(parsedJson);
		fprintf(stderr, "malloc failed\n");
		exit(-1);
	}
	strcpy(result, object->valuestring);

	cJSON_Delete(parsedJson);
	return result;
}

Response postHealth(CURL* curl) {
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	Request req = {.location = SERVERURL "/health", .content = ""};

	Response r = sendPost(curl, &req);

	if (r.code != CURLE_OK) {
		fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(r.code));
		freeResponse(&r);
		return r;
	}

	r.size = sizeof(char) * strlen(r.content);

	curl_slist_free_all(headers);

	return r;
}

int health(CURL* curl) {
	Response r = postHealth(curl);
	if (r.content == NULL) {
		fprintf(stderr, "Reponse is NULL\n");
		return 0;
	}

	char* status = parseHealth(r.content);
	int cmp = 0;

	printf("status is %s\n", status);
	if (strcmp(status, "ok") == 0) {
		cmp = 1;
	}

	printf("CMP IS %i\n", cmp);

	freeResponse(&r);
	free(status);
	return cmp;
}
