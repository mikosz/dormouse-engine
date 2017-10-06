#ifndef _DORMOUSEENGINE_RENDERER_CONTROL_SAMPLER_HPP_
#define _DORMOUSEENGINE_RENDERER_CONTROL_SAMPLER_HPP_

#pragma warning(push, 3)
#	include <ponder/pondertype.hpp>
#pragma warning(pop)

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Sampler.hpp"
#include "../command/commandfwd.hpp"

namespace dormouse_engine::renderer::control {

class Sampler final {
public:

	static constexpr graphics::Sampler::Configuration WRAPPED_LINEAR = {
		graphics::Sampler::AddressMode::WRAP,
		graphics::Sampler::AddressMode::WRAP,
		graphics::Sampler::AddressMode::WRAP,
		graphics::Sampler::Filter::MIN_MAG_MIP_LINEAR
		};

	static constexpr graphics::Sampler::Configuration CLAMPED_LINEAR = {
		graphics::Sampler::AddressMode::CLAMP,
		graphics::Sampler::AddressMode::CLAMP,
		graphics::Sampler::AddressMode::CLAMP,
		graphics::Sampler::Filter::MIN_MAG_MIP_LINEAR
		};

	static void initialiseSystem(graphics::Device& graphicsDevice);

	Sampler() = default;

	Sampler(graphics::Device& graphicsDevice, graphics::Sampler::Configuration configuration);

	// TODO: MUST rename these
	void bind(graphics::CommandList& commandList, graphics::ShaderType stage, size_t slot) const;

	void bindToDrawCommand(command::DrawCommand& drawCommand, graphics::ShaderType stage, size_t slot) const;

private:

	static constexpr size_t INVALID_SAMPLER_ID = static_cast<size_t>(-1);

	size_t samplerId_ = INVALID_SAMPLER_ID;

	friend bool operator==(const Sampler& lhs, const Sampler& rhs) {
		return lhs.samplerId_ == rhs.samplerId_;
	}

	friend bool operator!=(const Sampler& lhs, const Sampler& rhs) {
		return !(lhs == rhs);
	}

};

namespace detail { void declareSampler(); }

} // namespace dormouse_engine::renderer::control

PONDER_AUTO_TYPE(
	dormouse_engine::renderer::control::Sampler,
	&dormouse_engine::renderer::control::detail::declareSampler
	);

#endif /* _DORMOUSEENGINE_RENDERER_CONTROL_SAMPLER_HPP_ */
