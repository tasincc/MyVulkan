#include "Scene.h"
#include "Log.h"
#include "VulkanCommandPool.h"
#include "VulkanCommandBuffer.h"
#include "VulkanFence.h"

#include "meshoptimizer.h"
#include "tinyObj/tiny_obj_loader.h"
#include "stb_image/stb_image.h"

#include <unordered_map>

struct Vertex
{
	glm::vec3 mPosition;
	glm::vec2 mTexCoord;
	//glm::vec3 mNormal;
	//glm::vec3 mTangent;
};

struct Scene::Mesh
{
	uint32_t mTextureBase;
	glm::vec3 mAlbedo;
	std::vector<Vertex> mVertices;
};

std::string Scene::get_base_dir(const char* filename)
{
	size_t len = strlen(filename);
	const char* s = filename + len;
	while (*(s - 1) != '/' && *(s - 1) != '\\' && s > filename)
	{
		s--;
	}
	return { filename,s };
}

bool Scene::load_meshs(const char* filename, const char* dir, std::vector <Mesh>* meshs, std::vector<std::string>* textureFileNames)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string errMsg;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &errMsg, filename, dir, true))
	{
		CORE_ERROR(errMsg);
	}

	meshs->clear();
	meshs->resize(materials.size());

	glm::vec3 pmin(FLT_MAX), pmax(-FLT_MAX);
	for (const auto& shape : shapes)
	{
		size_t indexOffset = 0, faceId = 0;
		for (const auto& num_face_vertice : shape.mesh.num_face_vertices)
		{
			uint32_t matId = shape.mesh.material_ids[faceId];
			for (size_t v = 0; v < num_face_vertice; ++v)
			{
				tinyobj::index_t index = shape.mesh.indices[indexOffset + v];
				meshs->at(matId).mVertices.emplace_back();
				Vertex& vertex = meshs->at(matId).mVertices.back();
				vertex.mPosition =
				{
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};
				if (index.texcoord_index != -1)
				{
					vertex.mTexCoord =
					{
						attrib.texcoords[2 * index.texcoord_index + 0],
						1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
					};
				}

				pmin = glm::min(pmin, vertex.mPosition);
				pmax = glm::max(pmax, vertex.mPosition);
			}
			indexOffset += num_face_vertice;
			faceId++;
		}
	}

	//normalize to [-1,1]
	glm::vec3 entent3 = pmax - pmin;
	glm::vec3 center = (pmax + pmin) * 0.5f;
	float extent = glm::max(entent3.x, glm::max(entent3.y, entent3.z)) * 0.5f;
	float divide = 1 / extent;
	for (auto& mesh : *meshs)
	{
		for (auto& vertex : mesh.mVertices)
		{
			vertex.mPosition = (vertex.mPosition - center) * divide;
		}
	}

	//texture id
	std::unordered_map<std::string, uint32_t > textureNameMap;
	for (uint32_t i = 0; i < meshs->size(); i++)
	{
		Mesh& mesh = (*meshs)[i];
		mesh.mAlbedo =
		{
			materials[i].diffuse[0],
			materials[i].diffuse[1],
			materials[i].diffuse[2],
		};
		const std::string& texture_name = materials[i].diffuse_texname;

		if (!texture_name.empty() && textureNameMap.find(texture_name) == textureNameMap.end())
		{
			uint32_t index = textureNameMap.size();
			textureNameMap[texture_name] = index;
		}
		else if (texture_name.empty())
		{
			mesh.mTextureBase = UINT32_MAX;
		}
	}

	for (uint32_t i = 0; i < meshs->size(); i++)
	{
		Mesh& mesh = (*meshs)[i];

		const std::string& texture_name = materials[i].diffuse_texname;
		if (!texture_name.empty() && textureNameMap.find(texture_name) != textureNameMap.end())
		{
			mesh.mTextureBase = textureNameMap[texture_name];
		}
	}

	//textureName
	textureFileNames->resize(textureNameMap.size());
	for (const auto& textureName : textureNameMap)
	{
		(*textureFileNames)[textureName.second] = dir + textureName.first;
	}

	//empty
	uint32_t not_empty_count = meshs->size();
	for (const auto& mesh : *meshs)
	{
		not_empty_count -= mesh.mVertices.empty();
	}
	std::sort(meshs->begin(), meshs->end(), [](const Mesh& l, const Mesh& r)
		{
			return l.mVertices.size() > r.mVertices.size();
		});
	meshs->resize(not_empty_count);

	return true;
}

void Scene::load_texture(const std::shared_ptr<MVK::VulkanQueue> graphicQueue, const std::vector<std::string>& textureFileNames)
{
	const std::shared_ptr<MVK::VulkanDevice> device = graphicQueue->GetDevicePtr();
	mTextures.clear();
	mTextures.resize(textureFileNames.size());


	std::shared_ptr<MVK::VulkanCommandPool> commandPool = MVK::VulkanCommandPool::Create(graphicQueue, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	for (uint32_t i = 0; i < textureFileNames.size(); i++)
	{
		//Load image
		int width, height, channels;
		stbi_uc* data = stbi_load(textureFileNames[i].c_str(), &width, &height, &channels, 4);
		if (data == nullptr)
		{
			CORE_ERROR("failed to load texture{}", textureFileNames[i]);
			continue;;
		}

		//Staging buffer
		VkDeviceSize dataSize = width * height * 4;
		VkExtent2D extent = { (uint32_t)width,(uint32_t)height };
		std::shared_ptr<MVK::VulkanBuffer> stagingBuffer = MVK::VulkanBuffer::CreateStaging(device, dataSize);
		stagingBuffer->UpdateData(data, data + dataSize);
		stbi_image_free(data);

		//Create Image¡¢ImageView¡¢ImageSampler
		Texture& texture = mTextures[i];
		texture.mImage = MVK::VulkanImage::CreateTexture2D(device, extent, MVK::VulkanImage::QueryMipLevel(extent),
			VK_FORMAT_R8G8B8A8_SRGB,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		texture.mImageView = MVK::VulkanImageView::Create(texture.mImage, VK_IMAGE_VIEW_TYPE_2D);
		texture.mSampler = MVK::VulkanImageSampler::Create(device, 
			VK_FILTER_LINEAR, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_LINEAR, 
			VK_SAMPLER_ADDRESS_MODE_REPEAT, false, 0, texture.mImage->GetMipLevels());

		//Copy buffer to image
		std::shared_ptr<MVK::VulkanFence> fence = MVK::VulkanFence::Create(device);
		std::shared_ptr<MVK::VulkanCommandBuffer> commanBuffer = MVK::VulkanCommandBuffer::Create(commandPool);
		commanBuffer->Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageExtent = { (uint32_t)extent.width,(uint32_t)extent.height,1 };
		region.imageOffset = { 0,0,0 };
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageSubresource.mipLevel = 0;

		//Transition image layout
		commanBuffer->CmdPipelineBarrier(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, {}, {},
			texture.mImage->GetImageMemoryBarrirs({ region.imageSubresource }, 0,
				VK_ACCESS_TRANSFER_WRITE_BIT,
				VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL));
		commanBuffer->CmdCopyBuffer(stagingBuffer, texture.mImage, region, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		//commanBuffer->CmdPipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, {}, {},
		//	texture.mImage->GetImageMemoryBarrirs({ region.imageSubresource }, VK_ACCESS_TRANSFER_WRITE_BIT,
		//		VK_ACCESS_TRANSFER_READ_BIT,
		//		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL));


		commanBuffer->CmdGenerateMipmaps(texture.mImage,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_ACCESS_SHADER_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		commanBuffer->End();
		commanBuffer->Submit({}, {}, fence);
		fence->Wait();
		CORE_INFO("load texture:{}", textureFileNames[i]);
	}

	//Process texture load error
	uint32_t error = 0;
	std::vector<uint32_t> remap(mTextures.size());
	for (uint32_t i = 0; i < static_cast<uint32_t>(mTextures.size()); i++)
	{
		remap[i] = error;
		if (mTextures[i].mImage == nullptr)
		{
			CORE_ERROR("failed to load texture:{}", textureFileNames[i]);
		}
		error += (mTextures[i].mImage != nullptr);
	}
	for (uint32_t i = 0; i < static_cast<uint32_t>(mTextures.size()); i++)
	{
		mTextures[i] = mTextures[remap[i]];
	}
	mTextures.resize(error);

	for (uint32_t i = 0; i < static_cast<uint32_t>(mDrawCmd.size()); i++)
	{
		if (mDrawCmd[i].mPushConstant.mTextureId1 != UINT32_MAX)
		{
			mDrawCmd[i].mPushConstant.mTextureId1 = remap[mDrawCmd[i].mPushConstant.mTextureId1];
		}
	}
}

bool Scene::load_buffer_and_draw_cmd(const std::shared_ptr<MVK::VulkanQueue> graphicQueue, const std::vector<Mesh>& meshs)
{
	mDrawCmd.clear();
	//create a big buffer,all vertices fill in the buffer
	uint32_t indexCount = 0;
	std::vector<Vertex> nativeVertices;
	for (const Mesh& mesh : meshs)
		indexCount += mesh.mVertices.size();
	nativeVertices.resize(indexCount);
	mDrawCmd.resize(indexCount);
	uint32_t offset = 0;
	for (uint32_t i = 0; i < static_cast<uint32_t>(meshs.size()); i++)
	{
		const Mesh& mesh = meshs[i];
		mDrawCmd[i].mPushConstant.mTextureId1 = mesh.mTextureBase;
		mDrawCmd[i].mPushConstant.mAlbedo = glm::packUnorm4x8(glm::vec4(mesh.mAlbedo, 0.0f));
		mDrawCmd[i].mIndexCount = mesh.mVertices.size();
		mDrawCmd[i].mFirstIndex = offset;
		//std::copy(mesh.mVertices.begin(), mesh.mVertices.end(), nativeVertices.begin() + offset);
		memcpy(nativeVertices.data() + offset, mesh.mVertices.data(), mesh.mVertices.size() * sizeof(Vertex));
		offset += mesh.mVertices.size();
	}

	//optimize
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices(indexCount);
	std::vector<uint32_t> remap(indexCount);
	//No.1
	uint32_t vertexCount = meshopt_generateVertexRemap(remap.data(), nullptr, indexCount, nativeVertices.data(), indexCount, sizeof(Vertex));
	vertices.resize(vertexCount);
	meshopt_remapVertexBuffer(vertices.data(), nativeVertices.data(), indexCount, sizeof(Vertex), remap.data());
	meshopt_remapIndexBuffer(indices.data(), nullptr, indexCount, remap.data());
	nativeVertices.clear();
	//No.2&3
	offset = 0;
	uint32_t meshVertexCount = 0;
	for (uint32_t i = 0; i < static_cast<uint32_t>(meshs.size()); i++)
	{
		meshVertexCount = meshs[i].mVertices.size();
		//No.2
		meshopt_optimizeVertexCache(indices.data() + offset, indices.data() + offset, meshVertexCount, vertexCount);
		//No.3
		meshopt_optimizeOverdraw(indices.data() + offset, indices.data() + offset, meshVertexCount, &vertices[0].mPosition.x, vertexCount, sizeof(Vertex), 1.05f);
		offset += meshVertexCount;
	}
	//No.4
	meshopt_optimizeVertexFetch(vertices.data(), indices.data(), indexCount, vertices.data(), vertexCount, sizeof(Vertex));
	CORE_INFO("mesh opt: {} / {} vertex buffer", vertices.size(), indexCount);

	//create vertex buffer
	const std::shared_ptr < MVK::VulkanDevice>& device = graphicQueue->GetDevicePtr();
	VkDeviceSize vertexBufferSize = vertices.size() * sizeof(Vertex);
	VkDeviceSize indexBufferSize = indices.size() * sizeof(uint32_t);

	mVertexBuffer = MVK::VulkanBuffer::CreateBuffer(device, vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	mIndexBuffer = MVK::VulkanBuffer::CreateBuffer(device, indexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	std::shared_ptr<MVK::VulkanBuffer> vertexStaging = MVK::VulkanBuffer::CreateStaging(device, vertexBufferSize);
	std::shared_ptr<MVK::VulkanBuffer> indexStaging = MVK::VulkanBuffer::CreateStaging(device, indexBufferSize);

	vertexStaging->UpdateData(vertices.data(), vertices.data() + vertices.size());
	indexStaging->UpdateData(indices.data(), indices.data() + indices.size());

	std::shared_ptr<MVK::VulkanFence> fence = MVK::VulkanFence::Create(graphicQueue->GetDevicePtr());
	std::shared_ptr<MVK::VulkanCommandPool> commandPool = MVK::VulkanCommandPool::Create(graphicQueue, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	std::shared_ptr<MVK::VulkanCommandBuffer> commandBuffer = MVK::VulkanCommandBuffer::Create(commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	commandBuffer->Begin();
	commandBuffer->CmdCopyBuffer(vertexStaging, mVertexBuffer);
	commandBuffer->CmdCopyBuffer(indexStaging, mIndexBuffer);
	commandBuffer->End();
	commandBuffer->Submit({}, {}, fence);
	fence->Wait();

	return true;
}

void Scene::create_descriptor(std::shared_ptr<MVK::VulkanDevice> device)
{
	VkDescriptorSetLayoutBinding layoutBinding{};
	layoutBinding.binding = 0;
	layoutBinding.descriptorCount = mTextures.size();
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	mDescriptorSetLayout = MVK::VulkanDescriptorSetLayout::CreaeteDescriptorSetLayout(device, { layoutBinding });

	VkDescriptorPoolSize poolSize{};
	poolSize.descriptorCount = mTextures.size();
	poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	mDescriptorPool = MVK::VulkanDescriptorPool::Create(device, 1, { poolSize });

	mDescriptorSet = MVK::VulkanDescriptorSet::Create(mDescriptorPool, mDescriptorSetLayout);

	std::vector<VkDescriptorImageInfo> imageInfos(mTextures.size());
	for (uint32_t i = 0; i < static_cast<uint32_t>(mTextures.size()); i++)
	{
		imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfos[i].imageView = mTextures[i].mImageView->GetHandle();
		imageInfos[i].sampler = mTextures[i].mSampler->GetHandle();
	}

	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.pImageInfo = imageInfos.data();
	descriptorWrite.descriptorCount = imageInfos.size();
	descriptorWrite.dstSet = mDescriptorSet->GetHandle();
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	vkUpdateDescriptorSets(device->GetHandle(), 1, &descriptorWrite, 0, nullptr);

}

bool Scene::Initialize(std::shared_ptr<MVK::VulkanQueue> graphicQueue, const char* file_name)
{
	std::string dir = get_base_dir(file_name);

	std::vector<Mesh> meshs;
	std::vector<std::string> textureFilenames;

	load_meshs(file_name, dir.c_str(), &meshs, &textureFilenames);
	load_buffer_and_draw_cmd(graphicQueue, meshs);
	load_texture(graphicQueue, textureFilenames);
	create_descriptor(graphicQueue->GetDevicePtr());

	CORE_INFO("success load scene : {}", file_name);

	return true;
}