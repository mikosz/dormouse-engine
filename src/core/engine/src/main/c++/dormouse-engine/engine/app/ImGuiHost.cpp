#include "ImGuiHost.hpp"

#include <string>
#include <tuple>

#include <imgui.h>

#include "dormouse-engine/essentials/debug.hpp"
#include "dormouse-engine/essentials/Range.hpp"
#include "dormouse-engine/graphics/Texture.hpp"
#include "dormouse-engine/graphics/ShaderCompiler.hpp"
#include "dormouse-engine/renderer/command/CommandBuffer.hpp"
#include "dormouse-engine/renderer/control/Sampler.hpp"
#include "dormouse-engine/renderer/control/RenderState.hpp"
#include "dormouse-engine/renderer/control/Viewport.hpp"
#include "dormouse-engine/math/Transform.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::engine;
using namespace dormouse_engine::engine::app;

using namespace std::string_literals;

namespace /* anonymous */ {

const auto IMGUI_TECHNIQUE_CODE = R"(
	cbuffer VertexBufferData {
		float4x4 projectionMatrix;
	};

	struct VIn {
		float2 pos : POSITION;
		float4 colour : COLOR0;
		float2 texcoord : TEXCOORD0;
	};

	struct PIn {
		float4 pos : SV_POSITION;
		float4 colour : COLOR0;
		float2 texcoord : TEXCOORD0;
	};

	SamplerState fontAtlasSampler;
	Texture2D fontAtlasTexture;

	PIn vs(VIn vin) {
		PIn pin;
		pin.pos = mul(float4(vin.pos.xy, 0.0f, 1.0f), projectionMatrix);
		pin.colour = vin.colour;
		pin.texcoord = vin.texcoord;

		return pin;
	}

	float4 ps(PIn pin) : SV_Target {
		return pin.colour * fontAtlasTexture.Sample(fontAtlasSampler, pin.texcoord);
	}
	)"s;

std::tuple<graphics::Buffer, size_t> createImguiVertexBuffer(graphics::Device& graphicsDevice, size_t vertexCount) {
	auto configuration = graphics::Buffer::Configuration();
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = true;
	configuration.purpose = graphics::Buffer::CreationPurpose::VERTEX_BUFFER;
	configuration.size = vertexCount * sizeof(ImDrawVert);

	return { graphics::Buffer(graphicsDevice, configuration), vertexCount };
}

std::tuple<graphics::Buffer, size_t> createImguiIndexBuffer(graphics::Device& graphicsDevice, size_t indexCount) {
	auto configuration = graphics::Buffer::Configuration();
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = true;
	configuration.purpose = graphics::Buffer::CreationPurpose::INDEX_BUFFER;
	configuration.size = indexCount * sizeof(ImDrawIdx);

	return { graphics::Buffer(graphicsDevice, configuration), indexCount };
}

graphics::Buffer createImguiConstantBuffer(graphics::Device& graphicsDevice, size_t width, size_t height) {
	auto configuration = graphics::Buffer::Configuration();
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = false;
	configuration.purpose = graphics::Buffer::CreationPurpose::CONSTANT_BUFFER;
	configuration.size = sizeof(math::Matrix4x4);

	const auto transform = math::Transform::orthographicProjection(
		math::Handedness::LEFT,
		0.0f,
		static_cast<float>(width),
		0.0f,
		static_cast<float>(height),
		0.0f,
		1.0f,
		graphics::Device::NDC_NEAR
		);

	return graphics::Buffer(
		graphicsDevice,
		configuration,
		essentials::viewBuffer(&transform.matrix(), sizeof(math::Matrix4x4))
		);
}

renderer::shader::Technique createImguiTechnique(graphics::Device& graphicsDevice) {
	auto technique = renderer::shader::Technique();

	auto flags = graphics::ShaderCompiler::CompilerFlags();

	if (essentials::IS_DEBUG) {
		flags = graphics::ShaderCompiler::FULL_DEBUG_MASK;
	}

	auto shaderCompiler = graphics::ShaderCompiler(flags);

	{
		auto compiledVertexShader = shaderCompiler.compile(
			essentials::viewBuffer(IMGUI_TECHNIQUE_CODE), "imgui", "vs", graphics::ShaderType::VERTEX);
		technique.setShader(
			renderer::shader::VertexShader(graphicsDevice, essentials::viewBuffer(compiledVertexShader)));

		auto inputLayout = renderer::shader::InputLayout(graphicsDevice, essentials::viewBuffer(compiledVertexShader));
		technique.setInputLayout(std::move(inputLayout));
	}

	{
		auto compiledPixelShader = shaderCompiler.compile(
			essentials::viewBuffer(IMGUI_TECHNIQUE_CODE), "imgui", "ps", graphics::ShaderType::PIXEL);
		technique.setShader(
			renderer::shader::PixelShader(graphicsDevice, essentials::viewBuffer(compiledPixelShader)));
	}

	return technique;
}

renderer::control::Control createRenderControl(graphics::Device& graphicsDevice, size_t width, size_t height) {
	auto commandKey = renderer::command::CommandKey(
		renderer::command::FullscreenLayerId::DEBUG,
		renderer::command::ViewportId::FULLSCREEN,
		renderer::command::ViewportLayer::HUD,
		renderer::command::TranslucencyType::OPAQUE,
		0,
		0
		);

	auto renderTarget = renderer::control::RenderTargetView(graphicsDevice.backBuffer());

	auto viewportConfiguration = graphics::Viewport::Configuration(); // TODO: preset!
	viewportConfiguration.width = static_cast<float>(width);
	viewportConfiguration.height = static_cast<float>(height);
	viewportConfiguration.minDepth = 0.0f;
	viewportConfiguration.maxDepth = 1.0f;
	viewportConfiguration.topLeftX = 0.0f;
	viewportConfiguration.topLeftY = 0.0f;
	auto viewport = renderer::control::Viewport(viewportConfiguration);

	auto renderStateConfiguration = graphics::RenderState::Configuration();
	renderStateConfiguration.cullMode = graphics::RenderState::CullMode::NONE;
	renderStateConfiguration.fillMode = graphics::RenderState::FillMode::SOLID;
	renderStateConfiguration.frontCounterClockwise = false;
	renderStateConfiguration.blendingEnabled = true;
	auto renderState = renderer::control::RenderState(graphicsDevice, renderStateConfiguration);

	return renderer::control::Control(
		commandKey,
		renderer::control::DepthStencilView(),
		renderTarget,
		viewport,
		renderState
		);
}

} // anonymous namespace

ImGuiHost::ImGuiHost(time::Timer timer, graphics::Device& graphicsDevice, size_t width, size_t height) :
	timer_(std::move(timer)),
	constantBuffer_(createImguiConstantBuffer(graphicsDevice, width, height)),
	technique_(createImguiTechnique(graphicsDevice)),
	renderControl_(createRenderControl(graphicsDevice, width, height))
{
	auto& imguiIO = ImGui::GetIO();
	imguiIO.DisplaySize.x = static_cast<float>(width);
	imguiIO.DisplaySize.y = static_cast<float>(height);
	imguiIO.RenderDrawListsFn = nullptr;

	auto* imguiFontAtlasPixels = static_cast<essentials::Byte*>(nullptr);
	auto imguiFontAtlasWidth = int();
	auto imguiFontAtlasHeight = int();
	// TODO: try alpha 8
	imguiIO.Fonts->GetTexDataAsRGBA32(&imguiFontAtlasPixels, &imguiFontAtlasWidth, &imguiFontAtlasHeight);

	const auto pixelFormat = graphics::FORMAT_R8G8B8A8_UNORM;

	auto imguiFontAtlasConfiguration = graphics::Texture::Configuration2d();
	imguiFontAtlasConfiguration.allowCPURead = false;
	imguiFontAtlasConfiguration.allowGPUWrite = false;
	imguiFontAtlasConfiguration.allowModifications = false;
	imguiFontAtlasConfiguration.width = imguiFontAtlasWidth;
	imguiFontAtlasConfiguration.height = imguiFontAtlasHeight;
	imguiFontAtlasConfiguration.pixelFormat = pixelFormat;
	imguiFontAtlasConfiguration.purposeFlags = graphics::Texture::CreationPurpose::SHADER_RESOURCE;

	auto imguiFontAtlasTexture = graphics::Texture(
		graphicsDevice,
		imguiFontAtlasConfiguration,
		essentials::viewBuffer(imguiFontAtlasPixels, imguiFontAtlasWidth * imguiFontAtlasHeight * pixelFormat.pixelSize())
		);

	imguiFontAtlas_ = renderer::control::ResourceView(imguiFontAtlasTexture);

	imguiIO.Fonts->TexID = &imguiFontAtlas_;
}

void ImGuiHost::update() {
	auto& imguiIO = ImGui::GetIO();
	imguiIO.DeltaTime = timer_.lastFrameDuration();
	ImGui::NewFrame();

	ImGui::Text("Hejo Okruch!");
}

void ImGuiHost::render(
	graphics::Device& graphicsDevice, renderer::command::CommandBuffer& rendererCommandBuffer)
{
	ImGui::Render();

	const auto& imguiDrawData = *ImGui::GetDrawData();
	auto cmdIdx = size_t(0);

	populateBuffers_(graphicsDevice, imguiDrawData);

	for (const auto cmdListIdx : essentials::IndexRange(0, imguiDrawData.CmdListsCount)) {
		const auto* cmdList = imguiDrawData.CmdLists[cmdListIdx];

		for (const auto& cmd : cmdList->CmdBuffer) {
			if (cmd.UserCallback != nullptr) {
				cmd.UserCallback(cmdList, &cmd);
			} else {
				const auto fontAtlas = *static_cast<const renderer::control::ResourceView*>(cmd.TextureId);

				auto& drawCommand =
					rendererCommandBuffer.create(renderer::command::CommandBuffer::CommandId{ this, cmdIdx++ });

				drawCommand.setVertexBuffer(vertexBuffer_, vertexCount_, sizeof(ImDrawVert));
				drawCommand.setIndexBuffer(indexBuffer_, indexCount_, sizeof(ImDrawIdx));
				drawCommand.setResource(fontAtlas, graphics::ShaderType::PIXEL, 0u);
				drawCommand.setSampler(
					renderer::control::Sampler(graphicsDevice, renderer::control::Sampler::WRAPPED_LINEAR),
					graphics::ShaderType::PIXEL,
					0
					);
				drawCommand.setPrimitiveTopology(graphics::PrimitiveTopology::TRIANGLE_LIST);
				drawCommand.setConstantBuffer(constantBuffer_, graphics::ShaderType::VERTEX, 0u);
				drawCommand.setTechnique(essentials::make_observer(&technique_));
				drawCommand.setRenderControl(renderControl_);
			}
		}
	}
}

void ImGuiHost::populateBuffers_(graphics::Device& graphicsDevice, const ImDrawData& imguiDrawData) {
	vertexCount_ = imguiDrawData.TotalVtxCount;
	indexCount_ = imguiDrawData.TotalIdxCount;

	if (vertexCount_ == 0 || indexCount_ == 0) {
		return;
	}

	if (imguiDrawData.TotalVtxCount > vertexBufferSize_) {
		std::tie(vertexBuffer_, vertexBufferSize_) = createImguiVertexBuffer(graphicsDevice, vertexCount_);
	}

	if (imguiDrawData.TotalIdxCount > indexBufferSize_) {
		std::tie(indexBuffer_, indexBufferSize_) = createImguiIndexBuffer(graphicsDevice, indexCount_);
	}

	auto vertexData =
		graphicsDevice.getImmediateCommandList().lock(vertexBuffer_, graphics::CommandList::LockPurpose::WRITE_DISCARD);
	auto indexData =
		graphicsDevice.getImmediateCommandList().lock(indexBuffer_, graphics::CommandList::LockPurpose::WRITE_DISCARD);

	auto* vertexPtr = reinterpret_cast<ImDrawVert*>(vertexData.pixels.get());
	auto* indexPtr = reinterpret_cast<ImDrawIdx*>(indexData.pixels.get());

	for (const auto cmdListIdx : essentials::IndexRange(0, imguiDrawData.CmdListsCount)) {
		const auto* cmdList = imguiDrawData.CmdLists[cmdListIdx];
		vertexPtr = std::copy(cmdList->VtxBuffer.begin(), cmdList->VtxBuffer.end(), vertexPtr);
		indexPtr = std::copy(cmdList->IdxBuffer.begin(), cmdList->IdxBuffer.end(), indexPtr);
	}
}
