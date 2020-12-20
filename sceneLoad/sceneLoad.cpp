#include "sceneLoad.h"
#include "VulkanSemaphore.h"

Application::Application()
{
	m_AppName = "Deferred";
	m_Validation = true;
	mRequestQueue = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT;
}

Application::~Application()
{

}

void Application::Initialize()
{
	AppBase::Initialize();

	mGraphicQueue = MVK::VulkanQueue::Create(mVulkanDevice, VK_QUEUE_GRAPHICS_BIT);
	mTransferQueue = MVK::VulkanQueue::Create(mVulkanDevice, VK_QUEUE_TRANSFER_BIT);

	mCommandPool = MVK::VulkanCommandPool::Create(mGraphicQueue, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	//mScene.Initialize(mGraphicQueue, "C:/Users/41132/Desktop/San_Miguel/san-miguel-low-poly.obj");
	//mScene.Initialize(mGraphicQueue, "C://Users//41132//Desktop//Opengl//Learn//resources//objects//sponza//sponza.obj");
	mScene.Initialize(mGraphicQueue, "C://Users//41132//Desktop//Opengl//Learn//resources//objects//rock//rock.obj");

	//create uniform buffer
	mUniformBuffers.resize(mSwapchainImageViews.size());
	for (uint32_t i = 0; i < mSwapchainImageViews.size(); i++)
	{
		mUniformBuffers[i] = MVK::VulkanBuffer::CreateBuffer(mVulkanDevice, sizeof(Ubo), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		updateUniformBuffer(i);
	}

	//create Ubo descriptor set
	setDescriptorSet();

	//renderPass
	createRenderPass();

	//pipeline
	createPipeline();

	//frameBuffer
	mFrameBuffer.resize(mSwapchainImageViews.size());
	for (uint32_t i = 0; i < mSwapchainImageViews.size(); i++)
	{
		mFrameBuffer[i] = MVK::VulkanFramebuffer::Create(mRenderPass, { mSwapchainImageViews[i] ,mDepthView }, mSwapChain->GetExtent(), 1);
	}

	//create Command buffer
	createCommandBuffers();
}

void Application::commitCommandBuffer()
{
	updateUniformBuffer(mCurrentFrame);
	mInFlightFences[mCurrentFrame]->Reset();
	VkResult result = mDrwaCmdBuffers[mImageIndex]->Submit({ {mPresentSemaphores[mCurrentFrame],VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT} }, { mRenderSemaphores[mCurrentFrame] }, mInFlightFences[mCurrentFrame]);
	if (result != VK_SUCCESS)
	{
		ERROR("submit error");
	}
}

void Application::updateUniformBuffer(uint32_t index)
{
	mUbo.model = glm::mat4(1.0f);
	//mUbo.model = glm::translate(mUbo.model, glm::vec3(0.0f, 0.0f, 0.5f));
	mUbo.projection = mCamera.GetPerspective();
	mUbo.view = mCamera.GetViewMatrix();
	mUniformBuffers[index]->UpdateData(mUbo);

	//memcpy(mUniformBuffers[index]->Map(), &mUbo, sizeof(Ubo));
	//mUniformBuffers[index]->Unmap();
}

void Application::createRenderPass()
{
	std::array<VkAttachmentDescription, 2> attachments = {};
	attachments[0].format = mSwapChain->GetImageFormat();
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	attachments[1].format = mDepth->GetFormat();
	attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorReference{};
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthReference{};
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subPass{};
	subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subPass.colorAttachmentCount = 1;
	subPass.pColorAttachments = &colorReference;
	subPass.pDepthStencilAttachment = &depthReference;
	subPass.inputAttachmentCount = 0;
	subPass.pInputAttachments = nullptr;
	subPass.preserveAttachmentCount = 0;
	subPass.pPreserveAttachments = nullptr;
	subPass.pResolveAttachments = nullptr;

	std::array<VkSubpassDependency, 2> dependencies;
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkRenderPassCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	createInfo.pAttachments = attachments.data();
	createInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	createInfo.pDependencies = dependencies.data();
	createInfo.subpassCount = 1;
	createInfo.pSubpasses = &subPass;

	mRenderPass = MVK::VulkanRenderPass::Create(mVulkanDevice, createInfo);
}

void Application::createPipeline()
{
	std::shared_ptr<MVK::VulkanShaderModule> vertexShader = MVK::VulkanShaderModule::Create(mVulkanDevice, "../shader/deferred/deferred.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
	std::shared_ptr<MVK::VulkanShaderModule> fragShader = MVK::VulkanShaderModule::Create(mVulkanDevice, "../shader/deferred/deferred.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
	std::array<VkPipelineShaderStageCreateInfo, 2> shaderStageArray = { vertexShader->GetShaderStageCreateInfo("main"), fragShader->GetShaderStageCreateInfo("main") };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	auto attributeDescriptions = mScene.GetVertexInputAttributeDescription();
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	auto bindingDescriptions = mScene.GetVertexInputBindingDescription();
	vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyCI = MVK::VulkanGraphicPipeline::GetPipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);

	//**************************************************************************
	VkViewport viewPort{};
	viewPort.x = 0.0f;
	viewPort.y = 0.0f;
	viewPort.width = mSwapChain->GetExtent().width;
	viewPort.height = mSwapChain->GetExtent().height;
	viewPort.minDepth = 0.0f;
	viewPort.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0,0 };
	scissor.extent = mSwapChain->GetExtent();

	VkPipelineViewportStateCreateInfo pipelineViewPortCI{};
	pipelineViewPortCI.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	pipelineViewPortCI.viewportCount = 1;
	pipelineViewPortCI.pViewports = &viewPort;
	pipelineViewPortCI.scissorCount = 1;
	pipelineViewPortCI.pScissors = &scissor;

	//**************************************************************************

	VkPipelineRasterizationStateCreateInfo rasterizationCI = MVK::VulkanGraphicPipeline::GetPipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);

	VkPipelineMultisampleStateCreateInfo multisampleStateCI{};
	multisampleStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleStateCI.sampleShadingEnable = VK_FALSE;
	multisampleStateCI.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineDepthStencilStateCreateInfo depthStencilCI = MVK::VulkanGraphicPipeline::GetPipelineDepthStencilStateCreateInfo(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS);

	//*************************************************************************************
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo colorBlendCI{};
	colorBlendCI.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendCI.attachmentCount = 1;
	colorBlendCI.pAttachments = &colorBlendAttachment;
	//*************************************************************************************

	std::shared_ptr<MVK::VulkanPipelineLayout> pipelienLayout = MVK::VulkanPipelineLayout::Create(mVulkanDevice, { mDescriptorSetLayout, mScene.GetDescriptorSetLayout() }, {});

	VkGraphicsPipelineCreateInfo pipelineCI{};
	pipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCI.renderPass = mRenderPass->GetHandle();
	pipelineCI.subpass = 0;
	pipelineCI.pStages = shaderStageArray.data();
	pipelineCI.stageCount = static_cast<uint32_t>(shaderStageArray.size());
	pipelineCI.pVertexInputState = &vertexInputInfo;
	pipelineCI.pInputAssemblyState = &inputAssemblyCI;
	pipelineCI.pViewportState = &pipelineViewPortCI;
	pipelineCI.pRasterizationState = &rasterizationCI;
	pipelineCI.pMultisampleState = &multisampleStateCI;
	pipelineCI.pDepthStencilState = &depthStencilCI;
	pipelineCI.pColorBlendState = &colorBlendCI;
	pipelineCI.layout = pipelienLayout->GetHandle();
	pipelineCI.basePipelineHandle = VK_NULL_HANDLE;

	mGraphicPipeline = MVK::VulkanGraphicPipeline::Create(pipelienLayout, mRenderPass, pipelineCI);
}

void Application::createCommandBuffers()
{
	mDrwaCmdBuffers.resize(mSwapchainImages.size());
	for (uint32_t i = 0; i < static_cast<uint32_t>(mDrwaCmdBuffers.size()); i++)
	{
		mDrwaCmdBuffers[i] = MVK::VulkanCommandBuffer::Create(mCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		mDrwaCmdBuffers[i]->Begin();
		std::vector<VkClearValue> clearValues(2);
		clearValues[0].color = { 0.0f,0.0f,0.0f,1.0f };
		clearValues[1].depthStencil = { 1.0f,0 };
		mDrwaCmdBuffers[i]->BeginRenderPass(mRenderPass, mFrameBuffer[i], clearValues, VK_SUBPASS_CONTENTS_INLINE);
		mDrwaCmdBuffers[i]->CmdBindPipeline(mGraphicPipeline, VK_PIPELINE_BIND_POINT_GRAPHICS);

		//VkViewport viewPort{};
		//viewPort.x = 0.0f;
		//viewPort.y = 0.0f;
		//viewPort.width = mSwapChain->GetExtent().width;
		//viewPort.height = mSwapChain->GetExtent().height;
		//viewPort.minDepth = 0.0f;
		//viewPort.maxDepth = 1.0f;
		//mDrwaCmdBuffers[i]->CmdSetViewport(viewPort);
		//VkRect2D scissor{};
		//scissor.offset = { 0,0 };
		//scissor.extent = mSwapChain->GetExtent();
		//mDrwaCmdBuffers[i]->CmdSetScissor(scissor);

		mDrwaCmdBuffers[i]->CmdBindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS, 0, mGraphicPipeline->GetPipelineLayout(), { mDescriptorSets[i] }, {});
		mScene.Draw(mDrwaCmdBuffers[i], mGraphicPipeline);
		mDrwaCmdBuffers[i]->EndRenderPass();
		mDrwaCmdBuffers[i]->End();
	}
}

void Application::setDescriptorSet()
{
	VkDescriptorSetLayoutBinding setLayoutBinding{};
	setLayoutBinding.binding = 0;
	setLayoutBinding.descriptorCount = 1;
	setLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	setLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	mDescriptorSetLayout = MVK::VulkanDescriptorSetLayout::CreaeteDescriptorSetLayout(mVulkanDevice, { setLayoutBinding });

	VkDescriptorPoolSize poolSize{};
	poolSize.descriptorCount = mSwapchainImages.size();
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	mDescriptorPool = MVK::VulkanDescriptorPool::Create(mVulkanDevice, mSwapchainImages.size(), { poolSize });

	mDescriptorSets.resize(mSwapchainImages.size());
	for (uint32_t i = 0; i < static_cast<uint32_t>(mDescriptorSets.size()); i++)
	{
		mDescriptorSets[i] = MVK::VulkanDescriptorSet::Create(mDescriptorPool, mDescriptorSetLayout);
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = mUniformBuffers[i]->GetHandle();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(Ubo);
		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.dstSet = mDescriptorSets[i]->GetHandle();
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.pBufferInfo = &bufferInfo;
		vkUpdateDescriptorSets(mVulkanDevice->GetHandle(), 1, &descriptorWrite, 0, nullptr);
	}
}