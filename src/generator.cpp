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

Point getPointEllipse( float a, float b, float c, float theta, float phi)
{
    Point point;
    point.cx =  fabs(sin(0.5 *phi));
    point.cy =  fabs(sin(0.5 *theta));
    point.cz =  0.5;
    point.a =   1.0;
    point.x = a*cos(theta)*sin(phi);
    point.y = b*sin(theta)*sin(phi);
    point.z = c*cos(phi);
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
        points.push_back(getPointEllipse(0.5,0.5,0,theta,M_PI/2));
        points.push_back(getPointEllipse(0.5,0.5,0,theta1,M_PI/2));
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
}
