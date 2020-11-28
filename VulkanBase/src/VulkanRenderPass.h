#ifndef VULKAN_RENDER_PASS_H
#define VULKAN_RENDER_PASS_H

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"
#include "VulkanDevice.h"
#include <memory>

namespace MVK
{
	class VulkanRenderPass :public DeviceObjectBase
	{
	private:
		VkRenderPass m_render_pass{ nullptr };
		std::shared_ptr<VulkanDevice> m_device{ nullptr };
	public:
		~VulkanRenderPass();
		VkRenderPass GetHandle() const { return m_render_pass; }
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return m_device; }
		static std::shared_ptr<VulkanRenderPass> CreateRenderPass(const std::shared_ptr<VulkanDevice> device, const VkRenderPassCreateInfo& createInfo);
	};
}

#endif