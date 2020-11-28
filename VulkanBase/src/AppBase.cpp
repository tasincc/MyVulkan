#include "AppBase.h"
#include "Log.h"

AppBase::AppBase()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	m_Window = glfwCreateWindow(1920, 1080, m_AppName.c_str(), nullptr, nullptr);
	if (m_Window == nullptr)
	{
		CORE_ERROR("failed to create window");
		exit(EXIT_FAILURE);
	}
}

AppBase::~AppBase()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void AppBase::Initialize()
{
	//to initalize Instance、window、surface、surfaceImage、logicalDevice、deviceQueue

	m_Instance = MVK::VulkanInstance::createInstance(m_AppName.c_str(), true);
	m_surface = MVK::Surface::crateSurface(m_Instance, m_Window);

	std::vector < std::shared_ptr<MVK::VulkanPhysicalDevice>> devices = MVK::VulkanPhysicalDevice::GetAllPhysicalDevice(m_Instance);
	if (devices.empty())
	{
		CORE_ERROR("failed to get a physicalDevice");
		exit(EXIT_FAILURE);
	}
	m_VulkanPhysicalDevice = devices[0];
	m_VulkanDevice = MVK::VulkanDevice::createLogicDevice(m_VulkanPhysicalDevice, m_surface, m_EnabledFeatures, m_EnableDeviceExtensions, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT);

	m_swapChain = MVK::VulkanSwapChain::createSwapChain(m_Window, m_VulkanPhysicalDevice, m_surface, m_VulkanDevice);
	m_swapchain_images = MVK::SwapChainImage::createSwapChainImage(m_swapChain);
	m_swapchain_image_views.resize(m_swapchain_images.size());
	for (uint32_t i = 0; i < static_cast<uint32_t>(m_swapchain_images.size()); i++)
	{
		m_swapchain_image_views[i] = MVK::ImageView::createImageView(m_swapchain_images[i]);
	}
}
