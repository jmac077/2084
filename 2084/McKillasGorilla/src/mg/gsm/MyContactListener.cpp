#include "mg\gsm\sprite\AnimatedSprite.h"
#include "mg\gsm\MyContactListener.h"

void MyContactListener::BeginContact(b2Contact* contact) {

		//check if fixture A was a ball
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData && contact->GetFixtureB()->IsSensor()) {
			static_cast<AnimatedSprite*>(bodyUserData)->startContact();
			int i = (int)(contact->GetFixtureB()->GetBody()->GetUserData());
			static_cast<AnimatedSprite*>(bodyUserData)->setDestPos(i);
		}

		//check if fixture B was a ball
		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData && contact->GetFixtureA()->IsSensor()) {
			static_cast<AnimatedSprite*>(bodyUserData)->startContact();
			int i = (int)(contact->GetFixtureA()->GetBody()->GetUserData());
			static_cast<AnimatedSprite*>(bodyUserData)->setDestPos(i);
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