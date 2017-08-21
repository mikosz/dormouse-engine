#include <boost/test/auto_unit_test.hpp>

#include <memory>

#include "dormouse-engine/milk/graphics/TestSuite.hpp"

#include "dormouse-engine/milk/fs.hpp"

#include "dormouse-engine/renderer/shader/InputFactory.hpp"
#include "dormouse-engine/renderer/shader/ShaderFactory.hpp"
#include "dormouse-engine/renderer/OrientedCamera.hpp"
#include "dormouse-engine/renderer/CommandBuffer.hpp"
#include "dormouse-engine/renderer/Scene.hpp"

#include "dormouse-engine/milk/graphics/DirectXError.hpp"

using namespace dormouse_engine;
using namespace dormouse_engine;
using namespace dormouse_engine::renderer;
using namespace dormouse_engine::renderer::shader;

BOOST_FIXTURE_TEST_SUITE(PulpRendererShaderShaderFactoryTestSuite, milk::graphics::TestSuite);

BOOST_AUTO_TEST_CASE(SetsConstantBuffers) {
	// init shaders
	auto fs = std::make_shared<milk::Filesystem>();

	auto mount = std::make_unique<milk::DirectoryMount>("Debug/", true); // TODO!
	fs->mount("/", std::move(mount), milk::Filesystem::PredecessorHidingPolicy::HIDE);

	auto fsContext = milk::FilesystemContext(fs);

	auto passFactory = shader::PassFactory();
	passFactory.scanCompiledShaderDirectory(fs, "");

	// init data
	auto scene = Scene(renderer());

	{
		// TODO: put this in a file
		auto submeshes = Mesh::Submeshes();
		auto materials = Mesh::MaterialConfigurations();

		{
			auto material = MaterialConfiguration();

			material.renderStateConfiguration().cullMode = milk::graphics::RenderState::CullMode::BACK;
			material.renderStateConfiguration().fillMode = milk::graphics::RenderState::FillMode::SOLID;
			material.renderStateConfiguration().frontCounterClockwise = false;

			material.properties().emplace(mesh::MaterialConfiguration::AMBIENT_COLOUR_PROPERTY,
				Colour(0.01f, 0.02f, 0.03f, 0.04f));
			material.properties().emplace(mesh::MaterialConfiguration::DIFFUSE_COLOUR_PROPERTY,
				Colour(0.05f, 0.06f, 0.07f, 0.08f));
			material.properties().emplace(mesh::MaterialConfiguration::SPECULAR_COLOUR_PROPERTY,
				Colour(0.09f, 0.1f, 0.11f, 0.12f));

			material.shaderName() = "ConstantBuffers";

			materials.emplace("phong", std::move(material));

			auto submesh = Submesh();

			submesh.primitiveTopology() = milk::graphics::PrimitiveTopology::TRIANGLE_STRIP;

			auto vertex = Submesh::Vertex();
			vertex.normal = Vector(0.0f, 0.0f, -1.0f);

			vertex.position = Position(-1.0f, 1.0f, 0.0f);
			submesh.indices().emplace_back(submesh.vertices().size());
			submesh.vertices().emplace_back(vertex);

			vertex.position = Position(-1.0f, -1.0f, 0.0f);
			submesh.indices().emplace_back(submesh.vertices().size());
			submesh.vertices().emplace_back(vertex);

			vertex.position = Position(1.0f, -1.0f, 0.0f);
			submesh.indices().emplace_back(submesh.vertices().size());
			submesh.vertices().emplace_back(vertex);

			vertex.position = Position(1.0f, 1.0f, 0.0f);
			submesh.indices().emplace_back(submesh.vertices().size());
			submesh.vertices().emplace_back(vertex);

			submesh.materialId() = "phong";

			submeshes.emplace_back(std::move(submesh));
		}
		
		auto rectangleData = mesh::Mesh(submeshes, materials);
		auto model = std::make_shared<Model>(rectangleData, renderer(), passFactory, fs);
		auto rectangle = std::make_shared<Actor>(model);

		scene.add(rectangle);

		auto camera = std::make_unique<OrientedCamera>(); // TODO: shit api
		camera->translate(dormouse_engine::math::Vec3(0.13f, 0.14f, 0.15f));
		scene.setCamera(std::move(camera));

		{
			scene.add(lighting::DirectionalLight(
				math::Vec3(0.28f, 0.29f, 0.30f),
				math::Vec4(0.16f, 0.17f, 0.18f, 0.19f),
				math::Vec4(0.20f, 0.21f, 0.22f, 0.23f),
				math::Vec4(0.24f, 0.25f, 0.26f, 0.27f)
				));

			scene.add(lighting::DirectionalLight(
				math::Vec3(0.43f, 0.44f, 0.45f),
				math::Vec4(0.31f, 0.32f, 0.33f, 0.34f),
				math::Vec4(0.35f, 0.36f, 0.37f, 0.38f),
				math::Vec4(0.39f, 0.40f, 0.41f, 0.42f)
				));

			scene.add(lighting::PointLight(
				math::Vec3(0.46f, 0.47f, 0.48f),
				math::Vec3(0.49f, 0.50f, 0.51f),
				math::Vec4(0.52f, 0.53f, 0.54f, 0.55f),
				math::Vec4(0.56f, 0.57f, 0.58f, 0.59f),
				math::Vec4(0.60f, 0.61f, 0.62f, 0.63f)
				));
		}
	}

	// render
	for (;;)
	{
		app().update();

		renderer().beginScene();

		CommandBuffer commandBuffer;
		scene.render(commandBuffer);
		commandBuffer.submit(renderer().getImmediateCommandList());

		renderer().endScene();
	}

	BOOST_FAIL("No checks done");
}

BOOST_AUTO_TEST_SUITE_END(/* PulpRendererShaderShaderFactoryTestSuite */);
