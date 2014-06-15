#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <QDebug>

#include <memory>

#include "../../ZkCommon/Constants.h"
#include "../../ZkCommon/LibraryCast.h"

#include "BulletEntity.h"
#include "PlayerEntity.h"
#include "../Renderables/BoxRenderable.h"
#include "../Physics.h"
#include "../GameSystem.h"

using namespace Zk::Common;
using namespace Zk::Game;

BulletEntity::BulletEntity(
	sf::Vector2f pos,
	sf::Vector2f velocity,
	std::weak_ptr<PlayerEntity> owner,
	double damage
) :
	Entity(nullptr, nullptr),
	BodyCollisionListener(nullptr),
	std::enable_shared_from_this<BulletEntity>()
{
	this->owner = owner;
	this->damage = damage;
	
	b2World & world = GameSystem::getInstance()->getPhysicsSystem().getWorld();
	
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = lib_cast<b2Vec2>(pos);
	bodyDef.linearVelocity = lib_cast<b2Vec2>(velocity);
	bodyDef.bullet = true;
	bodyDef.userData = (void*)this;
	b2Body * body = world.CreateBody(&bodyDef);
	
	b2CircleShape circleShape;
	circleShape.m_radius = 0.05f;
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 2.0f;
	fixtureDef.friction = 1.0f;
	body->CreateFixture(&fixtureDef);
	body->SetFixedRotation(true);
	
	setBody(body);
	setFilteringBody(body);
	
	BoxRenderable * br = new BoxRenderable(
		body, GameSystem::resourcePath("grenade-pack.png").c_str()
	);
	br->setZValue(-(double)LayerType::MIDGROUND);
	setRenderable(br);
}

BulletEntity::~BulletEntity()
{
	
}

void BulletEntity::registerMe()
{
	GameSystem::getInstance()->getPhysicsSystem().registerListener(
		shared_from_this()
	);
}

void BulletEntity::onBeginContactEvent(b2Contact * contact)
{
	
}

void BulletEntity::onEndContactEvent(b2Contact * contact)
{
	
}

void BulletEntity::onPreSolveEvent(b2Contact * contact, const b2Manifold * oldManifold)
{
	ContactInfo ci(getBody(), contact);
	Entity * ent = (Entity*)ci.toucher->GetUserData();
	if (ent->getType() == EntityType::PlayerEntity)
	{
		PlayerEntity * ceEnt = (PlayerEntity*)ent;
		if (ceEnt == owner.lock().get())
		{
			//Samego siebie nie krzywdzimy
			contact->SetEnabled(false);
			return;
		}
		
		ceEnt->takeDamage(damage);
	}
	
	//Przy uderzeniu w cokolwiek innego niż gracz,
	//który nas wystrzelił, giniemy
	qDebug() << "BEEP";
	markForDeletion();
}

void BulletEntity::onPostSolveEvent(b2Contact * contact, const b2ContactImpulse * impulse)
{
	
}

void BulletEntity::update(double step)
{
	
}

EntityType BulletEntity::getType() const
{
	return EntityType::BulletEntity;
}