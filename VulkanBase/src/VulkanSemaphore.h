#ifndef VULKAN_SEMAPHORE_H
#define VULKAN_SEMAPHORE_Hs

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"
#include <memory>

namespace MVK
{
	class VulkanSemaphore : public DeviceObjectBase
	{
	private:
		VkSemaphore mSemaphore{ nullptr };
		std::shared_ptr<VulkanDevice> mDevice;
	public:
		static std::shared_ptr<VulkanSemaphore> Create(const std::shared_ptr<VulkanDevice>& device);
		~VulkanSemaphore();
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() { return mDevice; }
		VkSemaphore GetHandle()const { return mSemaphore; }
	};

	class VulkanSemaphoreGroup
	{
	private:
		std::vector<VkSemaphore> mSemaphores;
	public:
		VulkanSemaphoreGroup() = default;
		explicit VulkanSemaphoreGroup(const std::vector<std::shared_ptr<VulkanSemaphore>>& semaphores);
		VulkanSemaphoreGroup(const std::initializer_list<std::shared_ptr<VulkanSemaphore>>& semaphores);
		uint32_t GetCount() const { return mSemaphores.size(); }
		const VkSemaphore* GetSemaphorePtr() const { return mSemaphores.data(); }
	};

	class VulkanSemaphoreStageGroup
	{
	private:
		std::vector<VkSemaphore> mSemaphores;
		std::vector<VkPipelineStageFlags> mPipelineStages;
	public:
		VulkanSemaphoreStageGroup() = default;
		VulkanSemaphoreStageGroup(const std::vector<std::pair<std::shared_ptr<VulkanSemaphore>, VkPipelineStageFlags>>& semaphores);
		VulkanSemaphoreStageGroup(const std::initializer_list<std::pair<std::shared_ptr<VulkanSemaphore>, VkPipelineStageFlags>>& semaphores);
		uint32_t GetCount() const { return mSemaphores.size(); }
		const VkSemaphore* GetSemaphorePtr() const { return mSemaphores.data(); }
		const VkPipelineStageFlags* GetPipelineStageFlags() const { return mPipelineStages.data(); }
	};
}

#endif
