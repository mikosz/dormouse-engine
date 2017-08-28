#ifndef _DORMOUSEENGINE_RENDERER_CONTROL_SAMPLER_HPP_
#define _DORMOUSEENGINE_RENDERER_CONTROL_SAMPLER_HPP_

#include "dormouse-engine/graphics/Device.hpp"
#include "dormouse-engine/graphics/Sampler.hpp"

namespace dormouse_engine::renderer::control {

class Sampler final {
public:

	static constexpr graphics::Sampler::Configuration WRAPPED_LINEAR = {
		graphics::Sampler::AddressMode::WRAP,
		graphics::Sampler::AddressMode::WRAP,
		graphics::Sampler::AddressMode::WRAP,
		graphics::Sampler::Filter::MIN_MAG_MIP_LINEAR
		};

	Sampler(graphics::Device& graphicsDevice, graphics::Sampler::Configuration configuration);

	void bind(graphics::CommandList& commandList, graphics::ShaderType stage, size_t slot) const;

private:

	size_t samplerId_;

	friend bool operator==(const Sampler& lhs, const Sampler& rhs) {
		return lhs.samplerId_ == rhs.samplerId_;
	}

	friend bool operator!=(const Sampler& lhs, const Sampler& rhs) {
		return !(lhs == rhs);
	}

};

} // namespace dormouse_engine::renderer::control

#endif /* _DORMOUSEENGINE_RENDERER_CONTROL_SAMPLER_HPP_ */
