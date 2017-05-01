#ifndef SHADER_HANDLER_HPP
#define SHADER_HANDLER_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <utility>
namespace apollo
{
class ShaderHandler
{
    GLuint shader;
    GLenum shaderType;
    char *buffer;
    public:
    ShaderHandler(std::string filename, GLenum shaderType)
    {
        this->shaderType = shaderType;
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

        glShaderSource(shader, 1, (const char **)&buffer, NULL);
        glCompileShader(shader);
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {

            GLchar infoLog[2048];
            glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
            std::cerr << shaderType << " " << infoLog << std::endl; 
            exit(2);
        }
        return shader;
    } 
};

class ShaderUtil
{
    std::vector<GLuint> shaderList;
    public:
    using sprop = std::pair<std::string,GLenum>;
    ShaderUtil(std::vector<sprop> pairs)
    {
        for(auto pair:pairs)
        {
            ShaderHandler shader(pair.first,pair.second); 
            shaderList.push_back(shader.compileShader());
        }
    }

    GLuint getProgram()
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
        for(auto &shader:shaderList)
            glDetachShader(program, shader);
        return program;
    }
};
}
#endif //SHADER_HANDLER_HPP
