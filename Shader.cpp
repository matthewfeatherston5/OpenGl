#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Shader.h"
#include "Renderer.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};


Shader::Shader(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];// 0 =vertex shader 1 = fragment shader

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1

    };

    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                //set mode to vertex
                type = ShaderType::VERTEX;

            }
            else if (line.find("fragment") != std::string::npos)
            {
                //Set mode to fragment 
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    //Debugging purposes 


    return { ss[0].str(), ss[1].str() };
}





unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = (glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    //ERROR HANDLING 
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile: " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
            std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}




unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); //Creates the vertex shader
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); //Creates the fragment shader

    //attach shaders to program and vildates prgm
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    //Delete our shaders and free up memory 
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }


    GLCall(unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        std::cout << "Warning uniform" << name << "'doesn't exist!'" << std::endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}