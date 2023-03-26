#include <iostream>
#include "Window.h"
#include "Image.h"
#include "GL.h"

int main()
{

	vec3 e(0.f ,90.f, 0.f);
	quat q = EulerToQuat(radians(e));
	e = degrees(eulerAngles(q));
	cout << e << endl;
	// Result : 0,89.9802,0,

	return 0;

	CDAppleResourcesDir();

	Window window;
	
	while (window.IsOpen())
	{
		window.Tick();
	}

	return 0;
}