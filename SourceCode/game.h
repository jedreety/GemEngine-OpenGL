#pragma once

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "window.h"
#include "shaderProgram.h"

namespace Game
{
	class Game
	{
	public:
		Game();
		~Game();

		// Initialize GLFW
		void load_GLFW();

		// Set the OpenGL version
		void set_openGL_version(const int major, const int minor);

		// Set the OpenGL profile
		void set_openGL_profile(const int profile);

		// Load the OpenGL functions
		void load_GLAD();

		void run();

	private:
		Engine::Window* window_;
		Engine::Graphics::Shader* shader_;
		GLuint VAO;
	};

}