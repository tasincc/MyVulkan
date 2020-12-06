#ifndef SWAP_CHAIN_IMAGE_H
#define SWAP_CHAIN_IMAGE_H

#include "VulkanImageBase.h"
#include "VulkanSwapChain.h"
#include <memory>
namespace MVK
{
	class SwapChainImage : public ImageBase
	{
	private:
		std::shared_ptr<VulkanSwapChain> mSwapchain;
	public:
		static std::vector<std::shared_ptr<SwapChainImage>> Create(const std::shared_ptr<VulkanSwapChain>& swapChain);
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const override { return mSwapchain->GetDevicePtr(); };
	};
}

#endif
