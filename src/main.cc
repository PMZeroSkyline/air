#include "engine/GL.h"
#include "engine/Window.h"
#include "engine/Model.h"

int main()
{    
    CDAppleResourcesDir();

    Window window;

    while (GetCurrentContext()->IsOpen())
    {
        GetCurrentContext()->TickUI();

        if (GetCurrentContext()->keys[GLFW_KEY_ESCAPE].pressDown)
        {
            GetCurrentContext()->Close();
        }

        ImGui::Begin("test");
        ImGui::Text("hahah");
        ImGui::End();

        glClearColor(.2,.4,.8,1.);
        glClear(GL_COLOR_BUFFER_BIT);

        GetCurrentContext()->Tick();
    }
    

    return 0;
}
