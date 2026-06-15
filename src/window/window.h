#ifndef MFG_WINDOW_H
#define MFG_WINDOW_H

#include <raylib.h>

#include "src/request/api/api.h"

typedef enum {
    SEARCH,
    SELECT_SEARCH,
    SCORE,
    SEASONS,
    EPISODES,
} mode;

typedef struct {
	CURL* curl;
	Font font;
	mode mode;

	// input
	float backspaceTimer;
	float arrowTimer;

	// search
	char query[80];
	int queryLen;
	SearchResult* results;
	int resultsLen;
	int displayResults;

	int searchSelected;

} State;

int createWindow();
int runWindow();

void handleCharInput(State* state);
void handleBackspace(State* state);
void handleUpDownSearch(State* state);
int pressedEnter();

void drawQueryBar(State* state);
void handleSearch(State* state);
void displayResults(State* state);

#endif
