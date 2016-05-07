#include "mg\gsm\sprite\AnimatedSprite.h"
#include "mg\gsm\MyContactListener.h"
#include "mg\gsm\world\Teleporter.h"

void MyContactListener::BeginContact(b2Contact* contact) {

		//check if fixture A was a ball
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData && contact->GetFixtureB()->IsSensor()) {
			static_cast<AnimatedSprite*>(bodyUserData)->startContact();
			CollidableZone *zone = (CollidableZone*)(contact->GetFixtureB()->GetBody()->GetUserData());
			static_cast<AnimatedSprite*>(bodyUserData)->setCollisionZone(zone);
		}

		//check if fixture B was a ball
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData && contact->GetFixtureA()->IsSensor()) {
			static_cast<AnimatedSprite*>(bodyUserData)->startContact();
			CollidableZone *zone = (CollidableZone*)(contact->GetFixtureA()->GetBody()->GetUserData());
			static_cast<AnimatedSprite*>(bodyUserData)->setCollisionZone(zone);
		}

}

void MyContactListener::EndContact(b2Contact* contact) {

		//check if fixture A was a ball
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData && contact->GetFixtureB()->IsSensor()) {
			static_cast<AnimatedSprite*>(bodyUserData)->endContact();
		}

		//check if fixture B was a ball
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData && contact->GetFixtureA()->IsSensor()) {
			static_cast<AnimatedSprite*>(bodyUserData)->endContact();
		}
}