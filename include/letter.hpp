#ifndef LETTER_HPP
#define LETTER_HPP

#include <generator.hpp>
namespace apollo
{
class Square:public Generator
{
    public:
    Square(float side);
};

class CCircle:public Generator
{
    public:
    CCircle(float radius1, float radius2, float part=1.0);
    Point getPointCircle(float radius, float theta);
};

class Final
{
    public:
    Generator * generator;
    Final();
};
}
#endif
