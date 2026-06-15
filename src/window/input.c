#include "window.h"

#include <raylib.h>

void handleCharInput(State* state) {
	int ch = GetCharPressed();
	while (ch > 0) {
		if (ch >= 32 && ch < 127 && state->queryLen < 79) {
			state->query[state->queryLen++] = (char)ch;
			state->query[state->queryLen] = '\0';
		}
		ch = GetCharPressed();
	}
}

void handleBackspace(State* state) {
	const float BACKSPACE_INITIAL_DELAY = 0.4f;
	const float BACKSPACE_REPEAT_RATE = 0.05f;

	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_BACKSPACE)) {
		while (state->queryLen > 0 && state->query[state->queryLen - 1] == ' ') state->query[--(state->queryLen)] = '\0';
		while (state->queryLen > 0 && state->query[state->queryLen - 1] != ' ') state->query[--(state->queryLen)] = '\0';
		state->query[state->queryLen] = ' ';
		state->query[++(state->queryLen)] = '\0';
	} else if (IsKeyPressed(KEY_BACKSPACE)) {
		if (state->queryLen > 0) {
			state->query[--(state->queryLen)] = '\0';
		}
		state->backspaceTimer = -BACKSPACE_INITIAL_DELAY;
	} else if (IsKeyDown(KEY_BACKSPACE)) {
		state->backspaceTimer += GetFrameTime();
		while (state->backspaceTimer >= BACKSPACE_REPEAT_RATE && state->queryLen > 0) {
			state->query[--(state->queryLen)] = '\0';
			state->backspaceTimer -= BACKSPACE_REPEAT_RATE;
		}
	} else {
		state->backspaceTimer = 0.0f;
	}
}

void handleUpDownSearch(State* state) {
	const float ARROWS_INITIAL_DELAY = 0.4f;
	const float ARROWS_REPEAT_RATE = 0.05f;

	if (state->resultsLen <= 0) {
		return;
	}

	if (IsKeyPressed(KEY_UP)) {
		if (state->searchSelected > 0) {
			state->searchSelected--;
		} else if (state->searchSelected == 0) {
			state->mode = SEARCH;
			state->displayResults = 1;
			state->searchSelected = -1;
			return;
		}
		state->arrowTimer = -ARROWS_INITIAL_DELAY;
	} else if (IsKeyPressed(KEY_DOWN)) {
		if (state->searchSelected < state->resultsLen - 1) {
			state->searchSelected++;
		}
		state->arrowTimer = -ARROWS_INITIAL_DELAY;
	} else if (IsKeyDown(KEY_UP)) {
		state->arrowTimer += GetFrameTime();
		while (state->arrowTimer >= ARROWS_REPEAT_RATE && state->searchSelected > 0) {
			state->searchSelected--;
			state->arrowTimer -= ARROWS_REPEAT_RATE;
		}
	} else if (IsKeyDown(KEY_DOWN)) {
		state->arrowTimer += GetFrameTime();
		while (state->arrowTimer >= ARROWS_REPEAT_RATE && state->searchSelected < state->resultsLen - 1) {
			state->searchSelected++;
			state->arrowTimer -= ARROWS_REPEAT_RATE;
		}
	} else {
		state->arrowTimer = 0.0f;
	}
}
