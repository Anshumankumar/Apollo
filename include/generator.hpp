#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <utils.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

class Generator
{
    protected:
    std::vector <Point> points;
    
    public:
    std::vector<Point> getPoints();
    void savePoints(std::string filename);
};

class PartEllipsoid:public Generator
{    
    public:

    Point getPointEllipse(float a,float b,float c,float theta, float phi);
    PartEllipsoid(float a,float b, float c, float factor);
}; 

#endif // GENERATOR_HPP
