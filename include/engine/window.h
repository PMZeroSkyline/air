#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

struct key
{
	bool press = false;
	bool down = false;
	bool up = false;
	float press_dur = 0;
	float down_tm = 0;
	float down_dt = FLT_MAX;
};
struct window
{
	GLFWwindow* glfw_window;
	float tm = 0, dt = 0;
	key keys[GLFW_KEY_LAST];

	window(int w, int h, const char *title)
	{
		if (glfwGetCurrentContext() == nullptr)
		{

			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif
		}
		glfw_window = glfwCreateWindow(w, h, title, NULL, NULL);
		if (glfw_window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}
		glfwMakeContextCurrent(glfw_window);
		glfwSetWindowUserPointer(glfw_window, this);
        
		glfwSetFramebufferSizeCallback(glfw_window, [](GLFWwindow *w, int width, int height){
			reinterpret_cast<window*>(glfwGetWindowUserPointer(w))->framebuffer_size_callback(w, width, height);
		});
		glfwSetKeyCallback(glfw_window, [](GLFWwindow* w, int key, int scancode, int action, int mods){
			reinterpret_cast<window*>(glfwGetWindowUserPointer(w))->key_callback(w, key, scancode, action, mods);
		});
		
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    	{
        	std::cout << "Failed to initialize GLAD" << std::endl;
    	}    
	}
	~window()
	{
	    glfwTerminate();
	}
	bool is_open()
	{
		return !glfwWindowShouldClose(glfw_window);
	}
	void tick()
	{
		dt = glfwGetTime() - tm;
		tm = glfwGetTime();

		for (int i = 0; i < GLFW_KEY_LAST; i++)
		{
			key &k = keys[i];
			k.down = false;
			k.up = false;
			if (k.press)
				k.press_dur += dt;
			else
				k.press_dur = 0;
		}
		glfwSwapBuffers(glfw_window);
        glfwPollEvents();
	}
	void framebuffer_size_callback(GLFWwindow* w, int width, int height)
	{
    	glViewport(0, 0, width, height);
	}
	void key_callback(GLFWwindow* w, int id, int scancode, int action, int mods)
	{
		key& k = keys[id];

		if (action == 1) // event key down
		{
			k.press=true;
			k.down=true;
			k.down_dt = glfwGetTime() - k.down_tm;
			k.down_tm = glfwGetTime();
		}
		else if (action == 0) // event key up
		{
			k.press=false;
			k.up=true;
		}	
	}
	void close()
	{
		glfwSetWindowShouldClose(glfw_window, true);
	}
};
window* get_current_context()
{
    return (window*)glfwGetWindowUserPointer(glfwGetCurrentContext());
}
#endif
