#ifndef APOLLO_HANDLER_HPP
#define APOLLO_HANDLER_HPP

#include <cstdlib>
#include <shaderHandler.hpp>
#include <renderable.hpp>

class ApolloHandler
{
    GLuint program;
    public:
    ApolloHandler();
    GLFWwindow *window;
    Renderable *renderable;
    void run();
    void singleLoop();
    void setUpGl();
    static void keyCallBack(GLFWwindow*,int,int,int,int);
    static ApolloHandler * instance;
};


#endif //APOLLO_HANDLER_HPP
