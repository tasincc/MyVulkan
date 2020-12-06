#include "AppBase.h"
#include "Log.h"
#include "Config.h"

AppBase::AppBase()
{
}

AppBase::~AppBase()
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void AppBase::Initialize()
{
	//to initalize Instance¡¢window¡¢surface
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	mWindow = glfwCreateWindow(WIDTH, HEIGHT, m_AppName.c_str(), nullptr, nullptr);
	if (mWindow == nullptr)
	{
		CORE_ERROR("failed to create window");
		exit(EXIT_FAILURE);
	}

	mInstance = MVK::VulkanInstance::Create(m_AppName.c_str(), true);
	mSurface = MVK::Surface::Create(mInstance, mWindow);


	std::vector < std::shared_ptr<MVK::VulkanPhysicalDevice>> devices = MVK::VulkanPhysicalDevice::GetAllPhysicalDevice(mInstance);
	if (devices.empty())
	{
		CORE_ERROR("failed to get a physicalDevice");
		exit(EXIT_FAILURE);
	}

	mVulkanPhysicalDevice = devices[0];
	mVulkanDevice = MVK::VulkanDevice::Create(mVulkanPhysicalDevice, mSurface, mEnabledFeatures, mEnableDeviceExtensions, mRequestQueue);

	mSwapChain = MVK::VulkanSwapChain::Create(mWindow, mVulkanPhysicalDevice, mSurface, mVulkanDevice);
	mSwapchainImages = MVK::SwapChainImage::Create(mSwapChain);
	mSwapchainImageViews.resize(mSwapchainImages.size());
	for (uint32_t i = 0; i < static_cast<uint32_t>(mSwapchainImages.size()); i++)
	{
		mSwapchainImageViews[i] = MVK::VulkanImageView::Create(mSwapchainImages[i]);
	}

	mDepth = MVK::VulkanImage::CreateTexture2D(mVulkanDevice, { WIDTH ,HEIGHT }, 1, mVulkanPhysicalDevice->FindDepthFormat(), VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	mDepthView = MVK::VulkanImageView::Create(mDepth, VK_IMAGE_VIEW_TYPE_2D, mDepth->GetFormat(), VK_IMAGE_ASPECT_DEPTH_BIT);
}

void AppBase::DrawFrame()
{
	while (!glfwWindowShouldClose(mWindow))
	{
		glfwPollEvents();
	}
	vkDeviceWaitIdle(mVulkanDevice->GetHandle());
}
