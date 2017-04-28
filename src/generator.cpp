#include <generator.hpp>

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

Point PartEllipsoid::getPointEllipse( float a, float b, float c, float theta, float phi)
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

Circle::Circle(float radius)
{
    int k = 0;
    float theta;
    float theta1;
    for (int i = 0; i < 180; i++)
    {
        theta = i*2*M_PI/180;
        theta1 = (i+1)*2*M_PI/180;
        //points[k++]= getPointCircle(radius,theta);
        Point point1;
        Point point2;
        Point point3;
        //points[k++]= getPointCircle(radius,theta);
        point1.cx =  fabs(sin(0.5 *theta));
        point1.cy =  fabs(sin(0.5 *theta));
        point1.cz =  0.5;
        point1.a =   1.0;
        point1.x = radius*cos(theta);
        point1.y = radius*sin(theta);
        point1.z = 0;
        point1.w = 1;
        points.push_back(point1);

        point2.cx =  fabs(sin(0.5 *theta));
        point2.cy =  fabs(sin(0.5 *theta));
        point2.cz =  0.5;
        point2.a =   1.0;
        point2.x = radius*cos(theta1);
        point2.y = radius*sin(theta1);
        point2.z = 0;
        point2.w = 1;
        points.push_back(point2);

        //points[k++]= getPointCircle(radius,theta1);
        point3.cx =  fabs(sin(0.5 *theta));
        point3.cy =  fabs(sin(0.5 *theta));
        point3.cz =  0.5;
        point3.a =   1.0;
        point3.x = 0.0;
        point3.y = 0.0;
        point3.z = 0;
        point3.w = 1;
        points.push_back(point3);
    }
}

Cube::Cube()
{
    Point point1={0.0,0.0,0.0,1.0,0.3,0.7,0.5,1.0};
    Point point2={0.0,0.7,0.0,1.0,0.4,0.2,0.4,1.0};
    Point point3={0.7,0.0,0.0,1.0,0.2,0.3,0.6,1.0};
    Point point4={0.7,0.7,0.0,1.0,0.8,0.2,0.4,1.0};
    Point point5={0.0,0.0,0.7,1.0,0.2,0.6,0.5,1.0};
    Point point6={0.7,0.0,0.7,1.0,0.6,0.7,0.1,1.0};
    Point point7={0.0,0.7,0.7,1.0,0.6,0.4,0.2,1.0};
    Point point8={0.7,0.7,0.7,1.0,0.3,0.7,0.5,1.0};
    vertices.push_back(point1);
    vertices.push_back(point2);
    vertices.push_back(point3);
    vertices.push_back(point4);
    vertices.push_back(point5);
    vertices.push_back(point6);
    vertices.push_back(point7);
    vertices.push_back(point8);
    addSquarePoint(0,1,2,3,vertices);
    addSquarePoint(4,5,6,7,vertices);
    addSquarePoint(0,4,1,6,vertices);
    addSquarePoint(2,5,3,7,vertices);
    addSquarePoint(1,3,6,7,vertices);
    addSquarePoint(0,2,4,5,vertices);
}

void Cube::addSquarePoint(int a, int b, int c, int d, std::vector <Point> vertices)
{
    Point point1,point2,point3,point4,point5,point6;
    points.push_back(vertices[a]);
    points.push_back(vertices[b]);
    points.push_back(vertices[c]);
    points.push_back(vertices[b]);
    points.push_back(vertices[c]);
    points.push_back(vertices[d]);

}
