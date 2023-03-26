#ifndef WINDOW_H
#define WINDOW_H

#include "Core.h"

struct Key
{
	bool pressing = false;
	bool pressDown = false;
	bool liftUp = false;
	float pressDur = 0;
	float downTime = 0;
	float downDeltaTime = FLT_MAX;
};
class Window
{
public:
	GLFWwindow* glfw_window;
	float time = 0, deltaTime = 0;
	Key keys[GLFW_KEY_LAST];
	vector<string> dropPaths;

	Window(int w = 800, int h = 600, const char *title = "window")
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
			LOG("Failed to create GLFW window !")
			glfwTerminate();
		}
		LOG("create window succeed")
		glfwMakeContextCurrent(glfw_window);
		glfwSetWindowUserPointer(glfw_window, this);
        
		glfwSetFramebufferSizeCallback(glfw_window, [](GLFWwindow *w, int width, int height){
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(w))->FramebufferSizeCallback(w, width, height);
		});
		glfwSetKeyCallback(glfw_window, [](GLFWwindow* w, int key, int scancode, int action, int mods){
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(w))->KeyCallback(w, key, scancode, action, mods);
		});
		glfwSetDropCallback(glfw_window, [](GLFWwindow* w, int path_count, const char* paths[]){
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(w))->DropCallback(w, path_count, paths);
		});
		
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    	{
        	LOG("Failed to initialize GLAD !")
    	}
		LOG("load glad succeed")
	}
	~Window()
	{
	    glfwTerminate();
		LOG("delete window")
	}
	bool IsOpen()
	{
		return !glfwWindowShouldClose(glfw_window);
	}
	void Tick()
	{
		deltaTime = glfwGetTime() - time;
		time = glfwGetTime();

		for (int i = 0; i < GLFW_KEY_LAST; i++)
		{
			Key &k = keys[i];
			k.pressDown = false;
			k.liftUp = false;
			if (k.pressing)
				k.pressDur += deltaTime;
			else
				k.pressDur = 0;
		}

		glfwSwapBuffers(glfw_window);
        glfwPollEvents();
	}
	void FramebufferSizeCallback(GLFWwindow* w, int width, int height)
	{
    	glViewport(0, 0, width, height);
	}
	void KeyCallback(GLFWwindow* w, int id, int scancode, int action, int mods)
	{
		Key& k = keys[id];

		if (action == 1) // event key pressDown
		{
			k.pressing=true;
			k.pressDown=true;
			k.downDeltaTime = glfwGetTime() - k.downTime;
			k.downTime = glfwGetTime();
		}
		else if (action == 0) // event key liftUp
		{
			k.pressing=false;
			k.liftUp=true;
		}	
	}
	void DropCallback(GLFWwindow* w, int path_count, const char* paths[])
	{
		for (int i = 0; i < path_count; i++)
		{
			string path = paths[i];
			dropPaths.push_back(path);
		}
	}
	void Close()
	{
		glfwSetWindowShouldClose(glfw_window, true);
	}
};
Window* GetCurrentContext()
{
    return (Window*)glfwGetWindowUserPointer(glfwGetCurrentContext());
}
#endif
