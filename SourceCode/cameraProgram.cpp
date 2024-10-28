#include "cameraProgram.h"


namespace Engine {

	Camera::Camera() {
		// Stores the main vectors of the camera
		orientation_ = glm::vec3(0.0f, 0.0f, -1.0f);
		up_ = glm::vec3(0.0f, 1.0f, 0.0f);

		// Prevents the camera from jumping around when first clicking left click
		firstClick_ = true;

		// Adjust the speed of the camera and it's sensitivity when looking around
		speed_ = 0.1f;
		sensitivity_ = 100.0f;

		fov_ = 45.0f;

		nearPlane_ = 0.1f;
		farPlane_ = 100.0f;
	}

	void Camera::init() {
		// Check if the windows attributes are set
		if (!is_attr_set())
		{
			std::cerr << "ERROR::CAMERA::INIT : Initialise windows width heigth and position vector first." << std::endl;
			exit(EXIT_FAILURE);
			return;
		}

	}

	inline bool Camera::is_attr_set() const {
		return width_ != 0 && height_ != 0 && position_ != glm::vec3();
	}

	void Camera::set_Matrix_location(const Engine::Graphics::Shader* shader) {
		// Set the location to avoid repetition
		projectionMatrixLocation_ = glGetUniformLocation(shader->get_ID(), "projectionMatrix");
		viewMatrixLocation_ = glGetUniformLocation(shader->get_ID(), "viewMatrix");
	}

	void Camera::Matrix() const {
		// Check if the location of the matrices has been set
		if (projectionMatrixLocation_ == -1 || viewMatrixLocation_ == -1)
		{
			std::cerr << "ERROR::CAMERA::MATRIX : The location of the matrices has not been set." << std::endl;
			exit(EXIT_FAILURE);
			return;
		}
		
		// Initializes matrices since otherwise they will be the null matrix
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		// Makes camera look in the right direction from the right position
		view = glm::lookAt(position_, position_ + orientation_, up_);
		// Adds perspective to the scene
		projection = glm::perspective(glm::radians(fov_), (float)width_ / height_, nearPlane_, farPlane_);

		// Exports the camera matrix to the Vertex Shader
		glUniformMatrix4fv(projectionMatrixLocation_, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewMatrixLocation_, 1, GL_FALSE, glm::value_ptr(view));

	}



	void Camera::Inputs(GLFWwindow* window) {
		
		// Handles key inputs
		KeyInputs(window);

		// Handles mouse inputs
		MouseInputs(window);

	}

	void Camera::KeyInputs(GLFWwindow* window) {

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			position_ += speed_ * orientation_;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			position_ += speed_ * -glm::normalize(glm::cross(orientation_, up_));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			position_ += speed_ * -orientation_;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			position_ += speed_ * glm::normalize(glm::cross(orientation_, up_));
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			position_ += speed_ * up_;
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		{
			position_ += speed_ * -up_;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			speed_ = 0.004f;
		}
		else
		{
			speed_ = 0.001f;
		}
	}

	void Camera::MouseInputs(GLFWwindow* window) {

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			// Hides mouse cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			// Prevents camera from jumping on the first click
			if (firstClick_)
			{
				glfwSetCursorPos(window, (width_ / 2), (height_ / 2));
				firstClick_ = false;
			}

			// Stores the coordinates of the cursor
			double mouseX;
			double mouseY;
			// Fetches the coordinates of the cursor
			glfwGetCursorPos(window, &mouseX, &mouseY);

			// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
			// and then "transforms" them into degrees 
			float rotX = sensitivity_ * (float)(mouseY - (height_ / 2)) / height_;
			float rotY = sensitivity_ * (float)(mouseX - (width_ / 2)) / width_;

			// Calculates upcoming vertical change in the Orientation
			glm::vec3 newOrientation = glm::rotate(orientation_, glm::radians(-rotX), glm::normalize(glm::cross(orientation_, up_)));

			// Decides whether or not the next vertical Orientation is legal or not
			if (abs(glm::angle(newOrientation, up_) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				orientation_ = newOrientation;
			}

			// Rotates the Orientation left and right
			orientation_ = glm::rotate(orientation_, glm::radians(-rotY), up_);

			// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
			glfwSetCursorPos(window, (width_ / 2), (height_ / 2));
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			// Unhides cursor since camera is not looking around anymore
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			// Makes sure the next time the camera looks around it doesn't jump
			firstClick_ = true;
		}
	}

}