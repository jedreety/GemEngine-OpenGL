#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderProgram.h"

namespace Engine {
	class Camera
	{
	public:

		// Camera constructor to set up initial values
		Camera();
		void init();
		inline bool is_attr_set() const;

		void set_Matrix_location(const Engine::Graphics::Shader* shader);

		// Updates and exports the camera matrix to the Vertex Shader
		void Matrix() const;
		// Handles camera inputs
		void Inputs(GLFWwindow* window);
		void KeyInputs(GLFWwindow* window);
		void MouseInputs(GLFWwindow* window);

		inline void set_attr(const int width, const int height, const glm::vec3 position) {
			set_dimensions(width, height);
			set_position(position);
		};
		inline void set_width(const int width) { 
			width_ = width;
		};
		inline void set_height(const int height) { 
			height_ = height; 
		};
		inline void set_dimensions(const int width, const int height) { 
			set_width(width);
			set_height(height);
		};
		inline void set_position(const glm::vec3 position) {
			position_ = position;
		};
		inline void set_speed(const float speed) {
			speed_ = speed;
		};
		inline void set_sensitivity(const float sensitivity) {
			sensitivity_ = sensitivity;
		};

		inline void set_fov(const float fov) {
			fov_ = fov;
		};

		inline glm::vec3 get_position() const {
			return position_;
		};

	private:
		// Stores the main vectors of the camera
		glm::vec3 position_ = glm::vec3();
		glm::vec3 orientation_ = glm::vec3();
		glm::vec3 up_ = glm::vec3();

		// Prevents the camera from jumping around when first clicking left click
		bool firstClick_ = false;

		// Stores the width and height of the window
		int width_ = 0;
		int height_ = 0;

		// Adjust the speed of the camera and it's sensitivity when looking around
		float speed_ = 0.0f;
		float sensitivity_ = 0.0f;

		float fov_ = 0.0f;

		float nearPlane_ = 0.0f;
		float farPlane_ = 0.0f;

		GLint projectionMatrixLocation_ = -1;
		GLint viewMatrixLocation_ = -1;

	};
}
