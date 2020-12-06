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
		VkRenderPass mRenderPass{ nullptr };
		std::shared_ptr<VulkanDevice> mDevice{ nullptr };
	public:
		~VulkanRenderPass();
		VkRenderPass GetHandle() const { return mRenderPass; }
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return mDevice; }
		static std::shared_ptr<VulkanRenderPass> Create(const std::shared_ptr<VulkanDevice>& device, const VkRenderPassCreateInfo& createInfo);
	};
}

#endif