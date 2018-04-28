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
    Generator * sq1 = new Square(0.09);
    sq1->scale(1.0,6);
    Generator * sq2 = new Square(0.09);
    sq2->scale(3.0,1.0);
    sq2->translate(0.0,0.6);
    Generator * sq3 = new Square(0.09);
    sq3->scale(3.0,1.0);
    sq3->translate(0.0,-0.6);
    generators.push_back(sq2); 
    generators.push_back(sq3); 
    generators.push_back(sq1); 
    Generator *I = new Combiner(generators);


//T
    generators.clear();
    generators.push_back(sq2); 
    generators.push_back(sq1); 
    Generator *T = new Combiner(generators);




//Y 
    generators.clear();
    sq1 = new Square(0.08);
    sq1->scale(1.0,5);
    sq1->translate(0,-0.25);
    sq2 = new Square(0.09);
    sq2->scale(1,3.5);
    sq2->shearX(-0.4);
    sq3 = new Square(0.09);
    sq3->scale(1,3.5);
    sq3->shearX(0.4);
    sq2->translate(-0.13,0.3);
    sq3->translate(0.13,0.3);
    generators.push_back(sq2); 
    generators.push_back(sq3); 
    generators.push_back(sq1); 
 
    
    Generator *Y = new Combiner(generators);


    
    
    
    
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
//O
    Generator *O = new CCircle(0.34,0.22,1.0);
    O->scale(1.0,2.0);

//G
    generators.clear();
    Generator *cr = new CCircle(0.40,0.26,0.72);
    cr->rotate(0,0,1.50);
    cr->scale(1.0,1.7);

    sq2 = new Square(0.09);
    sq2->scale(2.0,1.0);
    sq2->translate(0.21,-0.1);

    generators.push_back(sq2); 
    generators.push_back(cr); 
    Generator *G = new Combiner(generators);
    G->scale(0.9,1.0);





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
    Y->scale(0.5,0.4);
    O->scale(0.4,0.4);
    G->scale(0.4,0.4);
    I->scale(0.4,0.35);
    T->scale(0.4,0.40);
    A->scale(0.3,0.33);

    Y->translate(-0.8);
    O->translate(-0.53);
    G->translate(-0.23);
    I->translate(0.03);
    T->translate(0.30,-0.03);
    A->translate(0.58);
    
    std::vector<Generator*> fullStack;
    //fullStack.push_back(Y);
    fullStack.push_back(O);
    //fullStack.push_back(G);
    //fullStack.push_back(I);
    //fullStack.push_back(T);
    //fullStack.push_back(A);
    generator = new Combiner(fullStack);
}
}
