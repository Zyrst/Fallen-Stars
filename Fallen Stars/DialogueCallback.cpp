#include "DialogueCallback.h"

#include "Player.h"
#include <Box2D\Box2D.h>
#include <iostream>

namespace
{
	Player* getPlayer(b2Fixture* otherFixture)
	{
		Entity* ent = (Entity*)otherFixture->GetBody()->GetUserData();

		if (ent != nullptr && !otherFixture->IsSensor())
		{
			return dynamic_cast<Player*>(ent);
		}

		return nullptr;
	}
}

DialogueCallback::DialogueCallback(b2Fixture* owner, DialogueOverlay& dialogueOverlay, Conversation* conversation)
: CallBack(owner)
, mDialogueOverlay(dialogueOverlay)
, mConversation(conversation)
, triggered(false)
{

}

void DialogueCallback::beginContact(b2Fixture* otherFixture)
{
	Player* p = getPlayer(otherFixture);
	if (p != nullptr)
	{
		if(!triggered)
		{
			mDialogueOverlay.setActiveConversation(mConversation);
			mDialogueOverlay.setEnabled(true);
			triggered = true;
		}
	}
}

void DialogueCallback::endContact(b2Fixture* otherFixture)
{

}