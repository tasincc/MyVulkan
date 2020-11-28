#ifndef VULKAN_BUFFER_H
#define VULKAN_BUFFER_H

#include "vulkan/vulkan.h"
#include "VulkanBufferBase.h"

namespace MVK
{
	class VulkanBuffer :public VulkanBufferBase
	{
	private:
		std::shared_ptr<VulkanDevice> m_device;
		VkDeviceMemory m_memory;
	public:
		~VulkanBuffer();
		void* Map() const;
		void Unmap() const;
		template<typename T>
		void Update(const T& data, uint32_t byte_offset) const;
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return m_device; }
		static std::shared_ptr<VulkanBuffer> CreateBuffer(const std::shared_ptr<VulkanDevice>& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		static std::shared_ptr<VulkanBuffer> CreateStaging(const std::shared_ptr<VulkanDevice>& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	};
}

#endif // VULKAN_BUFFER_H
