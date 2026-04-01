/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <vector>

struct Body {
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	float mass;
	float size;
	float restitution;
	//int force;
};

float GetRandomFloat() {
	return GetRandomValue(0,10000) / (float)10000;
}

Vector2 gravity = { 0, 9.81f };

int main ()
{
	std::vector<Body> bodies;
	bodies.reserve(1000);

	SetRandomSeed(5);

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(800, 600, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float dt = GetFrameTime();

		if (IsMouseButtonDown(0) || IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			Body body;
			body.position = GetMousePosition();
			// get random unit circle vector for direction
			float angle = GetRandomFloat() * (2*PI);
			Vector2 direction;
			direction.x = cosf(angle);
			direction.y = sinf(angle);

			body.velocity = direction * (50.0f + (GetRandomFloat() * 500)); // speed between 50 and 550
			body.size = 5.0f + (GetRandomFloat() * 20.0f); // size between 5 and 25
			body.restitution = 0.5f + (GetRandomFloat() * 0.5f);

			bodies.push_back(body);
		}

		// UPDATE
		for (Body& body : bodies)
		{
			//integration
			body.velocity += (gravity * 100.0f) * dt;
			body.position += body.velocity * dt;

			//collision
			if (body.position.x + body.size > GetScreenWidth())
			{
				body.position.x = GetScreenWidth() - body.size;
				body.velocity.x *= -1.0f;
				body.velocity *= -body.restitution;
			}
			if (body.position.y + body.size > GetScreenHeight())
			{
				body.position.y = GetScreenHeight() - body.size;
				body.velocity.y *= -1.0f;
				body.velocity *= -body.restitution;
			}
			if (body.position.x - body.size < 0)
			{
				body.position.x = body.size;
				body.velocity.x *= -1.0f;
				body.restitution *= -body.restitution;
			}
			if (body.position.y - body.size < 0)
			{
				body.position.y = body.size;
				body.velocity.y *= -1.0f;
				body.velocity *= -body.restitution;
			}
		}

		// DRAW
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		DrawText("Hello Raylib", 200,200,20,WHITE);

		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);

		for (Body& body : bodies)
		{
			//DrawCircleV(body.position, body.size, RED);
			DrawTexture(wabbit, body.position.x, body.position.y, WHITE);
		}

		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
