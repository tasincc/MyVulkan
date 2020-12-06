#ifndef APPLICATION_H
#define APPLICATION_H

#include "AppBase.h"
#include "Log.h"

#include "VulkanImage.h"
#include "VulkanImageView.h"
#include "VulkanPipeline.h"
#include "VulkanFrameBuffer.h"

#include "Config.h"
#include "Scene.h"

#include <memory>

class Application : public AppBase
{
private:
	std::shared_ptr<MVK::VulkanCommandPool> mCommandPool;
	std::shared_ptr<MVK::VulkanRenderPass> mRenderPass;
	std::shared_ptr<MVK::VulkanGraphicPipeline> mGraphicPipeline;
	std::vector<std::shared_ptr<MVK::VulkanFramebuffer>> mFrameBuffer;

	std::shared_ptr<MVK::VulkanQueue> mGraphicQueue{ nullptr };
	std::shared_ptr<MVK::VulkanPresentQueue> mPresentQueue{ nullptr };
	std::shared_ptr<MVK::VulkanQueue> mTransferQueue{ nullptr };

	Scene scene;
protected:
	void craetRenderPass();
	void createPipeline();
public:
	Application();
	~Application();
	void Initialize() override;
};

#endif
