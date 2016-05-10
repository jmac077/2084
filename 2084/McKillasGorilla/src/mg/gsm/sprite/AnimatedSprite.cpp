/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSprite.cpp

	See AnimatedSprite.h for a class description.
*/

#include "mg_VS\stdafx.h"
#include "mg\game\Game.h"
#include "mg\gsm\sprite\AnimatedSprite.h"
#include "mg\gsm\sprite\AnimatedSpriteType.h"
#include "mg\gsm\state\GameStateManager.h"

/*
	AnimatedSprite - Default constructor, just sets everything to 0.
*/
AnimatedSprite::AnimatedSprite()  
{
	spriteType = 0;
	frameIndex = 0;
	animationCounter = 0;
	dead = false;
	markedForRemoval = false;
	currentPathToFollow = new list<PathNode>(0);
}

/*
	Destructor - Nothing to clean up. We don't want to clean up the
	sprite type because it is a shared variable.
*/
AnimatedSprite::~AnimatedSprite() 
{

}

/*
	changeFrame - This method allows for the changing of an image in an 
	animation sequence for a given animation state.
*/
void AnimatedSprite::changeFrame()
{
	// RESET THE COUNTER
	animationCounter = 0;

	// REMEMBER, WE COUNT BY 2s
	frameIndex += 2;

	// GO BACK TO FIRST INDEX IF NECESSARY
	if (frameIndex == spriteType->getSequenceSize(currentState))
		frameIndex = 0;
}

/*
	getCurrentImageID - This method does the work of retrieving
	the image id that corresponds to this sprite's current state
	and frame index.
*/
unsigned int AnimatedSprite::getCurrentImageID()
{
	return spriteType->getAnimationFrameID(currentState, frameIndex);
}

/*
	setCurrentState - This method sets this sprite to the newState
	state and starts its animtion sequence from the beginning.
*/
void AnimatedSprite::setCurrentState(wstring newState) 
{
	string cs(currentState.begin(), currentState.end());
	string ns(newState.begin(), newState.end());
	if (strcmp(cs.c_str(), ns.c_str()) != 0)
	{
		// SET THE ANIMATINO STATE
		currentState = newState;

		// AND RESET OUR COUNTERS
		animationCounter = 0;
		frameIndex = 0;
	}
}

/*
	updateSprite - To be called every frame of animation. This
    method advances the animation counter appropriately per
	the animation speed. It also updates the sprite location
	per the current velocity.
*/
void AnimatedSprite::updateSprite()
{
	Game *game = Game::getSingleton();
	if (currentPathToFollow->size()>0) {
		float playerX = getB2Body()->GetLinearVelocity().x;
		float playerY = getB2Body()->GetLinearVelocity().y;
		if ((abs((getB2Body()->GetPosition().x) - (getCurrentPathNode()->column * 2 + 1)) <= abs(playerX*(1 / 60.0f))) && (abs((getB2Body()->GetPosition().y) - (getCurrentPathNode()->row * 2 + 1)) <= abs(playerY*(1 / 60.0f)))) {
			game->getPathFinder()->updatePath(this);
		}
	}
	if (m_contacting && collisionBehavior != NULL ) {
		(this->*(collisionBehavior))();
	}

	unsigned int duration = spriteType->getDuration(currentState, frameIndex);
	animationCounter++;

	// WE ONLY CHANGE THE ANIMATION FRAME INDEX WHEN THE
	// ANIMATION COUNTER HAS REACHED THE DURATION
	if (animationCounter >= duration)
		changeFrame();

	// AND NOW CALCULATE THE RADIANS
	// FOR RENDERING THIS SPRITE
	float vX = myBody->GetLinearVelocity().x*32;
	float vY = myBody->GetLinearVelocity().y*32;
	float epsilon = 0.00001f;

	// IF THE Y VELOCITY IS 0 WE HAVE TO BE CAREFUL DIVIDING
	if (fabs(vY) < epsilon)
	{
		// IF X ISN'T MOVING EITHER, LEAVE IT WHAT IT IS
		// AND PUT IT INTO AN IDLE STATE
		if (fabs(vX) > epsilon)
		{
			if (vX < 0.0f)
			{
				//rotationInRadians = M_PI + (M_PI/2);
				;
			}
			else
				//rotationInRadians = 0.0f + (M_PI / 2);
				;
		}
	}
	else
	{
		float tanTheta = vY / vX;
		//rotationInRadians = atan(tanTheta) + (M_PI / 2);
		if (vX < 0.0f)
		{
			if (vY < 0.0f)
				//rotationInRadians += M_PI;
				;
			else
				//rotationInRadians -= M_PI;
				;
		}
	}
}

void AnimatedSprite::startContact() { m_contacting = true; }
void AnimatedSprite::endContact() { m_contacting = false; }

void AnimatedSprite::handleCollision() {
	if(collisionZone == NULL){
		//player collided with drone sensor
		killSprite();
		endContact();
	}
	else if (collisionZone->getType() == TeleporterFlag)
	{
		teleportPlayer((Teleporter*)collisionZone->getZone());
	}
	else if (collisionZone->getType() == CheckpointFlag)
	{
		hitCheckpoint((Checkpoint*)collisionZone->getZone());
	}
	else if (collisionZone->getType() == HiddenWallFlag)
	{
		raiseWall();
	}
	else if (collisionZone->getType() == WorldItemFlag)
	{
		interactWithItem((WorldItem*)collisionZone->getZone());
	}
}

void AnimatedSprite::teleportPlayer(Teleporter *teleportTarget) {
	GameStateManager *gsm = Game::getSingleton()->getGSM();
	if (gsm->getWorld()->getRenderHiddenStuff())
		gsm->getSoundManager()->PlaySoundEffect(TELEPORT_SFX);
	// MOVE PLAYER TO TELEPORT LOCATION
	getB2Body()->SetTransform(b2Vec2(teleportTarget->getDestX(), teleportTarget->getDestY()), 0.0f);
	m_contacting = false;
	getB2Body()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	// SWITCH CURRENT LEVEL SECTION
	gsm->getWorld()->setCurrentLevelSection(teleportTarget->getTargetSection());
}

void AnimatedSprite::hitCheckpoint(Checkpoint *checkpoint) {
	// IF CHECKPOINT HASN'T ALREADY BEEN ACTIVATED, SET AS CURRENT CHECKPOINT
	if (!checkpoint->getActivated()) {
		checkpoint->setPlayerPos(getB2Body()->GetPosition().x, getB2Body()->GetPosition().y);
		Game::getSingleton()->getGSM()->getWorld()->setCurrentCheckpoint(checkpoint);
	}
}

void AnimatedSprite::raiseWall() {
	GameStateManager *gsm = Game::getSingleton()->getGSM();
	gsm->getSoundManager()->PlaySong(DETECTED_SONG);

	//get dimensions of wall create body and add it to wall
	//add graphics to world renderer
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(143, 141);
	b2Body* body = gsm->getB2World()->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(7.0f, 2.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	body->CreateFixture(&fixtureDef);
	//also add static tiles that teleport player
	Teleporter *teleportTarget = new Teleporter(176, 22, 4);

	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(168, 154);
	body = gsm->getB2World()->CreateBody(&bodyDef);
	dynamicBox.SetAsBox(2, 2);
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);
	body->SetUserData(new CollidableZone(teleportTarget, TeleporterFlag));

	teleportTarget = new Teleporter(122, 74, 4);

	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(126, 164);
	body = gsm->getB2World()->CreateBody(&bodyDef);
	dynamicBox.SetAsBox(2, 2);
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);
	body->SetUserData(new CollidableZone(teleportTarget, TeleporterFlag));

	teleportTarget = new Teleporter(120, 12, 4);

	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(148, 82);
	body = gsm->getB2World()->CreateBody(&bodyDef);
	dynamicBox.SetAsBox(4, 4);
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);
	body->SetUserData(new CollidableZone(teleportTarget, TeleporterFlag));

	teleportTarget = new Teleporter(166, 175, 4);

	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(142, 12);
	body = gsm->getB2World()->CreateBody(&bodyDef);
	dynamicBox.SetAsBox(2, 2);
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);
	body->SetUserData(new CollidableZone(teleportTarget, TeleporterFlag));

	teleportTarget = new Teleporter(120, 40, 4);

	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(122, 22);
	body = gsm->getB2World()->CreateBody(&bodyDef);
	dynamicBox.SetAsBox(2, 2);
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);
	body->SetUserData(new CollidableZone(teleportTarget, TeleporterFlag));

	teleportTarget = new Teleporter(122, 180, 4);

	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(120, 56);
	body = gsm->getB2World()->CreateBody(&bodyDef);
	dynamicBox.SetAsBox(2, 2);
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);
	body->SetUserData(new CollidableZone(teleportTarget, TeleporterFlag));

	teleportTarget = new Teleporter(178, 74, 4);

	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(176, 56);
	body = gsm->getB2World()->CreateBody(&bodyDef);
	dynamicBox.SetAsBox(2, 2);
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);
	body->SetUserData(new CollidableZone(teleportTarget, TeleporterFlag));

	teleportTarget = new Teleporter(156, 122, 4);

	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(178, 88);
	body = gsm->getB2World()->CreateBody(&bodyDef);
	dynamicBox.SetAsBox(2, 2);
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);
	body->SetUserData(new CollidableZone(teleportTarget, TeleporterFlag));

	gsm->getWorld()->setRenderHiddenStuff(true);
}

void AnimatedSprite::interactWithItem(WorldItem *item) {
	if (item->getActive()) {
		item->setActive(false);
		GameStateManager *gsm = Game::getSingleton()->getGSM();
		gsm->setFlag(item->getFlag());
		gsm->getSoundManager()->PlaySoundEffect(PICK_UP_ITEM_SFX);
	}
}

void AnimatedSprite::killSprite() {
	dead = true;
	GameStateManager *gsm = Game::getSingleton()->getGSM();
	gsm->goToGameOver();
	setCurrentState(L"BENDING_FRONT");
	gsm->getSoundManager()->PlaySong(CENSORED_SONG);
}

void AnimatedSprite::respawnAtLastCheckpoint() {
	dead = false;
	Checkpoint *lastCheckpoint = Game::getSingleton()->getGSM()->getWorld()->getCurrentCheckpoint();
	if (lastCheckpoint == nullptr)
		return;
	// MOVE PLAYER TO WHERE THEY HIT THE LAST CHECKPOINT
	getB2Body()->SetTransform(b2Vec2(lastCheckpoint->getPlayerX(), lastCheckpoint->getPlayerY()), 0.0f);
	m_contacting = false;
	getB2Body()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	// SWITCH CURRENT LEVEL SECTION
	Game::getSingleton()->getGSM()->getWorld()->setCurrentLevelSection(lastCheckpoint->getLevelSection());
}