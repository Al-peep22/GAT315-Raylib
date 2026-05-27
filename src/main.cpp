#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"

#include "Random.h"
#include "Body.h"
#include "World.h"
#include "point_effector.h"
#include "gravitational_effector.h"
#include "area_effector.h"
#include "drag_effector.h"
#include "world_camera.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define GUI_PHYSICS_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4576)
#include "gui_physics.h"
#pragma warning(pop)

#define GUI_TEXT(data) TextFormat("%0.2f", data)

#include <string>
#include "../Spring.h"

GuiPhysicsState state;


void AddEffector(World& world, WorldCamera& camera)
{
	Vector2 position = camera.ScreenToWorld(GetMousePosition());
	Effector* effector = nullptr;

	float size = state.EffectorSizeValue;
	float strength = state.EffectorForceValue;
	float angle = state.EffectorAngleValue;

	switch ((EffectorType)state.EffectorTypeActive)
	{
	case EffectorType::Gravitation:
		effector = new GravitationalEffector(position, size, strength);
		break;
	case EffectorType::Point:
		effector = new PointEffector(position, size, strength);
		break;
	case EffectorType::Area:
		effector = new AreaEffector(position, size, angle, strength);
		break;
	case EffectorType::Drag:
		effector = new DragEffector(position, size, strength);
		break;
	}

	if (effector) world.AddEffector(effector);
}

void AddBody(World& world, WorldCamera& camera)
{
	Body body;

	body.bodyType = (BodyType)state.BodyTypeActive;
	body.position = camera.ScreenToWorld(GetMousePosition());

	float angle = GetRandomFloat() * (2 * PI);
	Vector2 direction = { cosf(angle), sinf(angle) };

	body.color = ColorFromHSV(GetRandomFloat() * 360.0f, 1.0f, 1.0f);

	body.AddForce(direction * (10.0f + state.BodyVelocityValue * 20.0f), ForceMode::VelocityChange);

	body.size = state.BodySizeValue;
	body.restitution = state.BodyRestitutionValue;
	body.mass = state.BodyMassValue;
	body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : (1.0f / body.mass);
	body.gravityScale = -state.BodyGravityValue;
	body.damping = state.BodyDampingValue;

	world.AddBody(body);
}



int main ()
{
	SetRandomSeed(5);

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(800, 600, "Hello Raylib"); // Create the window - width, height, title

	state = InitGuiPhysics();

	SearchAndSetResourceDir("resources");

	// Load texture from resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	SetTargetFPS(60);

	World world;
	WorldCamera world_camera(Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f}, 5.0f);

	world.SetBounds(world_camera.ScreenToWorld({ 0.0f, (float)GetScreenHeight()}), world_camera.ScreenToWorld({(float)GetScreenWidth(), 0.0f}));

	Body* selectedBody = nullptr;
	Body* connectedBody = nullptr;

	////Blue
	//world.AddEffector(new GravitationalEffector(Vector2{ 650, 450 }, 200.0f, 2500.0f));

	////Purple
	//world.AddEffector(new DragEffector(Vector2{ 150, 450 }, 200.0f, 300.0f));

	////Red
	//world.AddEffector(new PointEffector(Vector2{ 150, 150 }, 200.0f, -3000.0f));

	////Green
	//world.AddEffector(new AreaEffector(Vector2{ 650, 150 }, 200.0f, 90.0f, 400.0f));


	float timeAccum = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f;
	bool simulate = true;

	// GAME_LOOP
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float dt = fminf(GetFrameTime(), 0.1f);

		if (IsKeyPressed(KEY_SPACE)) state.SimulateActive = !state.SimulateActive;

		World::SetGravity({ 0, state.GravityValue });

		// INPUT
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
			(IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
		{
			if (IsKeyDown(KEY_LEFT_SHIFT))
			{
				AddEffector(world, world_camera);
			}
			else
			{
				/*
				Body body;

				body.bodyType = (BodyType)state.BodyTypeActive;
				body.position = GetMousePosition();

				float angle = GetRandomFloat() * (2 * PI);
				Vector2 direction = { cosf(angle), sinf(angle) };

				body.color = ColorFromHSV(GetRandomFloat() * 360.0f, 1.0f, 1.0f);

				body.AddForce(direction * (10.0f + state.BodyVelocityValue * 20.0f), ForceMode::VelocityChange);

				body.size = state.BodySizeValue;
				body.restitution = state.BodyRestitutionValue;
				body.mass = state.BodyMassValue;
				body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : (1.0f / body.mass);
				body.gravityScale = state.BodyGravityValue;
				body.damping = state.BodyDampingValue;

				world.AddBody(body);
				*/
				AddBody(world, world_camera);
			}
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			selectedBody = world.GetBodyIntersect(world_camera.ScreenToWorld(GetMousePosition()));
		}

		if (selectedBody) {
			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && IsKeyDown(KEY_LEFT_CONTROL))
			{
				Vector2 position = world_camera.ScreenToWorld(GetMousePosition());
				Vector2 force = Spring::GetSpringForce(position, selectedBody->position, 1.0f, 3.0f);
				selectedBody->AddForce(force);
				DrawLineV(world_camera.WorldToScreen(position), selectedBody->position, WHITE);
			}
		}

		// UPDATE

		if (state.SimulateActive) {
			timeAccum += dt;
			while (timeAccum > fixedTimeStep)
			{
				world.Step(fixedTimeStep);
				timeAccum -= fixedTimeStep;
			}
		}


		// DRAW
		BeginDrawing();
		ClearBackground(BLACK); // clear the background to black

		DrawText(("FPS: " + std::to_string(GetFPS())).c_str(), GetScreenWidth() - 120, 10, 20, WHITE); // draw the current FPS at x=10, y=10 with font size 20 and white color
		//DrawText("Hello Raylib", 200, 200, 20, WHITE); // draw some text at x=200, y=200 with font size 20 and white color
		//DrawTexture(wabbit, 400, 200, WHITE); // draw our texture at x=400, y=200 with white tint (no tint)

		//world.Draw(wabbit);
		world_camera.Begin();
		world.Draw();
		world_camera.End();

		if (selectedBody) {
			DrawCircleLinesV(selectedBody->position, selectedBody->size + 5.0f, RED);
			 if (connectedBody) DrawLineV(selectedBody->position, connectedBody->position, WHITE);
		}

		GuiPhysics(&state);
		
		// END_DRAW
		EndDrawing();
	}

	// EXIT
	UnloadTexture(wabbit); // Unload the texture from GPU memory
	CloseWindow();
	return 0;
}
