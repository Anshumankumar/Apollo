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
        GPoint * p= (GPoint *)(&point);
        p->x = mat*p->x;
    }
}

void Generator::translate(float x, float y, float z)
{
    glm::mat4 mat(1.0);
    mat=glm::translate(mat,glm::vec3(x,y,z));
    for (auto &point:points)
    {
        GPoint * p= (GPoint *)(&point);
        p->x = mat*p->x;
    }

}
void Generator::scale(float sx, float sy, float sz)
{
    glm::mat4 identity(1.0);
    glm::mat4 mat=glm::scale(identity,glm::vec3(sx,sy,sz));
    for (auto &point:points)
    {
        GPoint * p= (GPoint *)(&point);
        p->x = mat*p->x;
    }
}

Point getPointEllipse( float a, float b, float c, float theta, float phi, float zShift)
{
    Point point;
    point.cx =  fabs(sin(0.5 *phi));
    point.cy =  fabs(sin(0.5 *theta));
    point.cz =  0.5;
    point.a =   1.0;
    point.x = a*cos(theta)*sin(phi);
    point.y = b*sin(theta)*sin(phi);
    point.z = c*cos(phi)+zShift;
    point.w = 1;
    return point;
}

std::vector<Point> Generator::getPoints()
{
    return points;
}

PartEllipsoid::PartEllipsoid(float a, float b, float c, float factor)
{
    int noOfPoints1 = a*200;
    int noOfPoints2 = b*100;
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
            points[k++]= getPointEllipse(a,b,c,theta1,phi1);
            points[k++]= getPointEllipse(a,b,c,theta1,phi2);
            points[k++]= getPointEllipse(a,b,c,theta2,phi1);
            points[k++]= getPointEllipse(a,b,c,theta1,phi2);
            points[k++]= getPointEllipse(a,b,c,theta2,phi1);
            points[k++]= getPointEllipse(a,b,c,theta2,phi2);
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
        points.push_back(getPointEllipse(radius,radius,0,theta,M_PI/2));
        points.push_back(getPointEllipse(radius,radius,0,theta1,M_PI/2));
        points.push_back(getPointEllipse(0,0,0,0,0));
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
        vertices.push_back({m,n,o,1,m,n,o,1});
    }
    for (int j=0; j<3;j++)
    {
        addSquarePoint(1<<j,vertices);
    } 
}

void Cube::addSquarePoint(int index, std::vector<Point> & vertices)
{
    int indexes[] ={0,1,2,1,2,3};
    int positive[4]; int negative[4];
    int pi=0; int ni=0;
    for(int i=0;i<8;i++)
    {
        if (i&index) positive[pi++] = i; 
        else negative[ni++] = i; 
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
        points.push_back(getPointEllipse(rTop,rTop,0,theta1,M_PI/2));
        points.push_back(getPointEllipse(rTop,rTop,0,theta2,M_PI/2));
        points.push_back(getPointEllipse(rBot,rBot,0,theta2,M_PI/2,height));
        points.push_back(getPointEllipse(rBot,rBot,0,theta2,M_PI/2,height));
        points.push_back(getPointEllipse(rBot,rBot,0,theta1,M_PI/2,height));
        points.push_back(getPointEllipse(rTop,rTop,0,theta1,M_PI/2));

    }
    std::vector<Point> tp=topCircle.getPoints();  
    std::vector<Point> bp=bottomCircle.getPoints(); 
    for(auto &point:bp)point.z=height;
    points.insert(points.end(),tp.begin(),tp.end());
    points.insert(points.end(),bp.begin(),bp.end());
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
