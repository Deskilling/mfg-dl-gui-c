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
		if (state->queryLen > 0)
			state->query[--(state->queryLen)] = '\0';
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

int pressedEnter() {
	if (IsKeyPressed(KEY_ENTER)) {
		return 1;
	}

	return 0;
}
