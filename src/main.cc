#include <iostream>
#include "engine/gl.h"
#include "engine/window.h"
#include "engine/utility.h"
#include "engine/node.h"
#include "engine/mesh.h"
#include "engine/gltf.h"
#include "engine/model.h"

int main()
{    
    cd_apple_dir();

    model m;
    m.load("../res/sponza/scene.gltf");
    cout << "1" << endl;

    return 0;
    window w(800, 600, "test");

    while (get_current_context()->is_open())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (get_current_context()->keys[GLFW_KEY_ESCAPE].down)
            get_current_context()->close();

        get_current_context()->tick();
    }

    return 0;
}