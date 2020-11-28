#include "VulkanInstance.h"
#include "VulKanDebug.h"
#include <vector>
#include <GLFW/glfw3.h>
#include "Log.h"

namespace MVK
{
	std::shared_ptr<VulkanInstance> VulkanInstance::createInstance(const char* appName, bool enableValidation)
	{
		std::shared_ptr<VulkanInstance> instance = std::make_shared<VulkanInstance>();

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = appName;
		appInfo.pEngineName = appName;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.pNext = nullptr;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> instanceExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		if (enableValidation)
		{
			instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
		createInfo.ppEnabledExtensionNames = instanceExtensions.data();

		if (enableValidation)
		{
			uint32_t instanceLayerCount = 0;
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
			std::vector<VkLayerProperties> instanceLayer(instanceLayerCount);
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayer.data());

			bool validationLayerPresent = true;
			for (const char* layerName : kValidationLayers)
			{
				bool  layerFound = false;
				for (VkLayerProperties layer : instanceLayer)
				{
					if (strcmp(layer.layerName, layerName) == 0)
					{
						layerFound = true;
						break;
					}
				}
				if (!layerFound)
				{
					validationLayerPresent = false;
					break;
				}
			}
			if (validationLayerPresent)
			{
				createInfo.enabledLayerCount = static_cast<uint32_t>(sizeof(kValidationLayers) / sizeof(const char*));
				createInfo.ppEnabledLayerNames = kValidationLayers;
			}
		}
		if (vkCreateInstance(&createInfo, nullptr, &instance->m_Instance) != VK_SUCCESS)
		{
			CORE_ERROR("failed to create instance");
			exit(EXIT_FAILURE);
		}

		if (enableValidation)
		{
			MVK::DEBUG::setUpDebug(instance->GetHandle());
		}

		return instance;
	}

	VulkanInstance::~VulkanInstance()
	{

		MVK::DEBUG::freeDebug(m_Instance);
		vkDestroyInstance(m_Instance, nullptr);
	}
}
