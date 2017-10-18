#ifndef _DORMOUSEENGINE_RENDERER_CONTROL_CONTROL_HPP_
#define _DORMOUSEENGINE_RENDERER_CONTROL_CONTROL_HPP_

#include "../command/CommandKey.hpp"
#include "DepthStencilView.hpp"
#include "RenderTargetView.hpp"
#include "RenderState.hpp"
#include "Viewport.hpp"

namespace dormouse_engine::renderer::control {

class Control final {
public:

	Control() = default;

	Control(
		command::CommandKey commandKey,
		DepthStencilView depthStencil,
		RenderTargetView renderTarget,
		Viewport viewport,
		RenderState renderState
		) :
		commandKey_(commandKey),
		depthStencil_(depthStencil),
		renderTarget_(renderTarget),
		viewport_(viewport),
		renderState_(renderState)
	{
	}


	command::CommandKey commandKey() const {
		return commandKey_;
	}

	DepthStencilView depthStencil() const {
		return depthStencil_;
	}

	RenderTargetView renderTarget() const {
		return renderTarget_;
	}

	Viewport viewport() const {
		return viewport_;
	}

	RenderState renderState() const {
		return renderState_;
	}

private:

	command::CommandKey commandKey_;

	DepthStencilView depthStencil_;

	RenderTargetView renderTarget_;

	Viewport viewport_;

	//ScissorRect scissorRect_;

	RenderState renderState_;

};

} // namespace dormouse_engine::renderer::control

namespace dormouse_engine::renderer {

using dormouse_engine::renderer::control::Control;

} // namespace dormouse_engine::renderer

#endif /* _DORMOUSEENGINE_RENDERER_CONTROL_CONTROL_HPP_ */
