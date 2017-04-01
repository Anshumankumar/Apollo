#include <handler.hpp>

std::vector<GLuint> ShaderHandler::shaderList;


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


ApolloHandler::ApolloHandler()
{
    //renderable = new FileRender("../models/test.raw");
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


    GLenum err=glewInit();
    if(err!=GLEW_OK)
    {
        std::cerr << "glewInit failed: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ShaderHandler vShader("../shaders/vshader.glsl",GL_VERTEX_SHADER);
    ShaderHandler fShader("../shaders/fshader.glsl",GL_FRAGMENT_SHADER); 
    vShader.compileShader();
    fShader.compileShader();
    program=ShaderHandler::useProgram();
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
