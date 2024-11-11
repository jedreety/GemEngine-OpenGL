#pragma once

#include <GlfwGlad.h>

#include<iostream>
#include <memory>

#include <Gem/Networking/network_client.h>

#include <Gem/Window/window.h>

#include <Gem/Graphics/camera.h>

#include <Gem/Graphics/shader_program.h>
#include <Gem/Graphics/texture_manager.h>

#include <Gem/Graphics/vao.h>
#include <Gem/Graphics/buffer.h>

class Game
{
public:
	Game();
	void run();

	~Game();

private:

	void updateDeltaTime();

	std::unique_ptr<Gem::Window::Window> window_;

	std::unique_ptr<Gem::Graphics::Camera> camera_;
	glm::vec3 oldPosition_;

	std::unique_ptr<Gem::Graphics::TextureManager> textureManager_;
	std::unique_ptr<Gem::Graphics::Shader> shader_;

	Gem::Graphics::VAO VAO_;
	Gem::Graphics::Buffer VBO_;
	Gem::Graphics::Buffer IBO_;

	float deltaTime_ = 0.0f;
	double last_frameTime_ = 0.0;

	float fpsAccumulator_;
	int frameCount_;

	Network::Client* networkClient_;
	glm::vec3 playerPosition_;
	std::unordered_map<enet_uint32, glm::vec3> otherPlayersPositions_;
};