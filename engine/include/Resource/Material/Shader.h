#ifndef SHADER_H
#define SHADER_H

#include "SDK/STL/STL.h"
#include "Platform/File/BinFileRead.h"
#include "SDK/OpenGL/CppOpenGL.h"
#include "Core/Math/Math.h"
#include "Core/Container/WeakMap.h"
#include "Core/RTTI/Preprocessor.h"
class Shader
{
public:
    string name;
    GLProgram glProgram;
    void Load(const string &vsPath, const string &fsPath)
    {
        name = "vs:" + vsPath + ",fs:" + fsPath;
        GLShader vsShader(GL_VERTEX_SHADER), fsShader(GL_FRAGMENT_SHADER);
        string vsStr, fsStr;
        vsStr = PreprocessorFile(vsPath);
        fsStr = PreprocessorFile(fsPath);
        vsShader.Compile(vsStr.c_str(), "vs compile failed : " + vsPath + " : \n");
        fsShader.Compile(fsStr.c_str(), "fs compile failed : " + fsPath + " : \n");
        glProgram.Link(vsShader.id, fsShader.id, "link failed : " + vsPath + "," + fsPath + " : \n");
    }
    void Use()
    {
        glUseProgram(glProgram.id);
    }
    void SetBool(const string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(glProgram.id, name.c_str()), (int)value); 
    }
    void SetInt(const string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(glProgram.id, name.c_str()), value); 
    }
    void SetFloat(const string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(glProgram.id, name.c_str()), value); 
    }
    void SetVec2(const string &name, const vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(glProgram.id, name.c_str()), 1, &value[0]); 
    }
    void SetVec2(const string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(glProgram.id, name.c_str()), x, y); 
    }
    void SetVec3(const string &name, const vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(glProgram.id, name.c_str()), 1, &value[0]); 
    }
    void SetVec3(const string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(glProgram.id, name.c_str()), x, y, z); 
    }
    void SetVec4(const string &name, const vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(glProgram.id, name.c_str()), 1, &value[0]); 
    }
    void SetVec4(const string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(glProgram.id, name.c_str()), x, y, z, w); 
    }
    void SetMat4(const string &name, const mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(glProgram.id, name.c_str()), 1, GL_TRUE, &mat[0][0]);
    }
    void BindUniformBlock(const string& shaderUniformBlockName, GLuint uniformBlockIndex)
    {
        unsigned int shaderBlockIndex = glGetUniformBlockIndex(glProgram.id, shaderUniformBlockName.c_str());
        glUniformBlockBinding(glProgram.id, uniformBlockIndex, shaderBlockIndex);
    }
};
WeakMap<Shader> shaderWeakMap;

shared_ptr<Shader> MakeShader(const string& name = "gbuffer")
{
    shared_ptr<Shader> shader = shaderWeakMap.Get(name);
    if (!shader)
    {
        string dir = "game/res/Shader/";
        shader = make_shared<Shader>();
        shader->Load(dir + name + "_vs.glsl", dir + name + "_fs.glsl");
        shaderWeakMap.Set(name, shader);
    }
    return shader;
}
#endif