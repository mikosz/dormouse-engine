#ifndef _DORMOUSEENGINE_RENDERER_DRAWCOMMAND_HPP_
#define _DORMOUSEENGINE_RENDERER_DRAWCOMMAND_HPP_

#include <cstdint>
#include <vector>

#include "dormouse-engine/graphics/CommandList.hpp"
#include "dormouse-engine/graphics/ConstantBuffer.hpp"
#include "dormouse-engine/graphics/RenderState.hpp"
#include "dormouse-engine/graphics/Sampler.hpp"
#include "dormouse-engine/graphics/ShaderType.hpp"
#include "dormouse-engine/graphics/InputLayout.hpp"
#include "dormouse-engine/graphics/Shader.hpp"
#include "dormouse-engine/graphics/Texture.hpp"
#include "dormouse-engine/graphics/Viewport.hpp"
#include "dormouse-engine/graphics/VertexBuffer.hpp"
#include "dormouse-engine/graphics/IndexBuffer.hpp"
#include "dormouse-engine/graphics/PrimitiveTopology.hpp"

#include "dormouse-engine/essentials/smart-pointer-definitions.hpp"

namespace dormouse_engine::renderer {

// TODO: figure out the draw command types. Compute command should also be incorporated here, but wont
// use the stuff from this class (add GPUCommand as superclass with run and key?)
class DrawCommand {
public:

	using Key = std::uint64_t;

	DrawCommand() = default;

	DrawCommand(const DrawCommand&) = delete;

	virtual ~DrawCommand() = default;

	void operator=(const DrawCommand&) = delete;

	Key key() const {
		return 0ull; // TODO
	}

	void submit(graphics::CommandList& commandList);

	void setInputLayout(const graphics::InputLayout* inputLayout) { // TODO: POINTERS!!! CONST!
		inputLayout_ = inputLayout;
	}

	void setVertexShader(graphics::VertexShader* vertexShader) {
		vertexShader_ = vertexShader;
	}

	void setGeometryShader(graphics::GeometryShader* geometryShader) {
		geometryShader_ = geometryShader;
	}

	void setHullShader(graphics::HullShader* hullShader) {
		hullShader_ = hullShader;
	}

	void setDomainShader(graphics::DomainShader* domainShader) {
		domainShader_ = domainShader;
	}

	void setPixelShader(graphics::PixelShader* pixelShader) {
		pixelShader_ = pixelShader;
	}

	void setRenderState(const graphics::RenderState* renderState) {
		renderState_ = renderState;
	}

	void addConstantBufferData(
		graphics::ConstantBuffer* constantBuffer,
		std::uint8_t* data,
		size_t size,
		graphics::ShaderType stage,
		size_t slot
		) {
		constantBuffersData_.emplace_back(constantBuffer, data, size, stage, slot);
	}

	void addTexture(
		graphics::Texture texture,
		graphics::ShaderType stage,
		size_t slot
		)
	{
		textures_.emplace_back(std::move(texture), stage, slot);
	}

	void addSampler(graphics::Sampler sampler, graphics::ShaderType stage, size_t slot) {
		samplers_.emplace_back(std::move(sampler), stage, slot);
	}

	void setRenderTarget(graphics::Texture2d* texture) { // TODO: pointers
		renderTarget_ = texture;
	}

	void setDepthStencil(graphics::Texture2d* texture) {
		depthStencil_ = texture;
	}

	void setViewport(graphics::Viewport* viewport) {
		viewport_ = viewport;
	}

		// TODO: pointers!
	void setVertexBuffer(graphics::VertexBuffer* vertexBuffer) {
		vertexBuffer_ = vertexBuffer;
	}

	void setIndexBuffer(graphics::IndexBuffer* indexBuffer) { // TODO: index count and index buffer in one call?
		indexBuffer_ = indexBuffer;
	}

	void setIndexCount(size_t indexCount) {
		indexCount_ = indexCount;
	}

	void setPrimitiveTopology(graphics::PrimitiveTopology primitiveTopology) {
		primitiveTopology_ = primitiveTopology;
	}

	void setInstanceDataBuffer(graphics::VertexBuffer* instanceDataBuffer) {
		instanceDataBuffer_ = instanceDataBuffer;
	}

	void setInstanceCount(size_t instanceCount) {
		instanceCount_ = instanceCount;
	}

private:

	struct ConstantBufferData {

		using Data = std::vector<std::uint8_t>; // TODO: array? pointer?

		graphics::ConstantBuffer* constantBuffer;

		Data data;

		graphics::ShaderType stage;

		size_t slot;

		ConstantBufferData(
			graphics::ConstantBuffer* constantBuffer,
			std::uint8_t* dataPtr,
			size_t size,
			graphics::ShaderType stage,
			size_t slot
			);

	};

	struct Resource {

		graphics::ShaderType stage;

		size_t slot;

		Resource(graphics::ShaderType stage, size_t slot) :
			stage(stage),
			slot(slot)
		{
		}

	};

	struct Texture : Resource {

		graphics::Texture texture;

		Texture(graphics::Texture texture, graphics::ShaderType stage, size_t slot) :
			Resource(stage, slot),
			texture(texture)
		{
		}

	};

	struct Sampler : Resource {

		graphics::Sampler sampler;

		Sampler(graphics::Sampler sampler, graphics::ShaderType stage, size_t slot) :
			Resource(stage, slot),
			sampler(sampler)
		{
		}

	};

	using ConstantBuffersData = std::vector<ConstantBufferData>; // TODO: ,,

	using Textures = std::vector<Texture>; // TODO: ,,

	using Samplers = std::vector<Sampler>; // TODO: use array to avoid allocs

	// TODO: pointers
	graphics::Viewport* viewport_ = nullptr;

	graphics::Texture2d* renderTarget_ = nullptr;

	graphics::Texture2d* depthStencil_ = nullptr;

	const graphics::InputLayout* inputLayout_ = nullptr;

	graphics::VertexShader* vertexShader_ = nullptr;

	graphics::GeometryShader* geometryShader_ = nullptr;

	graphics::HullShader* hullShader_ = nullptr;

	graphics::DomainShader* domainShader_ = nullptr;

	graphics::PixelShader* pixelShader_ = nullptr;

	const graphics::RenderState* renderState_ = nullptr;

	Samplers samplers_;

	ConstantBuffersData constantBuffersData_;

	Textures textures_;

	graphics::VertexBuffer* vertexBuffer_ = nullptr;

	graphics::VertexBuffer* instanceDataBuffer_ = nullptr;

	size_t instanceCount_ = 0u;

	graphics::IndexBuffer* indexBuffer_ = nullptr;

	size_t indexCount_ = 0u;

	graphics::PrimitiveTopology primitiveTopology_ = graphics::PrimitiveTopology::INVALID;

};

DE_SMART_POINTER_DEFINITONS(DrawCommand);

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_DRAWCOMMAND_HPP_ */
