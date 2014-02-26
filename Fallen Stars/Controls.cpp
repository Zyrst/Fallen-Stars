#include "Controls.h"

#include <iostream>

using namespace Controls;

Action& Controls::operator++(Action& original)
{
	if (original == UNUSED)
	{
		std::cout << "Warning! Incrementing beyond endpoint of Controls::Action" << std::endl;
		return original = UNUSED;
	}
	
	unsigned int enumValue = original;
	return original = static_cast<Action>( ++enumValue );
}