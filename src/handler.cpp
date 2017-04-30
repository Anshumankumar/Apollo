#include <handler.hpp>
#include <functional>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void ApolloHandler::keyCalls(){

     std::cout << "Got the callBack\n";
}
void ApolloHandler::setUpGl()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
}
void ApolloHandler::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
     auto handler = (ApolloHandler*)(glfwGetWindowUserPointer(window));
     handler->keyCalls();
}

ApolloHandler::ApolloHandler()
{
//    renderable = new FileRender("../models/test.raw");
    //renderable = new Triangle();
    Generator * generator = new PartEllipsoid(0.7,0.7,0.7,1.0);
    renderable = new GeneratorRender(generator);
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(2);
    window = glfwCreateWindow(640, 640, "Apollo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(3);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, ApolloHandler::keyCallBack);

    GLenum err=glewInit();
    if(err!=GLEW_OK)
    {
        std::cerr << "glewInit failed: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }
    setUpGl();
    std::vector<ShaderUtil::sprop>shaderList;
    shaderList.push_back(ShaderUtil::sprop("../shaders/vshader.glsl",GL_VERTEX_SHADER));
    shaderList.push_back(ShaderUtil::sprop("../shaders/fshader.glsl",GL_FRAGMENT_SHADER));
    ShaderUtil shaderUtil(shaderList);
    program=shaderUtil.getProgram();
    renderable->initialize(program);
    renderable->setPoints();
       
    GLint vpos_location = glGetAttribLocation(program, "vPosition");
    GLint vcol_location = glGetAttribLocation(program, "vColor");



    glVertexAttribPointer(vpos_location, 4, GL_FLOAT, GL_FALSE,
            sizeof(Point), (void*) 0);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE,
            sizeof(Point), (void*) (sizeof(float) * 4));
    glEnableVertexAttribArray(vcol_location);
    glUseProgram(program);


   




}

void ApolloHandler::run()
{
    while (!glfwWindowShouldClose(window))
    {
        singleLoop();
    }
    glfwTerminate();
}

void ApolloHandler::singleLoop()
{
    glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);    
    renderable->render();
    glfwSwapBuffers(window);
    glfwPollEvents();

}
