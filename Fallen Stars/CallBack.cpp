#include "CallBack.h"
#include <Box2D\Dynamics\b2Fixture.h>

CallBack::CallBack(b2Fixture* ow) : owner(ow) { owner->SetUserData(this); }

CallBack::~CallBack() { }
