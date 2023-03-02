#include <iostream>
#include "engine/gl.h"
#include "engine/window.h"
#include "engine/model.h"
#include "engine/utility.h"
#include "engine/node.h"

int main()
{
    window w(800, 600, "test");

    mesh quad = make_quad();
    string vs, fs;
    string_from_file("../res/vs.glsl", vs);
    string_from_file("../res/fs.glsl", fs);
    gl_shader glvs(GL_VERTEX_SHADER), glfs(GL_FRAGMENT_SHADER);
    glvs.compile(vs.c_str());
    glfs.compile(fs.c_str());
    gl_program glprog;
    glprog.link(glvs.id, glfs.id);
    glUseProgram(glprog.id);
    gl_mesh glm;
    glm.bind();
    gl_vao_data(quad.p, quad.n, quad.t, quad.uv);
    gl_ebo_data(quad.ids);

    while (get_current_context()->is_open())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(glGetUniformLocation(glprog.id, "iTime"), get_current_context()->tm);
        glDrawElements(GL_TRIANGLES, quad.ids.size(), GL_UNSIGNED_INT, 0);

        if (get_current_context()->keys[GLFW_KEY_ESCAPE].down)
            get_current_context()->close();

        get_current_context()->tick();
    }

    return 0;
}