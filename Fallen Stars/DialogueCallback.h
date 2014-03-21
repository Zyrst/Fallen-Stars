#pragma once

#include "CallBack.h"
#include "DialogueOverlay.h"

class b2Fixture;

class DialogueCallback : public CallBack
{
public:
	DialogueCallback(b2Fixture* owner, DialogueOverlay& dialogueOverlay, Conversation* conversation);

	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

private:
	DialogueOverlay& mDialogueOverlay;
	Conversation* mConversation;
	bool triggered;
};

class StreetLightSensorCallBack : public CallBack
{
public:
	StreetLightSensorCallBack(b2Fixture* owner);
	virtual ~StreetLightSensorCallBack();

	
};
