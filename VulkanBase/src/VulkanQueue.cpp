#include "VulkanQueue.h"

namespace MVK
{
	std::shared_ptr<VulkanQueue> VulkanQueue::createVulkanQueue(const std::shared_ptr<VulkanDevice> device, uint32_t familyIndex)
	{
		std::shared_ptr<VulkanQueue> ret = std::make_shared<VulkanQueue>();
		ret->m_familyIndex = familyIndex;
		ret->m_device = device;
		vkGetDeviceQueue(device->GetHandle(), familyIndex, 0, &ret->m_queue);
		return ret;
	}

	VulkanQueue::~VulkanQueue()
	{

	}
}