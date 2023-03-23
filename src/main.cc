#include "engine/GL.h"
#include "engine/Window.h"
#include "engine/Mesh.h"
#include "engine/Transform.h"

int main()
{    
    CDAppleResourcesDir();

    Window window;

    glm::mat4 m(1.f);
    m = glm::rotate(m, glm::radians(90.0f), glm::vec3(1.0f,0,0));
    glm::quat q(m);
    glm::vec3 e = glm::eulerAngles(q);
    e = glm::degrees(e);
    cout << e.x << " " << e.y << " " << e.z << endl;    

    while (GetCurrentContext()->IsOpen())
    {
        GetCurrentContext()->TickUI();

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
