#ifndef WINDOW_H
#define WINDOW_H

#include "SDK/STL/STL.h"
#include "Core/Log/Log.h"
#include "SDK/OpenGL/CppOpenGL.h"
#include <GLFW/glfw3.h>
#include "Core/Math/vec2.h"
#include "Key.h"

struct CursorPos
{
	vec2 pos = vec2(-1.f);
	vec2 lastPos = vec2(-1.f);
	vec2 deltaPos = vec2(-1.f);
};
void SetupOpenGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG("Failed to initialize GLAD !")
	}
}
class Window
{
public:
	GLFWwindow* glfwWindow;
	float time = 0, deltaTime = 0;
	vector<Key> keys;
	CursorPos mouseCursor;
	vector<string> dropPaths;
	int frameRate = 0;
	int lastSecond = 0;

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
		glfwWindow = glfwCreateWindow(w, h, title, NULL, NULL);
		if (glfwWindow == NULL)
		{
			LOG("Failed to create GLFW window !")
			glfwTerminate();
		}

		glfwMakeContextCurrent(glfwWindow);
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetWindowUserPointer(glfwWindow, this);
		SetupOpenGL();
        
		// MacOS vscode debug has bug : use vector reserve or use const size array mad by out of memory !
		keys.resize(GLFW_KEY_LAST);
		glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow *w, int width, int height){
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(w))->FramebufferSizeCallback(w, width, height);
		});
		glfwSetKeyCallback(glfwWindow, [](GLFWwindow* w, int key, int scancode, int action, int mods){
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(w))->KeyCallback(w, key, scancode, action, mods);
		});
		glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* w, int button, int action, int mods){
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(w))->MouseButtonCallback(w, button, action, mods);
		});
		glfwSetDropCallback(glfwWindow, [](GLFWwindow* w, int path_count, const char* paths[]){
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(w))->DropCallback(w, path_count, paths);
		});
		glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* w, double xpos, double ypos){
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(w))->CursorPosCallback(w, xpos, ypos);
		});
	}
	~Window()
	{
	    glfwTerminate();
	}
	
	// Utility
	bool IsOpen()
	{
		return !glfwWindowShouldClose(glfwWindow);
	}
	void Close()
	{
		glfwSetWindowShouldClose(glfwWindow, true);
	}
	ivec2 GetFrameBufferSize()
	{
		int w, h;
		glfwGetFramebufferSize(glfwWindow, &w, &h);
		return ivec2(w, h);
	}
	ivec2 GetWindowSize()
	{
		int w, h;
		glfwGetWindowSize(glfwWindow, &w, &h);
		return ivec2(w, h);
	}
	bool GetCursor()
	{
		return glfwGetInputMode(glfwWindow, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
	}
	void SetCursor(bool isNormal)
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, isNormal ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}
		
	// Delta data process
	void Tick()
	{
		// delta time
		deltaTime = glfwGetTime() - time;
		time = glfwGetTime();

		// frame rate
		int currentSecond = floor(time);
		if (currentSecond == lastSecond)
		{
			frameRate++;
		}
		else
		{
			LOG(frameRate)
			frameRate = 0;
			lastSecond = currentSecond;
		}
		
		// mouse delta
		mouseCursor.deltaPos = mouseCursor.pos - mouseCursor.lastPos;
		mouseCursor.lastPos = mouseCursor.pos;

		// key state
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

		// opengl
		glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
	}
	
	// Callback
	void FramebufferSizeCallback(GLFWwindow* w, int width, int height)
	{
		LOG(width << " " << height);
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
	void MouseButtonCallback(GLFWwindow* w, int button, int action, int mods)
	{
		Key& k = keys[button];

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
	void CursorPosCallback(GLFWwindow* w, double xpos, double ypos)
	{
		if (mouseCursor.pos == -1)
		{
			mouseCursor.lastPos = vec2(xpos, ypos);
		}
		mouseCursor.pos = vec2(xpos, ypos);
	}
};
Window* GetCurrentWindowContext()
{
    return (Window*)glfwGetWindowUserPointer(glfwGetCurrentContext());
}
#endif
