#include "src/request/api/api.h"
#include "window.h"
#include <raylib.h>

void displayScore(State* state) {
	SearchResult selected = state->results[state->searchSelected];
	ScoreEntry* current = selected.scores;
	int cnt = 0;

	while (current != NULL) {
		char text[100];
		snprintf(text, sizeof(text), "%s %.02f", current->key, current->value.score);
		DrawTextEx(state->font, text, (Vector2){10, 30 * cnt + 50}, 28.0f, 1.0f, WHITE);

		current = current->next;
		cnt++;
	}
}
