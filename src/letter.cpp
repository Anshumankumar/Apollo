#include <letter.hpp>

namespace apollo
{

void  pushTriangles(std::vector<Point> vec, std::vector<Point> &vec2)
{
    vec2.push_back(vec[0]); 
    vec2.push_back(vec[1]); 
    vec2.push_back(vec[2]); 
    vec2.push_back(vec[1]); 
    vec2.push_back(vec[2]); 
    vec2.push_back(vec[3]); 
}
Square::Square(float side)
{
    color = glm::vec4(1.0,1.0,1.0,1.0);
    std::vector<Point> temp;
    temp.push_back({glm::vec4(side,side,0,1),color});
    temp.push_back({glm::vec4(-side,side,0,1),color});
    temp.push_back({glm::vec4(side,-side,0,1),color});
    temp.push_back({glm::vec4(-side,-side,0,1),color});
    pushTriangles(temp,this->points);
}    

Point CCircle::getPointCircle(float r, float theta)
{
    Point p;
    p.c=color;
    p.x=glm::vec4(r*cos(theta), r*sin(theta), 0.0, 1.0);
    p.n=glm::vec3(0,0,1.0);
    return p;
}
CCircle::CCircle(float radius1, float radius2, float part)
{
    color=glm::vec4(1.0,1.0,1.0,1.0);
    for (int i=0;i<part*360;i++)
    {
        float theta1=i*M_PI/180;
        float theta2=(i+1)*M_PI/180;
        std::vector<Point> temp;
        temp.push_back(getPointCircle(radius1,theta1));
        temp.push_back(getPointCircle(radius2,theta1));
        temp.push_back(getPointCircle(radius1,theta2));
        temp.push_back(getPointCircle(radius2,theta2));
        pushTriangles(temp,points);
    }    
}

Final::Final()
{

//I
    std::vector <Generator*> generators;
    Generator * sq1 = new Square(0.12);
    sq1->scale(1.0,6);
    Generator * sq2 = new Square(0.12);
    sq2->scale(3.0,1.0);
    sq2->translate(0.0,0.6);
    Generator * sq3 = new Square(0.12);
    sq3->scale(3.0,1.0);
    sq3->translate(0.0,-0.6);
    generators.push_back(sq2); 
    generators.push_back(sq3); 
    generators.push_back(sq1); 
    Generator *I = new Combiner(generators);

//H
    std::vector <Generator*> generators2;
    sq1 = new Square(0.12);
    sq1->scale(3.0,1.0);
    sq2 = new Square(0.12);
    sq2->scale(1.0,6.0);
    sq2->translate(0.3);
    sq3 = new Square(0.12);
    sq3->scale(1.0,6.0);
    sq3->translate(-0.3);
    generators2.push_back(sq2); 
    generators2.push_back(sq3); 
    generators2.push_back(sq1); 
    Generator * H = new Combiner(generators2);

//A

    std::vector <Generator*> generators3;
    sq1 = new Square(0.12);
    sq1->scale(2.4,1.0);
    sq2 = new Square(0.12);
    sq2->scale(1.0,6.0);
    sq2->shearX(-0.25);
    sq2->translate(0.25);
    sq3 = new Square(0.12);
    sq3->scale(1.0,6.0);
    sq3->shearX(0.25);
    sq3->translate(-0.25);
    generators3.push_back(sq2); 
    generators3.push_back(sq3); 
    generators3.push_back(sq1); 
    Generator *A = new Combiner(generators3);

//C
    Generator *C = new CCircle(0.72,0.48,0.6);
    C->rotate(0,0,1.25);


//R

    std::vector <Generator*> generators4;
    sq1 = new Square(0.10);
    sq1->scale(1.0,7.2);
    sq2 = new Square(0.12);
    sq2->scale(1.0,3.0);
    sq2->shearX(-0.60);
    sq2->translate(0.3,-0.35);
    Generator *ic = new CCircle(0.42,0.20,0.60);
    ic->rotate(0,0,-1.87);
    ic->translate(0.14,0.30);
    generators4.push_back(sq1); 
    generators4.push_back(sq2); 
    generators4.push_back(ic); 
    Generator * R  = new Combiner(generators4);
    R->scale(0.5,0.4);
    I->scale(0.4,0.4);
    C->scale(0.4,0.4);
    H->scale(0.4,0.4);
    A->scale(0.4,0.4);
    R->translate(-0.9);
    I->translate(-0.41);
    C->translate(-0.0);
    H->translate(0.3);
    A->translate(0.72);
    std::vector<Generator*> fullStack;
    fullStack.push_back(R);
    fullStack.push_back(I);
    fullStack.push_back(C);
    fullStack.push_back(H);
    fullStack.push_back(A);
    generator = new Combiner(fullStack);
}
}
