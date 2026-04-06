#include "World.h"
#include "raylib.h"
#include "raymath.h"

World::World()
{
    gravity = { 0, 9.81f };
    bodies.reserve(1000);
}

void World::AddBody(const Body& body)
{
    bodies.push_back(body);
}

void World::Step(float dt)
{
    // Reset acceleration
    for (auto& body : bodies)
        body.acceleration = { 0, 0 };

    // Apply gravity
    for (auto& body : bodies)
        body.AddForce(gravity * 100.0f);

    // Apply right-click force
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Vector2 pos = GetMousePosition();
        for (auto& body : bodies)
        {
            Vector2 dir = body.position - pos;
            if (Vector2Length(dir) <= 100.0f)
            {
                Vector2 force = Vector2Normalize(dir) * 10000.0f;
                body.AddForce(force);
            }
        }
        DrawCircleLinesV(pos, 100, WHITE);
    }

    // Integrate
    for (auto& body : bodies)
        body.Step(dt);

    // Collisions
    for (auto& body : bodies)
    {
        if (body.position.x + body.size > GetScreenWidth())
        {
            body.position.x = GetScreenWidth() - body.size;
            body.velocity.x *= -body.restitution;
        }
        if (body.position.x - body.size < 0)
        {
            body.position.x = body.size;
            body.velocity.x *= -body.restitution;
        }
        if (body.position.y + body.size > GetScreenHeight())
        {
            body.position.y = GetScreenHeight() - body.size;
            body.velocity.y *= -body.restitution;
        }
    }
}

void World::Draw(Texture2D wabbit) const
{
    for (const auto& body : bodies)
        DrawTexture(wabbit, body.position.x, body.position.y, WHITE);
}