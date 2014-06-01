#include <Box2D/Box2D.h>

#include <memory>

#include "Physics.h"

using namespace Zk::Game;

BodyCollisionListener::BodyCollisionListener(b2Body * filteringBody)
{
	this->filteringBody = filteringBody;
}

static bool contactContainsBody(b2Contact * contact, b2Body * body)
{
	return (body == contact->GetFixtureA()->GetBody())
		|| (body == contact->GetFixtureB()->GetBody());
}

bool BodyCollisionListener::interestedInBeginContactEvent(b2Contact * contact)
{
	return contactContainsBody(contact, filteringBody);
}

bool BodyCollisionListener::interestedInEndContactEvent(b2Contact * contact)
{
	return contactContainsBody(contact, filteringBody);
}

ContactListener::ContactListener()
{
	
}

void ContactListener::BeginContact(b2Contact * contact)
{
	for (auto wcl : collisionListeners)
	{
		auto cl = wcl.lock();
		if (cl->interestedInBeginContactEvent(contact))
			cl->onBeginContactEvent(contact);
	}
}
void ContactListener::EndContact(b2Contact * contact)
{
	for (auto wcl : collisionListeners)
	{
		auto cl = wcl.lock();
		if (cl->interestedInEndContactEvent(contact))
			cl->onEndContactEvent(contact);
	}
}

void ContactListener::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
	
}

void ContactListener::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{
	
}

void ContactListener::registerListener(std::weak_ptr<CollisionListener> cl)
{
	collisionListeners.push_back(cl);
}

void ContactListener::discardOldListeners()
{
	std::remove_if(
		collisionListeners.begin(), collisionListeners.end(),
		[](const std::weak_ptr<CollisionListener> & cl) -> bool
		{
			return cl.expired();
		}
	);
}

PhysicsSystem::PhysicsSystem()
	: world(b2Vec2(0.0, 9.8))
{
	world.SetContactListener(&cl);
}

PhysicsSystem::~PhysicsSystem()
{
	
}

void PhysicsSystem::registerListener(std::weak_ptr<CollisionListener> cl)
{
	this->cl.registerListener(cl);
}

void PhysicsSystem::simulate(double step)
{
	cl.discardOldListeners();
	world.Step(step, 8, 3);
}
