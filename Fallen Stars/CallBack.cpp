#include "CallBack.h"
#include <Box2D\Box2D.h>

CallBack::CallBack(b2Fixture* ow) : owner(ow) { owner->SetUserData(this); }

CallBack::~CallBack() { }
