#include "VulkanFence.h"
#include "Log.h"

namespace MVK
{
	VulkanFence::~VulkanFence()
	{
		if (mFence != nullptr)
		{
			vkDestroyFence(mDevice->GetHandle(), mFence, nullptr);
		}
	}

	std::shared_ptr<VulkanFence> VulkanFence::Create(const std::shared_ptr<VulkanDevice>& device, VkFenceCreateFlags flags /*VK_FENCE_CREATE_SIGNALED_BIT*/)
	{
		std::shared_ptr<VulkanFence> ret = std::make_shared<VulkanFence>();
		ret->mDevice = device;

		VkFenceCreateInfo creatInfo{};
		creatInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		creatInfo.flags = flags;

		VkResult result = vkCreateFence(device->GetHandle(), &creatInfo, nullptr, &ret->mFence);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create fence");
		}
		return ret;
	}

	VkResult VulkanFence::Reset()
	{
		return vkResetFences(mDevice->GetHandle(), 1, &mFence);
	}

	VkResult VulkanFence::Wait(uint32_t timeOut)
	{
		return vkWaitForFences(mDevice->GetHandle(), 1, &mFence, VK_TRUE, timeOut);
	}
}