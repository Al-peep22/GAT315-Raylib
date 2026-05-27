#pragma once
#include <vector>
#include "Body.h"
#include "Effector.h"
#include "Collision.h"
#include "../Spring.h"

struct World
{
public:
    World() {
        bodies.reserve(1000);
    }

    void AddBody(const Body& body);
    void AddEffector(Effector* effector);
    void AddSpring(Body& bodyA, Body& bodyB, float restLength, float stiffness);
    
    void Step(float dt);
    void Draw(Texture2D wabbit) const;
    void Draw();
    void UpdateCollision();

    Body* GetBodyIntersect(const Vector2& position);

    static void SetGravity(Vector2 newGravity) { gravity = newGravity; }
    void SetBounds(Vector2 min, Vector2 max) { boundsMin = min; boundsMax = max; }

private:
    std::vector<Body> bodies;
    static Vector2 gravity;
    std::vector<Effector*> effectors;
    std::vector<Spring*> springs;
    std::vector<Contact> contacts;

    Vector2 boundsMin{ -10.0f, -5.0f };
    Vector2 boundsMax{ 10.0f,  5.0f };
};
