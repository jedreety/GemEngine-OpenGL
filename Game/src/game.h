#pragma once

#include <GlfwGlad.h>

#include<iostream>
#include <memory>

#include <Gem/Networking/network_client.h>

#include <Gem/Window/window.h>

#include <Gem/Graphics/camera.h>

#include <Gem/Graphics/shader_program.h>
#include <Gem/Graphics/tex_2D_array.h>

#include <Gem/Graphics/vao.h>
#include <Gem/Graphics/buffer.h>

#include <Gem/Core/timer.h>
#include <Gem/Core/scoped_timer.h>

#include <Gem/Voxel/chunk.h>
#include <Gem/Graphics/shapes/sphere.h>

class Game
{
public:
	Game();
	void run();

	~Game();

private:

	std::unique_ptr<Gem::Window::Window> window_;

	std::unique_ptr<Gem::Graphics::Camera> camera_;
	glm::vec3 oldPosition_ = glm::vec3();

	std::unique_ptr<Gem::Graphics::Texture2DArray> textureManager_;
	std::unique_ptr<Gem::Graphics::Shader> shader_;

	Gem::Graphics::VAO VAO_;
	Gem::Graphics::Buffer VBO_;
	Gem::Graphics::Buffer IBO_;

	Gem::Core::Timer gameTimer_;

	Network::Client* networkClient_;
	glm::vec3 playerPosition_;
	std::unordered_map<enet_uint32, glm::vec3> otherPlayersPositions_;
};