/*
For now I will use the OPENGL windowing system, but I will eventually make my own windowing system.
*/

#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>


namespace Engine
{

	// Callback function for key events
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	// Callback function for resizing the window
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	class Window
	{
	public:

		Window();
		// Initialize the window
		void Init();

		~Window();

		// Return true if the window attributes are set
		inline bool is_attr_set() const;
		// Create the OpenGL window
		void create_window();
		// Make the window the one we will be working with
		void set_context_current() const;
		// Bind the Resize and Key call back functions
		void set_callbacks() const;

		void set_vsync() const;

			
		// Clear the color buffer
		void clear_frame() const;
		// First thing rendered into the window
		void postFrame() const;
		// Last thing rendered into the window
		void afterFrame() const;

		// Check if the window is closed
		bool is_closed() { 
			return glfwWindowShouldClose(window_); 
		};

		inline int get_width() const { 
			return width_; 
		};
		inline int get_height() const { 
			return height_; 
		};

		inline void set_attr(const int width, const int height, const char* title) {
			set_dimensions(width, height);
			set_title(title);
		};

		inline void set_width(const int width) {
			width_ = width; 
		};
		inline void set_height(const int height) {
			height_ = height; 
		};
		inline void set_title(const char* title) {
			title_ = title;
		};
		inline void set_vsync(const bool vsync) {
			VSync_ = vsync;
		};

		void set_dimensions(const int width, const int height) {
			set_width(width);
			set_height(height);
		}

		inline GLFWwindow* get_windowPtr() const { 
			return window_; 
		};

	private:

		GLFWwindow* window_ = nullptr;
		const char* title_ = nullptr;

		unsigned int width_ = 0;
		unsigned int height_ = 0;
		bool VSync_ = false;

	};
	
}