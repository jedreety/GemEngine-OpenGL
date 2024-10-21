#include "window.h"


namespace Engine {

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

		// Close the window if the ESC key is pressed
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            // Tell glfw to close the window
            glfwSetWindowShouldClose(window, GLFW_TRUE);

    }

    void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		// Verify that the width and height are not negative
        if (width < 0 || height < 0)
            throw std::invalid_argument("Width or Height of the Window cannot be negatie.");

        // Call Back of the resize 
        std::cout << "Resize W: " << width << " H: " << height << '\n';

		// Set the viewport size
        glViewport(0, 0, width, height);
    }

	Window::Window() {
		// Initialize default values for the window
		width_ = 800;
		height_ = 600;
		title_ = "Window default Title";
		VSync_ = true;

	}

    void Window::Init() {
		std::cout << "Creating WINDOW" << '\n';

		// Check if the windows attributes are set
		if (!is_attr_set())
        {
			std::cerr << "ERROR::WINDOW::INIT : Initialise windows width heigth and title first." << std::endl;
            exit(EXIT_FAILURE);
			return;
		}

		// Create the window
        create_window();

		// Make the window the one we will be working with
		set_context_current();
				
		// Bind the Resize and Key call back functions
        set_callbacks();
				
		// Tell OpenGL to use VSync if it is set
        set_vsync();

        std::cout << "Window created succesfully !" << '\n';

    }

	Window::~Window() {
        std::cout << "Deleting WINDOW" << '\n';
				
        if (window_ != nullptr)
            // Delete window before ending the program
            glfwDestroyWindow(window_);
    }

	inline bool Window::is_attr_set() const {
		return width_ && height_ && title_;
	}

	void Window::create_window() {
		window_ = glfwCreateWindow(width_, height_, title_, NULL, NULL);
		// Check if the window was created successfully
		if (!window_) {
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
	}

	void Window::set_context_current() const {
		// Tell OpenGL to use the window
		glfwMakeContextCurrent(window_);
	}

	void Window::set_callbacks() const {
		// Bind the window to the key callback function
		glfwSetKeyCallback(window_, key_callback);
		// Bind the window to the resize callback function
		glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
	}

	void Window::set_vsync() const {
		// OpenGL set VSync
		glfwSwapInterval(VSync_);
	}

			
	void Window::clear_frame() const {
        // Clean the back buffer and depth buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
			
	void Window::postFrame() const {
        // Specify the color of the background
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		// Clear the color buffer
		clear_frame();

    }
			
	void Window::afterFrame() const {
        // Swap front and back buffers
        glfwSwapBuffers(window_);

        // Poll for and process events
        glfwPollEvents();

        //glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
    }
    
}	