#pragma once
#include "mg_VS\stdafx.h"
#include "Box2D.h"

class MyContactListener : public b2ContactListener
{
public:
	MyContactListener() {}
	~MyContactListener() {}

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};