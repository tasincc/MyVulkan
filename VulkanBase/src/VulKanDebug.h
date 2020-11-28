#ifndef VULKANDEBUG_H
#define VULKANDEBUG_H

#include "vulkan/vulkan.h"

namespace MVK
{
	namespace DEBUG
	{		
		void setUpDebug(VkInstance instance);
		void freeDebug(VkInstance instance);
	}
}

#endif // !VULKANDEBUG_H
