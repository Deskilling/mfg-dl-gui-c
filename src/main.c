#define NOWINDOW 1

#include <stdio.h>

#include "window/window.h"
#include "request/client.h"
#include "request/api/api.h"

void test();

int main() {
	if (NOWINDOW) {
		test();
	} else {
		runWindow();
	}
	return 0;
}

void test() {
	CURL* curl = initClient();
	if (!curl) {
		fprintf(stderr, "Failed to init curl\n");
		return;
	}

	int serverAlive = health(curl);

	if (!serverAlive) {
		fprintf(stderr, "Server is not running!\n");
		return;
	}

	SearchResult* results = search(curl, "conan");

	while (results->next != NULL) {
		printf("%s\n", results->name);
		printf(" %p\n", results->next);
		printf(" %p\n", results->prev);
		printf(" %s\n", results->cover);
		printf(" %s\n", results->href);
		printf(" %s\n", results->productionYear);

		printf("\n");

		results = results->next;
	}
}
