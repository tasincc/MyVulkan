#include "VulkanQueue.h"
#include "Log.h"

namespace MVK
{
	std::shared_ptr<VulkanQueue> VulkanQueue::Create(const std::shared_ptr<VulkanDevice>& device, VkQueueFlags m_requestQueue)
	{
		std::shared_ptr<VulkanQueue> ret = std::make_shared<VulkanQueue>();
		ret->mDevice = device;

		//graphic
		QueueFamilyIndices indices = device->GetQueueFamilyIndices();
		if (m_requestQueue == VK_QUEUE_GRAPHICS_BIT)
		{
			if (indices.graphicsFamily.has_value())
				ret->mFamilyIndex = indices.graphicsFamily.value();
			else
				CORE_ERROR("failed to fecth graphic queue");
		}

		//transfer
		if (m_requestQueue == VK_QUEUE_TRANSFER_BIT)
		{
			if (indices.transferFamilty.has_value())
				ret->mFamilyIndex = indices.transferFamilty.value();
			else
				CORE_ERROR("failed to fecth transfer queue");
		}

		//compute
		if (m_requestQueue == VK_QUEUE_COMPUTE_BIT)
		{
			if (indices.computeFamily.has_value())
				ret->mFamilyIndex = indices.computeFamily.value();
			else
				CORE_ERROR("failed to fecth compute queue");
		}

		vkGetDeviceQueue(device->GetHandle(), ret->mFamilyIndex, 0, &ret->mQueue);
		return ret;
	}

	std::shared_ptr<VulkanPresentQueue> VulkanPresentQueue::Create(const std::shared_ptr<VulkanDevice>& device)
	{
		std::shared_ptr<VulkanPresentQueue> ret = std::make_shared<VulkanPresentQueue>();
		ret->mDevice = device;

		//present
		QueueFamilyIndices indices = device->GetQueueFamilyIndices();
		if (indices.graphicsFamily.has_value())
		{
			ret->mFamilyIndex = indices.graphicsFamily.value();
		}
		else
		{
			CORE_ERROR("failed to fecth present queue");
		}

		vkGetDeviceQueue(device->GetHandle(), ret->mFamilyIndex, 0, &ret->mQueue);
		return ret;
	}
}