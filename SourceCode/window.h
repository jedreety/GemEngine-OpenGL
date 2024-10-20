/*
For now I will use the OPENGL windowing system, but I will eventually make my own windowing system.
*/

#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

namespace Game
{
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
			bool is_attr_set();
			// Create the OpenGL window
			void create_window();
			// Make the window the one we will be working with
			void set_context_current();
			// Bind the Resize and Key call back functions
			void set_callbacks();

			void set_vsync();

			
			// Clear the color buffer
			void clear_frame();
			// First thing rendered into the window
			void postFrame();
			// Last thing rendered into the window
			void afterFrame();

			// Check if the window is closed
			bool is_closed() { 
				return glfwWindowShouldClose(window_); 
			};

			int get_width() { 
				return width_; 
			};
			int get_height() { 
				return height_; 
			};

			void set_width(const int width) {
				width_ = width; 
			};
			void set_height(const int height) {
				height_ = height; 
			};
			void set_title(const char* title) {
				title_ = title;
			};
			void set_vsync(const bool vsync) {
				VSync_ = vsync;
			};

			void set_dimention(const int width, const int height) {
				set_width(width);
				set_height(height);
			}

			GLFWwindow* get_windowPtr() { 
				return window_; 
			};

		private:

			GLFWwindow* window_ = nullptr;
			const char* title_ = nullptr;

			unsigned int width_ = 0;
			unsigned int height_ = 0;
			bool VSync_ = 1;

		};
		

	}
}