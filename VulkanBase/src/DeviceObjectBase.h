#ifndef DEVICE_OBJECT_BASE_H
#define DEVICE_OBJECT_BASE_H

#include "VulkanDevice.h"

namespace MVK
{
	class DeviceObjectBase
	{
	public:
		virtual const std::shared_ptr<VulkanDevice>& GetDevicePtr() const = 0;
	};
}

#endif
