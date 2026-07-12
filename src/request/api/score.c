
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "src/request/requests.h"
#include "src/util/json.h"

SearchResult* parseScoreResult(cJSON* json) {
	SearchResult* head = NULL;
	SearchResult* tail = NULL;

	SearchResult* result = malloc(sizeof(SearchResult));
	if (result == NULL) {
		fprintf(stderr, "malloc failed in parseSearchResults\n");
		exit(-1);
	}

	result->prev = NULL;
	result->next = NULL;

	result->service = getJsonString(json, "service");
	result->name = getJsonString(json, "name");
	result->href = getJsonString(json, "href");
	result->description = getJsonString(json, "description");
	result->cover = getJsonString(json, "coverPath");
	result->productionYear = getJsonString(json, "productionYear");

	result->scoreCount = 0;
	result->scores = NULL;
	ScoreEntry* scoreTail = NULL;

	cJSON* scoresJson = cJSON_GetObjectItemCaseSensitive(json, "scores");
	if (scoresJson != NULL && cJSON_IsObject(scoresJson)) {
		cJSON* scoreEntry = NULL;
		cJSON_ArrayForEach(scoreEntry, scoresJson) {
			ScoreEntry* newEntry = malloc(sizeof(ScoreEntry));
			if (newEntry == NULL) {
				fprintf(stderr, "malloc failed for ScoreEntry\n");
				exit(-1);
			}

			newEntry->key = malloc(strlen(scoreEntry->string) + 1);
			if (newEntry->key == NULL) {
				fprintf(stderr, "malloc failed for ScoreEntry key\n");
				exit(-1);
			}
			strcpy(newEntry->key, scoreEntry->string);

			cJSON* scoreValue = cJSON_GetObjectItemCaseSensitive(scoresJson, scoreEntry->string);
			newEntry->value.score = cJSON_GetObjectItemCaseSensitive(scoreValue, "score")->valuedouble;
			printf("got score %f\n", newEntry->value.score);

			cJSON* queryJson = cJSON_GetObjectItemCaseSensitive(scoreValue, "query");
			if (queryJson != NULL) {
				newEntry->value.query = malloc(strlen(queryJson->valuestring) + 1);
				if (newEntry->value.query == NULL) {
					fprintf(stderr, "malloc failed for ScoreEntry query\n");
					exit(-1);
				}
				strcpy(newEntry->value.query, queryJson->valuestring);
			} else {
				newEntry->value.query = NULL;
			}

			cJSON* hrefJson = cJSON_GetObjectItemCaseSensitive(scoreValue, "href");
			if (hrefJson != NULL) {
				newEntry->value.href = malloc(strlen(hrefJson->valuestring) + 1);
				if (newEntry->value.href == NULL) {
					fprintf(stderr, "malloc failed for ScoreEntry href\n");
					exit(-1);
				}
				strcpy(newEntry->value.href, hrefJson->valuestring);
			} else {
				newEntry->value.href = NULL;
			}

			newEntry->prev = scoreTail;
			newEntry->next = NULL;

			if (scoreTail == NULL) {
				result->scores = newEntry;
			} else {
				scoreTail->next = newEntry;
			}
			scoreTail = newEntry;
			result->scoreCount++;
		}
	}

	if (tail == NULL) {
		head = tail = result;
	} else {
		tail->next = result;
		result->prev = tail;
		tail = result;
	}

	return head;
}
char* serializeSearchResult(SearchResult* result) {
	cJSON* json = cJSON_CreateObject();

	cJSON_AddStringToObject(json, "service", result->service);
	cJSON_AddStringToObject(json, "name", result->name);
	cJSON_AddStringToObject(json, "href", result->href);
	cJSON_AddStringToObject(json, "description", result->description);
	cJSON_AddStringToObject(json, "cover", result->cover);
	cJSON_AddStringToObject(json, "productionYear", result->productionYear);

	char* jsonStr = cJSON_Print(json);
	cJSON_Delete(json);

	return jsonStr;
}

Response postScore(CURL* curl, SearchResult* result) {
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	size_t locationLen = strlen(SERVERURL) + strlen("/score") + 1;
	char* location = malloc(locationLen);
	if (location == NULL) {
		fprintf(stderr, "malloc failed in postSearch\n");
		exit(-1);
	}

	sprintf(location, "%s/score", SERVERURL);

	Request req = {.location = location, .content = serializeSearchResult(result)};
	Response r = sendPost(curl, &req);

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

SearchResult* score(CURL* curl, SearchResult* result) {
	printf("calling scores\n”");
	SearchResult* empty = {0};

	Response r = postScore(curl, result);
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

	SearchResult* results = parseScoreResult(json);
	cJSON_Delete(json);
	return results;
}
