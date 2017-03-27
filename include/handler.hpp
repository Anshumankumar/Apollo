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
};


#endif //APOLLO_HANDLER_HPP
