#pragma once

#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "window.h"

#include "shaderProgram.h"
#include "textureManager.h"

#include "VAO.h"
#include "VBO.h"
#include "IBO.h"


class Game
{
public:
	Game();
	void run();

	~Game();

	// Initialize GLFW
	void load_GLFW();

	// Set the OpenGL version
	void set_openGL_version(const int major, const int minor);

	// Set the OpenGL profile
	void set_openGL_profile(const int profile);

	// Load the OpenGL functions
	void load_GLAD();

	void set_parameters();

private:
	Engine::Window* window_;

	Engine::Graphics::Shader* shader_;
	Engine::Graphics::Texture::TextureManager* textureManager_;

	Engine::GL::VAO VAO_;
	Engine::GL::VBO VBO_;
	Engine::GL::IBO IBO_;
};

