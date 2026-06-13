#include <cjson/cJSON.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "src/request/requests.h"
#include "src/util/json.h"

SearchResult* parseSearchResults(cJSON* json) {
	SearchResult* head = NULL;
	SearchResult* tail = NULL;

	cJSON* jsonEntry;
	cJSON_ArrayForEach(jsonEntry, json) {
		SearchResult* result = malloc(sizeof(SearchResult));
		if (result == NULL) {
			fprintf(stderr, "malloc failed in parseSearchResults\n");
			exit(-1);
		}

		result->prev = NULL;
		result->next = NULL;

		result->service = getJsonString(jsonEntry, "service");
		result->name = getJsonString(jsonEntry, "name");
		result->href = getJsonString(jsonEntry, "href");
		result->description = getJsonString(jsonEntry, "description");
		result->cover = getJsonString(jsonEntry, "coverPath");
		result->productionYear = getJsonString(jsonEntry, "productionYear");

		// scores are empty at this point
		result->scoreCount = 0;
		result->scores = NULL;

		if (tail == NULL) {
			head = tail = result;
		} else {
			tail->next = result;
			result->prev = tail;
			tail = result;
		}
	}

	return head;
}

Response postSearch(CURL* curl, char* query) {
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	size_t locationLen = strlen(SERVERURL) + strlen("/search?q=") + strlen(query) + strlen("&cacheCover=true") + 1;
	char* location = malloc(locationLen);
	if (location == NULL) {
		fprintf(stderr, "malloc failed in postSearch\n");
		exit(-1);
	}

	snprintf(location, locationLen, "%s/search?q=%s&cacheCover=true", SERVERURL, query);

	Request req = {.location = location, .content = ""};
	Response r = sendPost(curl, &req);

	// printf("%s\n", location);
	// printf("%s\n", r.content);

	free(location);
	curl_slist_free_all(headers);

	if (r.code != CURLE_OK) {
		fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(r.code));
		freeResponse(&r);
		return r;
	}

	r.size = sizeof(char) * strlen(r.content);
	return r;
}

SearchResult* search(CURL* curl, char* query) {
	SearchResult* empty = {0};

	Response r = postSearch(curl, query);
	if (r.content == NULL) {
		fprintf(stderr, "Response is NULL\n");
		return empty;
	}

	cJSON* json = cJSON_Parse(r.content);
	freeResponse(&r);

	if (json == NULL) {
		fprintf(stderr, "Failed to parse JSON\n");
		return empty;
	}

	SearchResult* results = parseSearchResults(json);
	cJSON_Delete(json);
	return results;
}

void freeSearchResult(SearchResult* result) {
	while (result != NULL) {
		SearchResult* next = result->next;

		free(result->service);
		free(result->name);
		free(result->href);
		free(result->description);
		free(result->cover);
		free(result->productionYear);

		if (result->scores != NULL) {
			// TODO if scores are implemented
		}

		free(result);
		result = next;
	}
}
