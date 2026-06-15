#include "window.h"

#include "src/request/api/api.h"

#include <raylib.h>
#include <stdio.h>
#include <string.h>

void drawQueryBar(State* state) {
	const float fontSize = 28.0f;
	const float spacing = 1.0f;
	const Vector2 origin = {10, 10};

	char display[100];
	Color color = WHITE;

	if (strcmp(state->query, "") == 0) {
		snprintf(display, sizeof(display), "> Search TMDB");
		color = (Color){255, 255, 255, 50};
	} else {
		snprintf(display, sizeof(display), "> %s", state->query);

		if (state->mode == SEARCH) {
			if ((int)(GetTime() * 2) % 2 == 0) {
				Vector2 textSize = MeasureTextEx(state->font, display, fontSize, spacing);
				Vector2 cursorPos = {origin.x + textSize.x, origin.y};
				DrawTextEx(state->font, "|", cursorPos, fontSize, spacing, WHITE);
			}
		}
	}

	DrawTextEx(state->font, display, origin, fontSize, spacing, color);
}

void handleSearch(State* state) {
	if (!health(state->curl)) {
		fprintf(stderr, "Server is not running!\n");
		return;
	}

	// TODO A bit of a goofy solution rn, if i would free the results here it segfauls
	if (strlen(state->query) < 1) {
		return;
	}

	if (state->results != NULL) {
		state->resultsLen = 0;
		freeSearchResult(state->results);
	}

	state->results = search(state->curl, state->query);

	SearchResult* current = state->results;
	while (current != NULL) {
		state->resultsLen++;
		printf("incremented resultslen %i\n", state->resultsLen);
		current = current->next;
	}
}

void displayResults(State* state) {
	const float fontSize = 28.0f;
	const float spacing = 1.0f;
	const float lineHeight = 40.0f;
	int cnt = 0;

	if (state->query[0] != '\0' && state->resultsLen == 0) {
		DrawTextEx(state->font, "Nothing there :(", (Vector2){10, 50}, fontSize, spacing, GRAY);
		return;
	}

	SearchResult* current = state->results;
	while (current != NULL) {
		Vector2 origin = {10, 50 + cnt * lineHeight};

		if (cnt == state->searchSelected) {
			DrawTextEx(state->font, current->name, origin, fontSize, spacing, GRAY);
		} else {
			DrawTextEx(state->font, current->name, origin, fontSize, spacing, WHITE);
		}

		cnt++;
		current = current->next;
	}
}
