#include <stdlib.h>
#include <string.h>

#include "requests.h"

/*
 * https://curl.se/libcurl/c/post-callback.html
 * https://curl.se/libcurl/c/postinmemory.html
 */

static size_t read_cb(char* dest, size_t size, size_t nmemb, void* userp) {
	Writer* wt = (Writer*)userp;
	size_t buffer_size = size * nmemb;

	if (wt->sizeleft) {
		size_t copy_this_much = wt->sizeleft;
		if (copy_this_much > buffer_size) {
			copy_this_much = buffer_size;
		}
		memcpy(dest, wt->readptr, copy_this_much);

		wt->readptr += copy_this_much;
		wt->sizeleft -= copy_this_much;

		return copy_this_much;
	}
	return 0;
}

static size_t write_cb(void* contents, size_t size, size_t nmemb, void* userp) {
	size_t realsize = size * nmemb;
	Response* buffer = (Response*)userp;

	char* ptr = realloc(buffer->content, buffer->size + realsize + 1);
	if (!ptr) {
		fprintf(stderr, "not enought memory (realloc returned NULL)\n");
		return 0;
	}

	buffer->content = ptr;
	memcpy(&(buffer->content[buffer->size]), contents, realsize);
	buffer->size += realsize;
	buffer->content[buffer->size] = '\0';

	return realsize;
}

Response sendPost(CURL* curl, Request* request) {
	Response response = {0};

	if (curl == NULL || request == NULL) {
		response.code = CURLE_FAILED_INIT;
		return response;
	}

	Writer wt = {request->content, strlen(request->content)};

	response.content = malloc(1);
	if (response.content == NULL) {
		fprintf(stderr, "malloc failed in sendPost\n");
		exit(-1);
	}

	response.size = 0;

	curl_easy_setopt(curl, CURLOPT_URL, request->location);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_cb);
	curl_easy_setopt(curl, CURLOPT_READDATA, &wt);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)wt.sizeleft);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	response.code = curl_easy_perform(curl);
	return response;
}

void freeResponse(Response* response) {
	free(response->content);
	response->content = NULL;
}
