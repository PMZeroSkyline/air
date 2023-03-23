#include "engine/GL.h"
#include "engine/Window.h"
#include "engine/Mesh.h"
#include "engine/Transform.h"
#include "engine/Profiler.h"

int main()
{    
    CDAppleResourcesDir();

    Window window(420,280,"GPA metric values calculator");
    Profiler profiler;


    while (window.IsOpen())
    {
        window.TickEditor();

        profiler.Draw();
        

        if (window.keys[GLFW_KEY_ESCAPE].pressDown)
        {
            window.Close();
        }

        glClearColor(.2,.4,.8,1.);
        glClear(GL_COLOR_BUFFER_BIT);
        

        window.Tick();
    }

    return 0;
}
