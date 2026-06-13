#include "json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getJsonString(cJSON* json, const char* string) {
	cJSON* field = cJSON_GetObjectItem(json, string);
	char* jsonString = (field != NULL && field->valuestring) ? field->valuestring : "";
	char* result = malloc(strlen(jsonString) + 1);
	if (result == NULL) {
		fprintf(stderr, "malloc failed in getJsonString\n");
		exit(-1);
	}
	strcpy(result, jsonString);
	return result;
}
