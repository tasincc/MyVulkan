#ifndef APPLICATION_H
#define APPLICATION_H

#include "AppBase.h"
#include "Log.h"

#include "VulkanImage.h"
#include "VulkanImageView.h"
#include "VulkanPipeline.h"

#include "Config.h"
#include "Scene.h"

#include <memory>

class Application : public AppBase
{
private:
	std::shared_ptr<MVK::VulkanCommandPool> m_command_pool;
	std::shared_ptr<MVK::VulkanRenderPass> m_render_pass;

	std::shared_ptr<MVK::VulkanDescriptorSetLayout> m_descriptor_set_layout;
	std::shared_ptr<MVK::VulkanPipelineLayout> m_pipeline_layout;
	std::shared_ptr<MVK::VulkanGraphicPipeline> m_graphic_pipeline;

	std::shared_ptr<MVK::VulkanQueue> mGraphicQueue{ nullptr };
	std::shared_ptr<MVK::VulkanPresentQueue> mPresentQueue{ nullptr };
	std::shared_ptr<MVK::VulkanQueue> mTransferQueue{ nullptr };

	Scene scene;
protected:
	void craetRenderPass();
	//void prepare();
	void createPipeline();
public:
	Application();
	~Application();
	void Initialize() override;
};

#endif
