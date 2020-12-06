#include "DeferredShade.h"

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
	mPresentQueue = MVK::VulkanPresentQueue::Create(mVulkanDevice);

	scene.Initialize(mGraphicQueue, "C://Users//41132//Desktop//Opengl//Learn//resources//objects//sponza//sponza.obj");

	//renderPass
	craetRenderPass();

	//pipeline
	createPipeline();

	//
	mFrameBuffer.resize(mSwapchainImageViews.size());
	for (uint32_t i = 0; i < mSwapchainImageViews.size(); i++)
	{
		mFrameBuffer[i] = MVK::VulkanFramebuffer::Create(mRenderPass, { mSwapchainImageViews[i] ,mDepthView }, mSwapChain->GetExtent(), 1);
	}
}


void Application::craetRenderPass()
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
	VkPipelineInputAssemblyStateCreateInfo input_assembly_state{};
	input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly_state.primitiveRestartEnable = VK_FALSE;

	std::shared_ptr<MVK::VulkanShaderModule> vertexShader = MVK::VulkanShaderModule::Create(mVulkanDevice, "../shader/deferred/deferred.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
	std::shared_ptr<MVK::VulkanShaderModule> fragShader = MVK::VulkanShaderModule::Create(mVulkanDevice, "../shader/deferred/deferred.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
	std::array<VkPipelineShaderStageCreateInfo, 2> shaderStageArray = { vertexShader->GetShaderStageCreateInfo("main"), fragShader->GetShaderStageCreateInfo("main") };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	auto attributeDescriptions = scene.GetVertexInputAttributeDescription();
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	auto bindingDescriptions = scene.GetVertexInputBindingDescription();
	vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyCI{};
	inputAssemblyCI.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyCI.primitiveRestartEnable = VK_FALSE;
	inputAssemblyCI.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

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

	VkPipelineRasterizationStateCreateInfo rasterizationCI{};
	rasterizationCI.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationCI.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationCI.lineWidth = 1.0f;
	rasterizationCI.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizationCI.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizationCI.depthClampEnable = VK_FALSE;
	rasterizationCI.depthBiasClamp = VK_FALSE;
	rasterizationCI.rasterizerDiscardEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampleStateCI{};
	multisampleStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleStateCI.sampleShadingEnable = VK_FALSE;
	multisampleStateCI.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineDepthStencilStateCreateInfo depthStencilCI{};
	depthStencilCI.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilCI.depthTestEnable = VK_TRUE;
	depthStencilCI.depthWriteEnable = VK_TRUE;
	depthStencilCI.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencilCI.depthBoundsTestEnable = VK_FALSE;
	depthStencilCI.stencilTestEnable = VK_FALSE;

	//*************************************************************************************
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo colorBlendCI{};
	colorBlendCI.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendCI.attachmentCount = 1;
	colorBlendCI.pAttachments = &colorBlendAttachment;
	//*************************************************************************************

	std::shared_ptr<MVK::VulkanPipelineLayout> pipelienLayout = MVK::VulkanPipelineLayout::Create(mVulkanDevice, { scene.GetDescriptorSetLayout() });

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