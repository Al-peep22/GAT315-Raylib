#include "World.h"
#include "raylib.h"
#include "raymath.h"
#include <Integrater.h>

Vector2 World::gravity = {0,9.8f};

void World::AddBody(const Body& body)
{
    bodies.push_back(body);
}

void World::AddEffector(Effector* effector)
{
    effectors.push_back(effector);
}

void World::AddSpring(Body& bodyA, Body& bodyB, float restLength, float stiffness)
{
	//Spring sprig = Spring(&bodyA, &bodyB, restLength, stiffness);
	//springs.push_back(sprig);
}

void World::Step(float dt)
{

    // Apply world gravity
    for (auto& body : bodies)
        body.AddForce(gravity * body.gravityScale, ForceMode::Acceleration);

    // Apply effectors (NEW)
    for (auto& effector : effectors)
        effector->Apply(bodies);

    for (auto& spring : springs)
		spring->Apply(100.0f);

    for (auto& body : bodies)
        if (body.bodyType == BodyType::Dynamic)
			body.AddForce(body.velocity * -body.damping, ForceMode::Force);

    // Reset acceleration
    for (auto& body : bodies)
        body.acceleration = { 0, 0 };

    // Right-click radial force
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Vector2 pos = GetMousePosition();
        for (auto& body : bodies)
        {
            Vector2 dir = Vector2Subtract(body.position, pos);
            if (Vector2Length(dir) <= 100.0f)
            {
                Vector2 force = Vector2Scale(Vector2Normalize(dir), 10000.0f);
                body.AddForce(force);
            }
        }
    }

    // Integrate
    for (auto& body : bodies)
        SemiImplicitEuler(body, dt);

    for (int i = 0; i < 4; i++) {
	    UpdateCollision();
    }

    //// Collisions
    //for (auto& body : bodies)
    //{
    //    if (body.position.x + body.size > GetScreenWidth())
    //    {
    //        body.position.x = GetScreenWidth() - body.size;
    //        body.velocity.x *= -body.restitution;
    //    }
    //    if (body.position.x - body.size < 0)
    //    {
    //        body.position.x = body.size;
    //        body.velocity.x *= -body.restitution;
    //    }
    //    if (body.position.y + body.size > GetScreenHeight())
    //    {
    //        body.position.y = GetScreenHeight() - body.size;
    //        body.velocity.y *= -body.restitution;
    //    }
    //}
}

void World::Draw(Texture2D wabbit) const
{
    for (const auto& body : bodies)
        DrawTexture(wabbit, (int)body.position.x, (int)body.position.y, WHITE);
}

void World::Draw()
{
    for (const auto& effector : effectors) effector->Draw();
	for (const auto& body : bodies) body.Draw();
}

void World::UpdateCollision()
{
    contacts.clear();
    CreateContacts(bodies, contacts);
    SeparateContacts(contacts);
    ResolveContacts(contacts);

    // collision
    for (auto& body : bodies)
    {
        if (body.position.x + body.size > boundsMax.x)
        {
            body.position.x = boundsMax.x - body.size;
            body.velocity.x *= -body.restitution;
        }
        if (body.position.x - body.size < boundsMin.x)
        {
            body.position.x = boundsMin.x + body.size;
            body.velocity.x *= -body.restitution;
        }
        if (body.position.y + body.size > boundsMax.y)
        {
            body.position.y = boundsMax.y - body.size;
            body.velocity.y *= -body.restitution;
        }
        if (body.position.y - body.size < boundsMin.y)
        {
            body.position.y = boundsMin.y + body.size;
            body.velocity.y *= -body.restitution;
        }
    }
}

Body* World::GetBodyIntersect(const Vector2& position)
{
    for (auto& body : bodies) {
        if (CheckCollisionPointCircle(position, body.position, body.size))
            return &body;
    }
    return nullptr;
}
