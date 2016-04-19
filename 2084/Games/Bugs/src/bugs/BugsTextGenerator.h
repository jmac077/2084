/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	BugsTextGenerator.h

	This class is a custom text generator for the EmptyGame
	application. Basically, by defining the initText and updateText
	methods, we can have it display whatever text we like.
*/

#pragma once
#include "bugs_VS\stdafx.h"
#include "mg\text\TextGenerator.h"

class BugsTextGenerator : public TextGenerator
{
private:
	// THIS IS THE TEXT THIS OBJECT WILL UPDATE EACH FRAME
	wstring leftTextToGenerate;
	wstring rightTextToGenerate;

	// THESE ARE HELPER METHODS FOR GENERATING THE TEXT, CALLED
	// EACH FRAME BY updateText

	// FOR THE LEFT COLUMN
	void appendMouseCoords();
	void appendClock();
	void appendBotCount();

	// AND THE RIGHT COLUMN
	void appendBotRecycler();

public:
	// NOTHING TO INITIALIZE OR DESTROY
	BugsTextGenerator()		{}
	~BugsTextGenerator()	{}

	// DEFINED IN BugsTextGenerator.cpp
	void startUp();
	void update();
};