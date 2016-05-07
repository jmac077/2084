#include "mg\gsm\sprite\AnimatedSprite.h"
#include "mg\gsm\MyContactListener.h"
#include "mg\gsm\world\Teleporter.h"

void MyContactListener::BeginContact(b2Contact* contact) {

		//check if fixture A was a ball
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData && contact->GetFixtureB()->IsSensor()) {
			static_cast<AnimatedSprite*>(bodyUserData)->startContact();
			Teleporter *tel = (Teleporter*)(contact->GetFixtureB()->GetBody()->GetUserData());
			static_cast<AnimatedSprite*>(bodyUserData)->setTeleportTarget(tel);
		}

		//check if fixture B was a ball
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData && contact->GetFixtureA()->IsSensor()) {
			static_cast<AnimatedSprite*>(bodyUserData)->startContact();
			Teleporter *tel = (Teleporter*)(contact->GetFixtureA()->GetBody()->GetUserData());
			static_cast<AnimatedSprite*>(bodyUserData)->setTeleportTarget(tel);
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