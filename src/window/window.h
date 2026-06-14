#ifndef MFG_WINDOW_H
#define MFG_WINDOW_H

#include <raylib.h>

#include "src/request/api/api.h"

typedef struct {
	CURL* curl;
	Font font;

	// input
	float backspaceTimer;

	// search
	char query[80];
	int queryLen;
	SearchResult* results;
	int drawResults;

} State;

int createWindow();
int runWindow();

void handleCharInput(State* state);
void handleBackspace(State* state);
int pressedEnter();

void drawQueryBar(State* state);
void handleSearch(State* state);
void displayResults(State* state);

#endif
