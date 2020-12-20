#ifndef VULKAN_FRAME_BUFFER
#define VULKAN_FRAME_BUFFER

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"
#include "VulkanRenderPass.h"
#include "VulkanImageView.h"
#include <memory>

namespace MVK
{
	class VulkanFramebuffer :public DeviceObjectBase
	{
	private:
		VkFramebuffer mFramebuffer{ nullptr };
		std::shared_ptr<VulkanRenderPass> mRenderpass;
		std::vector<std::shared_ptr<VulkanImageView>> mImageViews;
		VkExtent2D mExtent;
		uint32_t mLayer;
	public:
		static std::shared_ptr<VulkanFramebuffer> Create(const std::shared_ptr<VulkanRenderPass>& renderPass, const std::vector<std::shared_ptr<VulkanImageView>>& imageViews, const VkExtent2D& extent, uint32_t layers);
		~VulkanFramebuffer();
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return mRenderpass->GetDevicePtr(); }
		VkFramebuffer GetHandle() const { return mFramebuffer; }
		VkExtent2D GetExtent() const { return mExtent; }
	};
}

#endif