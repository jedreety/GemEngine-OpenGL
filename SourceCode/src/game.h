#pragma once

#include<iostream>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Network/Client/network_client.h"

#include "Engine/Window/engine_window.h"

#include "Engine/Camera/engine_camera.h"

#include "Engine/Graphics/Shader/engine_shaderProgram.h"
#include "Engine/Graphics/Texture/TextureManager/engine_textureManager.h"

#include "Engine/GLStructure/VertexArrayObject/gl_vao.h"
#include "Engine/GLStructure/BufferObject/gl_buffer.h"


class Game
{
public:
	Game();
	void run();

	~Game();

private:

	// Initialize GLFW
	void load_GLFW();

	// Set the OpenGL version
	void set_openGL_version(const int major, const int minor);

	// Set the OpenGL profile
	void set_openGL_profile(const int profile);

	// Load the OpenGL functions
	void load_GLAD();

	void set_parameters();

	void updateDeltaTime();

	std::unique_ptr<Engine::Window> window_;

	std::unique_ptr<Engine::Camera> camera_;
	glm::vec3 oldPosition_;

	std::unique_ptr<Engine::Graphics::Texture::TextureManager> textureManager_;
	std::unique_ptr<Engine::Graphics::Shader> shader_;

	Engine::GL::VAO VAO_;
	Engine::GL::Buffer VBO_;
	Engine::GL::Buffer IBO_;

	float deltaTime_ = 0.0f;
	double last_frameTime_ = 0.0;

	float fpsAccumulator_;
	int frameCount_;

	NetworkClient* networkClient_;
	glm::vec3 playerPosition_;
	std::unordered_map<enet_uint32, glm::vec3> otherPlayersPositions_;
};