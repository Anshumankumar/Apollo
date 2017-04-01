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
    int noOfPoints1 = a*180;
    int noOfPoints2 = b*90;
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
