#include "Pass.hpp"

#include "dormouse-engine/renderer/PassContext.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;

void Pass::bind(DrawCommand& drawCommand, const Properties& properties) const {
	drawCommand.setInputLayout(&input_.layout());
	drawCommand.setVertexShader(&vertexShader_->shaderData());
	vertexShader_->bind(drawCommand, properties);

	if (geometryShader_) {
		drawCommand.setGeometryShader(&geometryShader_->shaderData());
		geometryShader_->bind(drawCommand, properties);
	}

	if (hullShader_) {
		drawCommand.setHullShader(&hullShader_->shaderData());
		hullShader_->bind(drawCommand, properties);
	}

	if (domainShader_) {
		drawCommand.setDomainShader(&domainShader_->shaderData());
		domainShader_->bind(drawCommand, properties);
	}

	drawCommand.setPixelShader(&pixelShader_->shaderData());
	pixelShader_->bind(drawCommand, properties);
}
