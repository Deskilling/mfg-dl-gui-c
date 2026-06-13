#include <raylib.h>

#include "src/request/api/api.h"
#include "src/request/client.h"

#include "src/fonts/inter.h"

int runWindow() {
	CURL* curl = initClient();
	if (!curl) {
		fprintf(stderr, "Failed to init curl\n");
		return -1;
	}

	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(800, 600, "mfg");

	Font font = LoadFontFromMemory(".ttf", Inter_VariableFont_opsz_wght_ttf, Inter_VariableFont_opsz_wght_ttf_len, 48, NULL, 0);
	SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

	int serverRunning = health(curl);
	double lastCheck = GetTime();
	const double CHECK_INTERVAL = 2.0;

	while (!WindowShouldClose()) {
		if (GetTime() - lastCheck >= CHECK_INTERVAL) {
			serverRunning = health(curl);
			lastCheck = GetTime();
		}

		BeginDrawing();
		ClearBackground((Color){28, 28, 28, 255});
		if (serverRunning) {
			DrawTextEx(font, "Server Ok", (Vector2){50, 50}, 24, 1.0f, WHITE);
		} else {
			DrawTextEx(font, "Request Failed", (Vector2){50, 50}, 24, 1.0f, RED);
		}

		EndDrawing();
	}

	cleanupClient(curl);

	UnloadFont(font);
	CloseWindow();
	return 0;
}
