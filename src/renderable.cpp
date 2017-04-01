#include <renderable.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

void Renderable::initialize(GLuint program)
{
    vao=0;vbo=0;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    this->program = program;
    tmat[0][0]=1;
    tmat[1][1]=1;
    tmat[2][2]=1;
    tmat[3][3]=1;
}

  
void Renderable::render()
{
    GLuint gScaleLocation = glGetUniformLocation(program, "gScale");
    glUniform1f(gScaleLocation, sin(scale));
    GLuint matLocation = glGetUniformLocation(program, "transMat");

    modifyPoints();
    glUniformMatrix4fv(matLocation, 1, GL_TRUE, &tmat[0][0]);
    glBufferData (GL_ARRAY_BUFFER,points.size()*sizeof(Point),
            &points[0], GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, points.size());
//    glDrawArrays(GL_LINES, 0, points.size());
//    glDrawArrays(GL_TRIANGLES, 0, points.size());

}

void  Renderable::modifyPoints(){}

void Triangle::setPoints()
{
    points.resize(0);
    points.push_back({0.0,0.7,0.1,1.0,1.0,1.0,1.0,1.0});
    points.push_back({0.7,-0.7,0.1,1.0,0.5,0.5,0.0,1.0});
    points.push_back({-0.7,-0.7,0.1,1.0,0.0,1.0,1.0,1.0});
}
 
void  Triangle::modifyPoints()
{
    
    scale += 0.01f;
    tmat[0][0]=cos(scale);
    tmat[2][0]=-sin(scale);
    tmat[0][2]=sin(scale);
    tmat[2][2]=cos(scale); 

    if (points[0].cx > 0.99) points[0].cx =0;
    if (points[0].cy > 0.99) points[0].cy =0;
    if (points[0].cz > 0.99) points[0].cz =0;
    points[0].cx= points[0].cx +0.0005;
    points[0].cy= points[0].cy +0.0003;
    points[0].cz= points[0].cz +0.0002;

    if (points[1].cx > 0.99) points[1].cx =0;
    if (points[1].cy > 0.99) points[1].cy =0;
    if (points[1].cz > 0.99) points[1].cz =0;
    points[1].cx= points[1].cx +0.001;
    points[1].cy= points[1].cy +0.0025;
    points[1].cz= points[1].cz +0.00035;

    if (points[2].cx > 0.99) points[2].cx =0;
    if (points[2].cy > 0.99) points[2].cy =0;
    if (points[2].cz > 0.99) points[2].cz =0;
    points[2].cx= points[2].cx +0.0015;
    points[2].cy= points[2].cy +0.0012;
    points[2].cz= points[2].cz +0.00045;
}

GeneratorRender::GeneratorRender(Generator * generator)
{
    this->generator = generator;
}

void GeneratorRender::setPoints()
{
    points = generator->getPoints();
}

void GeneratorRender::modifyPoints()
{
    scale += 0.01f;
    tmat[0][0]=cos(scale);
    tmat[2][0]=-sin(scale);
    tmat[0][2]=sin(scale);
    tmat[2][2]=cos(scale); 
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
