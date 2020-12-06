#include "VulkanBuffer.h"
#include "Log.h"

namespace MVK
{
	std::shared_ptr<VulkanBuffer> VulkanBuffer::CreateBuffer(const std::shared_ptr<VulkanDevice>& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
	{
		std::shared_ptr<VulkanBuffer> ret = std::make_shared<VulkanBuffer>();
		ret->m_device = device;
		ret->mSize = size;

		VkBufferCreateInfo buffInfo{};
		buffInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffInfo.size = size;
		buffInfo.usage = usage;
		buffInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //default

		VkResult result = vkCreateBuffer(device->GetHandle(), &buffInfo, nullptr, &ret->mBuffer);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create buffer");
			exit(EXIT_FAILURE);
		}

		VkMemoryRequirements memRequire;
		vkGetBufferMemoryRequirements(device->GetHandle(), ret->mBuffer, &memRequire);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequire.size;
		allocInfo.memoryTypeIndex = device->GetPhysicalDevicePtr()->FindMemoryType(memRequire.memoryTypeBits, properties);

		result = vkAllocateMemory(device->GetHandle(), &allocInfo, nullptr, &ret->m_memory);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to allocate memory");
			exit(EXIT_FAILURE);
		}

		vkBindBufferMemory(device->GetHandle(), ret->mBuffer, ret->m_memory, 0);

		return ret;
	}

	std::shared_ptr<VulkanBuffer> VulkanBuffer::CreateStaging(const std::shared_ptr<VulkanDevice>& device, VkDeviceSize size)
	{
		return CreateBuffer(device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}

	VulkanBuffer::~VulkanBuffer()
	{
		if (mBuffer)
		{
			vkDestroyBuffer(m_device->GetHandle(), mBuffer, nullptr);
			vkFreeMemory(m_device->GetHandle(), m_memory, nullptr);
		}
	}

	void* VulkanBuffer::Map() const
	{
		void* data;
		vkMapMemory(m_device->GetHandle(), m_memory, 0, mSize, 0, &data);
		return data;
	}

	void VulkanBuffer::Unmap() const
	{
		vkUnmapMemory(m_device->GetHandle(), m_memory);
	}

	//template<typename T>
	//void VulkanBuffer::Update(const T* begin, const T* end, uint32_t byte_offset) const
	//{
	//	//memcpy(Map(), begin, (size_t)begin);
	//	std::copy(begin, end, (T*)((uint8_t*)Map() + byte_offset));
	//	Unmap();
	//}
}