#include <handler.hpp>

using namespace apollo;
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
//   Generator * generator = new Circle(0.7);
    Generator * cube = new Cube(0.7);
    Generator * triangle = new Triangle();
    Generator * frustum = new Frustum(0.8,0.4,0.5);
    Generator * sphere = new PartEllipsoid(0.5,0.5,0.5,1);
    frustum->scale(1.0,1.0,1.0);
    frustum->rotate(-M_PI/2,0.0,0.0);
    frustum->translate(0.0,-0.2,0.0);
    cube->translate(-0.35,-0.35,-0.35);
//    sphere->translate(0.0,-0.50,0);
    cube->scale(1,1,1);
    std::vector <Generator*> generators;
 //   generators.push_back(frustum);
  //  generators.push_back(cube);
 //  generators.push_back(sphere);
    generators.push_back(triangle);
    Generator * generator = new Combiner(generators);
//    renderable = new FileRender("../models/test.raw");
   // renderable = new Triangle();
   // Generator * generator = new PartEllipsoid(0.7,0.7,0.7,1.0);
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
    GLint vnor_location = glGetAttribLocation(program, "vNormal");
    GLint vtex_location = glGetAttribLocation(program, "vTex");



    glVertexAttribPointer(vpos_location, 4, GL_FLOAT, GL_FALSE,
            sizeof(Point), (void*) 0);
    glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE,
            sizeof(Point), (void*) (sizeof(float) * 4));
    glVertexAttribPointer(vnor_location, 3, GL_FLOAT, GL_FALSE,
            sizeof(Point), (void*) (sizeof(float) * 8));
    glVertexAttribPointer(vtex_location, 2, GL_FLOAT, GL_FALSE,
            sizeof(Point), (void*) (sizeof(float) * 11));

    glEnableVertexAttribArray(vnor_location);
    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray(vtex_location);
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
