#include "window.h"

#include <raylib.h>
#include <string.h>

#include "src/request/client.h"

#include "src/fonts/inter.h"

int runWindow() {
	State state = {0};

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

		handleCharInput(&state);
		handleBackspace(&state);
		drawQueryBar(&state);

		if (pressedEnter()) {
			handleSearch(&state);
		}

		if (state.drawResults) {
			displayResults(&state);
		}

		EndDrawing();
	}

	cleanupClient(state.curl);
	UnloadFont(state.font);
	CloseWindow();
	return 0;
}
