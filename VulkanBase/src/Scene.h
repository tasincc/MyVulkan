#ifndef SCENE_H
#define SCENE_H

#include "VulkanQueue.h"
#include "VulkanBuffer.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanImage.h"
#include "VulkanImageView.h"
#include "VulkanImageSampler.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSet.h"

#include "glm/glm.hpp"

#include <memory>
#include <string>
#include <vector>

class Scene
{
private:
	struct Mesh;

	struct PushConstant
	{
		uint32_t mTextureId1;
		uint32_t mAlbedo;
	};

	struct DrawCmd
	{
		PushConstant mPushConstant;
		uint32_t mIndexCount;
		uint32_t mFirstIndex;
	};

	struct Texture
	{
		std::shared_ptr<MVK::VulkanImage> mImage;
		std::shared_ptr<MVK::VulkanImageView> mImageView;
		std::shared_ptr<MVK::VulkanImageSampler> mSampler;
	};

	std::shared_ptr<MVK::VulkanBuffer> mVertexBuffer;
	std::shared_ptr<MVK::VulkanBuffer> mIndexBuffer;
	std::vector<Texture> mTextures;
	std::vector<DrawCmd> mDrawCmd;

	std::shared_ptr<MVK::VulkanDescriptorSetLayout> mDescriptorSetLayout;
	std::shared_ptr<MVK::VulkanDescriptorPool> mDescriptorPool;
	std::shared_ptr<MVK::VulkanDescriptorSet> mDescriptorSet;


	static std::string get_base_dir(const char* filename);
	static bool load_meshs(const char* filename, const char* dir, std::vector<Mesh>* meshs, std::vector<std::string>* textureFileNames);
	bool load_buffer_and_draw_cmd(const std::shared_ptr<MVK::VulkanQueue> graphicQueue, const std::vector<Mesh>& meshs);
	void load_texture(const std::shared_ptr<MVK::VulkanQueue> graphicQueue, const std::vector<std::string>& textureFileNames);
	void create_descriptor(std::shared_ptr<MVK::VulkanDevice> device);
public:
	bool Initialize(std::shared_ptr<MVK::VulkanQueue> queuem, const char* filename);
	const std::shared_ptr<MVK::VulkanDescriptorSetLayout>& GetDescriptorSetLayout() const { return mDescriptorSetLayout; }
	const std::shared_ptr<MVK::VulkanDescriptorSet>& GetDescriptorSet() const { return mDescriptorSet; }
};

#endif