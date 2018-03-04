#include <renderable.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace apollo;
void Renderable::initialize(GLuint program)
{
    vao=0;vbo=0;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    this->program = program;
    tmat = glm::mat4(1.0f);
}

  
void Renderable::render()
{
    GLuint matLocation = glGetUniformLocation(program, "transMat");

    modifyPoints();
    glUniformMatrix4fv(matLocation, 1, GL_FALSE, &tmat[0][0]);
    GLuint drawType = GL_TRIANGLES;
    glDrawArrays(drawType, 0, points.size());

}

void  Renderable::modifyPoints(){}


GeneratorRender::GeneratorRender(Generator * generator)
{
    this->generator = generator;
}

void GeneratorRender::setPoints()
{
    points = generator->getPoints();
    glBufferData (GL_ARRAY_BUFFER,points.size()*sizeof(Point),
            &points[0], GL_STATIC_DRAW);
 
}

void GeneratorRender::modifyPoints()
{
    scale+=0.01f;
    float mean = 0.2*sin(scale);
    float mean2 = 0.6*sin(1.7*scale);
    float mean3 = 0.2*sin(2.7*scale);
    points= generator->getPoints();
    for(auto &point:points)
    {
       // point.c.x=1*exp(-2*(point.x.y-mean)*(point.x.y-mean));
        point.c.x=1*exp(-1*(point.x.x-mean2)*(point.x.x-mean2));
        point.c.y=1*exp(-3*(point.x.x-mean2)*(point.x.x-mean2));
        point.c.z=1*exp(-5*(point.x.y-mean3)*(point.x.y-mean3));
    }
    glBufferData (GL_ARRAY_BUFFER,points.size()*sizeof(Point),
            &points[0], GL_STATIC_DRAW);
 

    tmat=glm::mat4(1.0);    
   // tmat=glm::scale(tmat,glm::vec3(1.0,1.0,1.0));
    glm::mat4 identity(1.0);
    glm::mat4 rot=glm::rotate(identity,scale,glm::vec3(0.0,1.0,0.0));
    glm::mat4 trans = glm::translate(identity,glm::vec3(-0.35,-0.35,-0.35));
    tmat = glm::translate(identity,glm::vec3(0.0,0.5*sin(scale),0.0));
   // tmat = rot;

//    tmat=glm::mat4(1.0);    
   // tmat = identity;
}

FileRender::FileRender(std::string filename)
{
    this->filename = filename;
}

void FileRender::setPoints()
{
    points.resize(0);
    std::ifstream fs(filename);
    while(fs.good())
    {

        std::string  cline;
        fs >> cline;
        if (cline =="") continue;
        Point point;
        Point tempPoint;
        float * pPointer;
        pPointer = (float*)&point;
        std::string delimiter = ",";
        std::string token;
        int len = sizeof(Point)/sizeof(float);
        for (int i =0;i<len;i++)
        {
            auto pos = cline.find(delimiter);
            token = cline.substr(0, pos);
            pPointer[i] = std::stof(token);
            cline.erase(0, pos + delimiter.length());
        }
        points.push_back(point);
    }
    std::cout << points.size() << "\n";
}
