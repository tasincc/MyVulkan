#ifndef VULKANQUEUE_H
#define VULKANQUEUE_H

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"
#include <memory>

namespace MVK
{
	class VulkanQueue :public DeviceObjectBase
	{
	private:
		VkQueue m_queue{ nullptr };
		uint32_t m_familyIndex;
		std::shared_ptr<VulkanDevice> m_device;
	public:
		~VulkanQueue();
		static std::shared_ptr<VulkanQueue> createVulkanQueue(const std::shared_ptr<VulkanDevice> device, uint32_t familyIndex);
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return m_device; }
		VkQueue GetHandle() const { return m_queue; }
		uint32_t GetFamilyIndex() { return m_familyIndex; }
	};
}

#endif // !VULKANQUEUE_H