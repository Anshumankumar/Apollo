#include <generator.hpp>
#include <iostream>

namespace apollo
{
void Generator::savePoints(std::string filename)
{
    std::ofstream out(filename);
    float *pPointer;
    for (auto &point:points)
    {
        pPointer = (float*)&point;
        int i=0;
        for(; i<sizeof(Point)/sizeof(float)-1; i++) out << pPointer[i] << ",";
        out << pPointer[i] << std::endl;
    }
    out.close();
}

void Generator::rotate(float ax, float ay, float az)
{
    glm::mat4 mat(1.0);
    mat=glm::rotate(mat,az,glm::vec3(0,0,1));
    mat=glm::rotate(mat,ay,glm::vec3(0,1,0));
    mat=glm::rotate(mat,ax,glm::vec3(1,0,0));
    for (auto &point:points)
    {
        point.x = mat*point.x;
        point.n = glm::mat3(mat)*point.n;
    }
}

void Generator::translate(float x, float y, float z)
{
    glm::mat4 mat(1.0);
    mat=glm::translate(mat,glm::vec3(x,y,z));
    for (auto &point:points)
    {
        point.x = mat*point.x;
    }

}
void Generator::scale(float sx, float sy, float sz)
{
    glm::mat4 identity(1.0);
    glm::mat4 mat=glm::scale(identity,glm::vec3(sx,sy,sz));
    for (auto &point:points)
    {
        point.x = mat*point.x;
    }
}

void Generator::shearX(float mag)
{
    glm::mat4 mat(1.0);
    mat[1][0]=mag;
    for (auto &point:points)
    {
        point.x = mat*point.x;
    }
}


void Generator::getColor()
{
    double x=1,y=1,z=1,a=1;
     if (params.getParam("c") != nullptr) 
    {
        Params p = params.get("c");
        std::cout << "HELLO\n";
        if (p.getParam("x") != nullptr ) x=p.get<double>("x");
        if (p.getParam("y") != nullptr ) y=p.get<double>("y");
        if (p.getParam("z") != nullptr ) z=p.get<double>("z");
        if (p.getParam("a") != nullptr ) z=p.get<double>("a");
    }
    color = glm::vec4(x,y,z,a);

}
glm::vec3 getVec(Params p, float x=0, float y=0, float z=0)
{
    if (p.getParam("x") != nullptr ) x=p.get<double>("x");
    if (p.getParam("y") != nullptr ) y=p.get<double>("y");
    if (p.getParam("z") != nullptr ) z=p.get<double>("z");
    return glm::vec3(x,y,z);
}

void Generator::modify()
{
    if (params.getParam("s") != nullptr) 
    {
        glm::vec3 t = getVec(params.get("s"),1,1,1);
        scale(t.x,t.y,t.z);
    }


    if (params.getParam("r") != nullptr)
    {
         glm::vec3 t = getVec(params.get("r"));
         rotate(t.x,t.y,t.z);
    }
    if (params.getParam("t") != nullptr) 
    {
        glm::vec3 t = getVec(params.get("t"));
        translate(t.x,t.y,t.z);
    }
}

Point getPointFrustum(float radius, float theta, float angle, float zShift=0)
{
    Point point;
    //point.c = color;    
    point.x = glm::vec4(radius*cos(theta), radius*sin(theta),0,1); 
}
Point getPointEllipse( float a, float b, float c, float theta, float phi,glm::vec4 color, float zShift)
{
    Point point;
    point.c = color;
    point.x = glm::vec4(a*cos(theta)*sin(phi), b*sin(theta)*sin(phi), 
                        c*cos(phi)+zShift,1);
    point.n = glm::vec3(cos(theta)*sin(phi),sin(theta)*sin(phi),cos(phi));
    return point;
}

std::vector<Point> Generator::getPoints()
{
    return points;
}

PartEllipsoid::PartEllipsoid(float a, float b, float c, float factor)
{
    int noOfPoints1 = a*100;
    int noOfPoints2 = b*50;
    points.resize(noOfPoints1*noOfPoints2*6);
    float theta1,theta2;
    float phi1,phi2;
    int k = 0;
    for (int i = 0;i < noOfPoints1; i++)
    {
        theta1 = i*2*M_PI/noOfPoints1;
        theta2 = (i+1)*2*M_PI/noOfPoints1;
        for (int j = 0; j < noOfPoints2; j++)
        {
            phi1 = j*M_PI*factor/noOfPoints2;
            phi2 = (j+1)*M_PI*factor/noOfPoints2;
            points[k++]= getPointEllipse(a,b,c,theta1,phi1,color);
            points[k++]= getPointEllipse(a,b,c,theta1,phi2,color);
            points[k++]= getPointEllipse(a,b,c,theta2,phi1,color);
            points[k++]= getPointEllipse(a,b,c,theta1,phi2,color);
            points[k++]= getPointEllipse(a,b,c,theta2,phi1,color);
            points[k++]= getPointEllipse(a,b,c,theta2,phi2,color);
        }
    }
}



Circle::Circle(float radius)
{
    int k = 0;
    float theta;
    float theta1;
    for (int i = 0; i < 180; i++)
    {
        theta = i*2*M_PI/180;
        theta1 = (i+1)*2*M_PI/180;
        points.push_back(getPointEllipse(radius,radius,0,theta,M_PI/2,color));
        points.push_back(getPointEllipse(radius,radius,0,theta1,M_PI/2,color));
        points.push_back(getPointEllipse(0,0,0,0,0,color));
    }
}
Cube::Cube(float a)
{
    std::vector<Point> vertices;
    int X = 1 <<0 ;char  Y = 1 <<1; char Z = 1 << 2;
    float m,n,o;
    for (int i=0;i<8;i++)
    {
        m = a*(bool)(i&X);n = a*(bool)(i&Y);o= a*(bool)(i&Z);
        vertices.push_back({glm::vec4(m,n,o,1),color});
    }
    for (int j=0; j<3;j++)
    {
        addSquarePoint(1<<j,vertices);
    } 
}

void Cube::addSquarePoint(int index, std::vector<Point>  vertices)
{
    glm::vec3 normal = glm::vec3((1<<0)&index,(bool)((1<<1)&index),(bool)((1<<2)&index));
    int indexes[] ={0,1,2,1,2,3};
    int positive[4]; int negative[4];
    int pi=0; int ni=0;
    for(int i=0;i<8;i++)
    {
        if (i&index) {positive[pi++] = i; vertices[i].n=normal;}
        else {negative[ni++] = i; vertices[i].n = -normal;}
    }
    for (auto in:indexes) points.push_back(vertices[positive[in]]);
    for (auto in:indexes) points.push_back(vertices[negative[in]]);
}


Frustum::Frustum(float rTop, float rBot, float height)
{
    Circle topCircle(rTop);
    Circle bottomCircle(rBot);
    for(int i=0;i<180;i++)
    {
        float theta1 = i*2*M_PI/180;
        float theta2 = (i+1)*2*M_PI/180;
        points.push_back(getPointEllipse(rTop,rTop,0,theta1,M_PI/2,color));
        points.push_back(getPointEllipse(rTop,rTop,0,theta2,M_PI/2,color));
        points.push_back(getPointEllipse(rBot,rBot,0,theta2,M_PI/2,color,height));
        points.push_back(getPointEllipse(rBot,rBot,0,theta2,M_PI/2,color,height));
        points.push_back(getPointEllipse(rBot,rBot,0,theta1,M_PI/2,color,height));
        points.push_back(getPointEllipse(rTop,rTop,0,theta1,M_PI/2,color));

    }
    std::vector<Point> tp=topCircle.getPoints();  
    std::vector<Point> bp=bottomCircle.getPoints(); 
    for(auto &point:bp)point.x.z=height;
    points.insert(points.end(),tp.begin(),tp.end());
    points.insert(points.end(),bp.begin(),bp.end());
}

Triangle::Triangle()
{
    params = parseParam("temp.param");
    getColor();
    points.push_back({glm::vec4(0.0,sqrt(3)/3,0.0,1.0),color,glm::vec3(0,0,-1.0)});
    points.push_back({glm::vec4(0.5,-sqrt(3)/6,0.0,1.0),color,glm::vec3(0,0,-1.0)});
    points.push_back({glm::vec4(-0.5,-sqrt(3)/6,0.0,1.0),color,glm::vec3(0,0,-1.0)});
    modify();
}

Combiner::Combiner(std::vector<Generator*> generators)
{
    for (auto generator:generators)
    {
        for (auto point:generator->getPoints())
        {
            points.push_back(point);
        }
    }
}
}
