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
void AnimatedSprite::endContact() { m_contacting = true; }

void AnimatedSprite::handleCollision() {
	if (collisionZone->getType() == TeleporterFlag)
	{
		teleportPlayer((Teleporter*)collisionZone->getZone());
	}
	else if (collisionZone->getType() == CheckpointFlag)
	{
		hitCheckpoint((Checkpoint*)collisionZone->getZone());
	}
}

void AnimatedSprite::teleportPlayer(Teleporter *teleportTarget) {
	// MOVE PLAYER TO TELEPORT LOCATION
	getB2Body()->SetTransform(b2Vec2(teleportTarget->getDestX(), teleportTarget->getDestY()), 0.0f);
	m_contacting = false;
	getB2Body()->SetLinearVelocity(b2Vec2(0.0f,0.0f));
	// SWITCH CURRENT LEVEL SECTION
	Game::getSingleton()->getGSM()->getWorld()->setCurrentLevelSection(teleportTarget->getTargetSection());
}

void AnimatedSprite::hitCheckpoint(Checkpoint *checkpoint) {
	// IF CHECKPOINT HASN'T ALREADY BEEN ACTIVATED, SET AS CURRENT CHECKPOINT
	if (!checkpoint->getActivated()) {
		checkpoint->setPlayerPos(getB2Body()->GetPosition().x, getB2Body()->GetPosition().y);
		Game::getSingleton()->getGSM()->getWorld()->setCurrentCheckpoint(checkpoint);
	}
}

void AnimatedSprite::killSprite() {
	respawnAtLastCheckpoint();
}

void AnimatedSprite::respawnAtLastCheckpoint() {
	Checkpoint *lastCheckpoint = Game::getSingleton()->getGSM()->getWorld()->getCurrentCheckpoint();
	if (lastCheckpoint == nullptr)
		return;
	// MOVE PLAYER TO WHERE THEY HIT THE LAST CHECKPOINT
	getB2Body()->SetTransform(b2Vec2(lastCheckpoint->getPlayerX(), lastCheckpoint->getPlayerY()), 0.0f);
	getB2Body()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	// SWITCH CURRENT LEVEL SECTION
	Game::getSingleton()->getGSM()->getWorld()->setCurrentLevelSection(lastCheckpoint->getLevelSection());
}