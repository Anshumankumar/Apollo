#ifndef SHADER_HANDLER_HPP
#define SHADER_HANDLER_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ShaderHandler
{
    GLuint shader;
    char *buffer;
    public:
    static std::vector<GLuint> shaderList;
    ShaderHandler(std::string filename, GLenum shaderType)
    {
        std::ifstream stream(filename,std::ifstream::in);
        if (stream.is_open())
        {
            stream.seekg(0,stream.end);
            int len = stream.tellg();
            stream.seekg(0,stream.beg);
            buffer = new char[len];
            stream.read(buffer,len);
            stream.close();
        }
        else
        {
            std::cerr << "Error in reading file " << filename<<"\n";
            exit(2);
        }
        shader = glCreateShader(shaderType);
    }        

    GLuint compileShader()
    {

        glShaderSource(shader, 1, &buffer, NULL);
        glCompileShader(shader);
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            std::cerr << "HULA";
            exit(2);
        }
        shaderList.push_back(shader);
    } 

    static GLuint useProgram()
    {
        GLuint program = glCreateProgram();

        for(auto &shader:shaderList)
        {
            glAttachShader(program, shader);
        }
        glLinkProgram(program);

        GLint status;
        glGetProgramiv (program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
            std::cerr<<"GLSL Linker failure: "<<strInfoLog<<std::endl;
            delete[] strInfoLog;
        }
//        for(auto &shader:shaderList)
  //          glDetachShader(program, shader);
        return program;
    }
};

#endif //SHADER_HANDLER_HPP
