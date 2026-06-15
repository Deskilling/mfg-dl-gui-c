#include "window.h"

#include <raylib.h>
#include <string.h>

#include "src/request/client.h"

#include "src/fonts/inter.h"

int runWindow() {
	State state = {0};
	state.mode = SEARCH;

	state.curl = initClient();
	if (!state.curl) {
		fprintf(stderr, "Failed to init curl\n");
		return -1;
	}

	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(800, 600, "mfg");

	state.font = LoadFontFromMemory(".ttf", Inter_VariableFont_opsz_wght_ttf, Inter_VariableFont_opsz_wght_ttf_len, 48, NULL, 0);
	SetTextureFilter(state.font.texture, TEXTURE_FILTER_BILINEAR);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground((Color){28, 28, 28, 0});

		drawQueryBar(&state);

		// printf("%i\n", state.searchSelected);
		// printf("State %i\n", state.mode);

		switch (state.mode) {
		case SEARCH: {
			handleCharInput(&state);
			handleBackspace(&state);

			if (state.displayResults) {
				displayResults(&state);
				if (IsKeyPressed(KEY_DOWN) && state.resultsLen != 0) {
					state.mode = SELECT_SEARCH;
				}
			}

			if (IsKeyPressed(KEY_ENTER)) {
				handleSearch(&state);
				if (state.resultsLen != 0) {
					state.searchSelected = 0;
					state.mode = SELECT_SEARCH;
				} else {
					state.displayResults = 1;
				}
			}

			break;
		}

		case SELECT_SEARCH: {
			displayResults(&state);
			handleUpDownSearch(&state);

			break;
		}

		case SCORE: {
			break;
		}

		case SEASONS: {
			break;
		}

		case EPISODES: {
			break;
		}

		default: {
			break;
		}
		}

		EndDrawing();
	}

	cleanupClient(state.curl);
	UnloadFont(state.font);
	CloseWindow();
	return 0;
}
