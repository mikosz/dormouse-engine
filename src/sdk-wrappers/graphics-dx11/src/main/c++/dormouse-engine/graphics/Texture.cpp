#include "graphics.pch.hpp"

#include "Texture.hpp"

#include "Device.hpp"
#include "DirectXError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine::graphics;

void Texture::initialise(Device& renderer, dormouse_engine::Mask<CreationPurpose> purposeFlags) {
	if (purposeFlags & CreationPurpose::SHADER_RESOURCE) {
		checkDirectXCall(
			renderer.internalDevice().CreateShaderResourceView(internalResource(), nullptr, &shaderResourceView_.get()),
			"Failed to create a shader resource view of texture"
			);
	}

	if (purposeFlags & CreationPurpose::RENDER_TARGET) {
		checkDirectXCall(
			renderer.internalDevice().CreateRenderTargetView(internalResource(), nullptr, &renderTargetView_.get()),
			"Failed to create a render target view of texture"
			);
	}

	if (purposeFlags & CreationPurpose::DEPTH_STENCIL) {
		checkDirectXCall(
			renderer.internalDevice().CreateDepthStencilView(internalResource(), nullptr, &depthStencilView_.get()),
			"Failed to create a depth stencil view of texture"
			);
	}
}

void Texture::reset() {
	Resource::reset();

	shaderResourceView_.reset();
	depthStencilView_.reset();
	renderTargetView_.reset();
}
