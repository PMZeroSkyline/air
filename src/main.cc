#include "engine/GL.h"
#include "engine/Window.h"
#include "engine/Mesh.h"

int main()
{    
    CDAppleResourcesDir();

    Window window;

    while (GetCurrentContext()->IsOpen())
    {
        GetCurrentContext()->TickEditor();

        if (GetCurrentContext()->keys[GLFW_KEY_ESCAPE].pressDown)
        {
            GetCurrentContext()->Close();
        }

        glClearColor(.2,.4,.8,1.);
        glClear(GL_COLOR_BUFFER_BIT);

        GetCurrentContext()->Tick();
    }
    

    return 0;
}
