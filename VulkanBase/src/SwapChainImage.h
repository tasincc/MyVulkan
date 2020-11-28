#ifndef SWAP_CHAIN_IMAGE_H
#define SWAP_CHAIN_IMAGE_H

#include "ImageBase.h"
#include "VulkanSwapChain.h"
#include <memory>
namespace MVK
{
	class SwapChainImage : public ImageBase
	{
	private:
		std::shared_ptr<VulkanSwapChain> m_swapchain;
	public:
		static std::vector<std::shared_ptr<SwapChainImage>> createSwapChainImage(const std::shared_ptr<VulkanSwapChain> swapChain);
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const override { return m_swapchain->GetDevicePtr(); };
	};
}

#endif
