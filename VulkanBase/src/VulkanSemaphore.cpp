#include "VulkanSemaphore.h"
#include "Log.h"

namespace MVK
{
	VulkanSemaphore::~VulkanSemaphore()
	{
		if (mSemaphore != nullptr)
		{
			vkDestroySemaphore(mDevice->GetHandle(), mSemaphore, nullptr);
		}
	}

	std::shared_ptr<VulkanSemaphore> VulkanSemaphore::Create(const std::shared_ptr<VulkanDevice>& device)
	{
		std::shared_ptr<VulkanSemaphore> ret = std::shared_ptr<VulkanSemaphore>();
		ret->mDevice = device;

		VkSemaphoreCreateInfo creatInfo{};
		creatInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkResult result = vkCreateSemaphore(device->GetHandle(), &creatInfo, nullptr, &ret->mSemaphore);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create semaphore");
		}

		return ret;
	}

	inline VulkanSemaphoreGroup::VulkanSemaphoreGroup(const std::initializer_list <std::shared_ptr<VulkanSemaphore>>& semaphores)
	{
		for (const auto& semaphore : semaphores)
		{
			mSemaphores.push_back(semaphore->GetHandle());
		}
	}

	inline VulkanSemaphoreGroup::VulkanSemaphoreGroup(const std::vector<std::shared_ptr<VulkanSemaphore>>& semaphores)
	{
		for (const auto& semaphore : semaphores)
		{
			mSemaphores.push_back(semaphore->GetHandle());
		}
	}

	inline VulkanSemaphoreStageGroup::VulkanSemaphoreStageGroup(const std::vector<std::pair<std::shared_ptr<VulkanSemaphore>, VkPipelineStageFlags>>& semaphores)
	{
		for (const auto& semaphore : semaphores)
		{
			mSemaphores.push_back(semaphore.first->GetHandle());
			mPipelineStages.push_back(semaphore.second);
		}
	}

	inline VulkanSemaphoreStageGroup::VulkanSemaphoreStageGroup(const std::initializer_list<std::pair<std::shared_ptr<VulkanSemaphore>, VkPipelineStageFlags>>& semaphores)
	{
		for (const auto& semaphore : semaphores)
		{
			mSemaphores.push_back(semaphore.first->GetHandle());
			mPipelineStages.push_back(semaphore.second);
		}
	}
}