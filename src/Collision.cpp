#include "Collision.h"
#include "raymath.h"

bool Intersects(const Body& bodyA, const Body& bodyB)
{
	float distance = Vector2Distance(bodyA.position,bodyB.position);
	float radius = bodyA.size + bodyB.size;

	return (distance < radius);
}

void CreateContacts(std::vector<Body>& bodies, std::vector<Contact>& contacts)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		for (int j = i + 1; j < bodies.size(); j++)
		{
			Body& bodyA = bodies[i];
			Body& bodyB = bodies[j];

			if (bodyA.bodyType != BodyType::Dynamic && bodyB.bodyType != BodyType::Dynamic) continue;
			
			if (Intersects(bodyA, bodyB))
			{
				Contact contact;
				contact.bodyA = &bodyA;
				contact.bodyB = &bodyB;
			
				Vector2 direction = bodyA.position - bodyB.position;
				float distance = Vector2Length(direction);

				// SPECIAL CASE: bodies at the exact same position
				if (distance == 0.0f)
				{
					// Give them a tiny random separation direction
					float angle = GetRandomValue(0, 359) * DEG2RAD;
					direction = { cosf(angle), sinf(angle) };
					distance = 0.001f; // tiny non-zero distance
				}
			
				float radius = bodyA.size + bodyB.size;
				contact.depth = radius - distance;
				contact.normal = Vector2Normalize(direction);
				contact.restitution = (bodyA.restitution + bodyB.restitution) * 0.5f;
			
				contacts.push_back(contact);
			}
		}
	}
}

void SeparateContacts(std::vector<Contact>& contacts)
{
	for (auto& contact : contacts)
	{
		float totalInverseMass = contact.bodyA->inverseMass + contact.bodyB->inverseMass;
		Vector2 separation = contact.normal * (contact.depth / totalInverseMass);
		contact.bodyA->position = contact.bodyA->position + (separation * contact.bodyA->inverseMass);
		contact.bodyB->position = contact.bodyB->position - (separation * contact.bodyB->inverseMass);
	}
}

void ResolveContacts(std::vector<Contact>& contacts)
{
	for (auto& contact : contacts)
	{
		Body* A = contact.bodyA;
		Body* B = contact.bodyB;

		// compute relative velocity
		Vector2 rv = A->velocity - B->velocity;

		// project relative velocity onto the contact normal
		float nv = Vector2DotProduct(rv, contact.normal);

		// skip if bodies are separating
		if (nv > 0.0f) continue;

		// total inverse mass = (1/mA + 1/mB)
		float totalInverseMass = A->inverseMass + B->inverseMass;
		if (totalInverseMass <= 0.0f) continue; // both static

		// impulse scalar = -(1 + restitution) * vn / (1/mA + 1/mB)
		float impulseMagnitude = -(1.0f + contact.restitution) * nv / totalInverseMass;

		// impulse vector along contact normal
		Vector2 impulse = contact.normal * impulseMagnitude;

		// apply equal and opposite impulses
		A->AddForce(impulse, ForceMode::Impulse);
		B->AddForce(Vector2Negate(impulse), ForceMode::Impulse);
	}
}
