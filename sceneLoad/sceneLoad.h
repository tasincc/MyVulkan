#ifndef APPLICATION_H
#define APPLICATION_H

#include "AppBase.h"
#include "Log.h"

#include "VulkanImage.h"
#include "VulkanImageView.h"
#include "VulkanPipeline.h"
#include "VulkanFrameBuffer.h"
#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"

#include "Config.h"
#include "Scene.h"

#include <memory>

class Application : public AppBase
{
private:
	struct Ubo
	{
		glm::mat4 model;
		glm::mat4 projection;
		glm::mat4 view;
	};

	std::shared_ptr<MVK::VulkanCommandPool> mCommandPool;
	std::vector<std::shared_ptr<MVK::VulkanCommandBuffer>> mDrwaCmdBuffers;
	std::shared_ptr<MVK::VulkanRenderPass> mRenderPass;
	std::shared_ptr<MVK::VulkanGraphicPipeline> mGraphicPipeline;
	std::vector<std::shared_ptr<MVK::VulkanFramebuffer>> mFrameBuffer;

	std::shared_ptr<MVK::VulkanQueue> mGraphicQueue{ nullptr };
	std::shared_ptr<MVK::VulkanQueue> mTransferQueue{ nullptr };

	Ubo mUbo;
	std::vector<std::shared_ptr<MVK::VulkanBuffer>> mUniformBuffers;

	std::shared_ptr<MVK::VulkanDescriptorPool> mDescriptorPool;
	std::shared_ptr<MVK::VulkanDescriptorSetLayout> mDescriptorSetLayout;
	std::vector<std::shared_ptr<MVK::VulkanDescriptorSet>> mDescriptorSets;

	Scene mScene;
protected:
	void createRenderPass();
	void createPipeline();
	void createCommandBuffers();
	void commitCommandBuffer();
	void updateUniformBuffer(uint32_t index);
	void setDescriptorSet();
public:
	Application();
	~Application();
	void Initialize() override;
};

#endif
