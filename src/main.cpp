#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"

#include "Random.h"
#include "Body.h"
#include "World.h"

World world;

int main ()
{
	SetRandomSeed(5);

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(800, 600, "Hello Raylib"); // Create the window - width, height, title

	SearchAndSetResourceDir("resources");

	// Load texture from resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	// GAME_LOOP
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float dt = GetFrameTime();

		// INPUT
		if (IsMouseButtonDown(0) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
		{
			Body body;
			body.position = GetMousePosition();

			float angle = GetRandomFloat() * (2 * PI);
			Vector2 direction = { cosf(angle), sinf(angle) };

			body.velocity = direction * (50.0f + GetRandomFloat() * 500.0f);
			body.acceleration = { 0, 0 };
			body.size = 5.0f + GetRandomFloat() * 20.0f;
			body.restitution = 0.5f + GetRandomFloat() * 0.5f;
			body.mass = 1.0f;

			world.AddBody(body);

		}

		// UPDATE
		world.Step(dt);

		// DRAW
		BeginDrawing();
		ClearBackground(BLACK); // clear the background to black

		DrawText("Hello Raylib", 200, 200, 20, WHITE); // draw some text at x=200, y=200 with font size 20 and white color
		DrawTexture(wabbit, 400, 200, WHITE); // draw our texture at x=400, y=200 with white tint (no tint)

		world.Draw(wabbit);
		
		// END_DRAW
		EndDrawing();
	}

	// EXIT
	UnloadTexture(wabbit); // Unload the texture from GPU memory
	CloseWindow();
	return 0;
}
