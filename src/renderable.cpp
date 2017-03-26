#include <renderable.hpp>
#include <iostream>

void Renderable::setPoints()
{
    points.push_back({0.0,0.7,0.1,1.0,1.0,1.0,1.0,1.0});
    points.push_back({0.7,-0.7,0.1,1.0,0.5,0.5,0.0,1.0});
    points.push_back({-0.7,-0.7,0.1,1.0,0.0,1.0,1.0,1.0});
    vao=0;vbo=0;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
 

}

void Renderable::initVboVao()
{
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


        glBufferData (GL_ARRAY_BUFFER,points.size()*sizeof(Point),
            &points[0], GL_STATIC_DRAW);

}
