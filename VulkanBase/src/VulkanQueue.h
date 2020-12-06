#ifndef VULKANQUEUE_H
#define VULKANQUEUE_H

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"
#include <memory>

namespace MVK
{
	class VulkanQueue :public DeviceObjectBase
	{
	protected:
		VkQueue mQueue{ nullptr };
		uint32_t mFamilyIndex;
		std::shared_ptr<VulkanDevice> mDevice;
	public:
		static std::shared_ptr<VulkanQueue> Create(const std::shared_ptr<VulkanDevice>& device, VkQueueFlags m_requestQueue);
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return mDevice; }
		VkQueue GetHandle() const { return mQueue; }
		uint32_t GetFamilyIndex() const { return mFamilyIndex; }
	};


	class VulkanPresentQueue : public VulkanQueue
	{
	private:
		std::shared_ptr<Surface> m_surface;
	public:
		const std::shared_ptr<Surface>& GetSufacePtr() { return m_surface; }
		static std::shared_ptr<VulkanPresentQueue> Create(const std::shared_ptr<VulkanDevice>& device);
	};
}

#endif // !VULKANQUEUE_H