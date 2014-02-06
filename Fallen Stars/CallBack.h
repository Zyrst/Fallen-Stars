#pragma once

class b2Fixture;

class CallBack
{
public:
	CallBack(b2Fixture* owner);
	virtual ~CallBack();

	virtual void beginContact(b2Fixture* otherFixture) = 0;
	virtual void endContact(b2Fixture* otherFixture) = 0;

protected:
	b2Fixture* owner;
};

