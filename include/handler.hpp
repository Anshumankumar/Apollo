#ifndef APOLLO_HANDLER_HPP
#define APOLLO_HANDLER_HPP

#include <cstdlib>
#include <shaderHandler.hpp>
#include <renderable.hpp>
namespace apollo
{
class ApolloHandler
{
    GLuint program;
    void keyCalls(int, int);
    public:
    ApolloHandler();
    GLFWwindow *window;
    Renderable *renderable;
    void run();
    void singleLoop();
    void setUpGl();
    static void keyCallBack(GLFWwindow*,int,int,int,int);
};
}

#endif //APOLLO_HANDLER_HPP
