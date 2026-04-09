#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"

#include "Random.h"
#include "Body.h"
#include "World.h"
#include "point_effector.h"
#include "GravitationalEffector.h"

int main ()
{
	SetRandomSeed(5);

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(800, 600, "Hello Raylib"); // Create the window - width, height, title

	SearchAndSetResourceDir("resources");

	// Load texture from resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	SetTargetFPS(10);

	World world;
	//world.AddEffector(new PointEffector(Vector2{ 200, 200 }, 100.0f, 30000.0f));
	//world.AddEffector(new PointEffector(Vector2{ 600, 600 }, 100.0f, -30000.0f));
	world.AddEffector(new GravitationalEffector(10000.0f));


	float timeAccum = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f;

	// GAME_LOOP
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float dt = GetFrameTime();

		// INPUT
		if (IsMouseButtonDown(0) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
		{
			Body body;

			body.bodyType = (IsKeyDown(KEY_LEFT_ALT)) ? BodyType::Static : BodyType::Dynamic;

			body.position = GetMousePosition();

			float angle = GetRandomFloat() * (2 * PI);
			Vector2 direction = { cosf(angle), sinf(angle) };

			body.AddForce(direction * (50.0f + GetRandomFloat() * 500.0f), ForceMode::VelocityChange);

			body.size = 5.0f + GetRandomFloat() * 20.0f;
			body.restitution = 0.5f + GetRandomFloat() * 0.5f;
			body.mass = body.size;
			body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : (1.0f / body.mass);
			body.gravityScale = 0.0f;
			body.damping = 0.5f;

			world.AddBody(body);

		}

		// UPDATE
		timeAccum += dt;
		while (timeAccum > fixedTimeStep)
		{
			world.Step(fixedTimeStep);
			timeAccum -= fixedTimeStep;
		}


		// DRAW
		BeginDrawing();
		ClearBackground(BLACK); // clear the background to black

		DrawText("FPS: " + GetFPS(), 10, 10, 20, WHITE); // draw the current FPS at x=10, y=10 with font size 20 and white color
		//DrawText("Hello Raylib", 200, 200, 20, WHITE); // draw some text at x=200, y=200 with font size 20 and white color
		//DrawTexture(wabbit, 400, 200, WHITE); // draw our texture at x=400, y=200 with white tint (no tint)

		world.Draw(wabbit);
		
		// END_DRAW
		EndDrawing();
	}

	// EXIT
	UnloadTexture(wabbit); // Unload the texture from GPU memory
	CloseWindow();
	return 0;
}
