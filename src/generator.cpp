#include <generator.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp> 
#include <ctime>
namespace apollo
{

#define GOLDEN_RATIO  (1+ sqrt(5))/2

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
    color = glm::vec4(0.0,1.0,0.0,1.0);
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
    char X = 1 <<0 ;char  Y = 1 <<1; char Z = 1 << 2;
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

glm::vec3 getRandomNormal(glm::vec4 vec){
    //Generate vector in plane
    float alpha = (2.0f*M_PI*rand())/RAND_MAX;
    glm::vec3 v(cos(alpha), sin(alpha),0.0);
    float len =  glm::length(glm::vec2(vec.y,vec.z));
    if (len == 0){
        return glm::vec3(v.x, 0,v.y);    
    }
    if (vec.z != 0.0) {
        len = abs(vec.z)*len/vec.z;
    }

    float theta = atan2(vec.x, len);
    float phi = asin(vec.y/len);
    glm::vec3 v2 =  glm::rotateY(v,  theta);
    v2 =  glm::rotateX(v2,  -phi);
    return v2;
}


Icosahedron::Icosahedron(float len){
    std::vector<Point> vertices;
    float A=0; float B=len/2 ; float C = GOLDEN_RATIO * len/2;
    char X = 1 <<0; char  Y = 1 <<1; char Z = 1 << 2;
    glm::vec4 clr(0.5,1.0,0.5,1);
    
    float m[3];
    for (int i=0; i < 12; i++){
        m[0]=A;
        m[1] = B* ((i&X) ? -1 : 1) ;
        m[2] = C* ((i&Y) ? -1 : 1);
        int ri =  ((i >> 2) & 3);
        glm::vec4 vec(m[(0 + ri)%3], m[(1 + ri)%3], m[(2 + ri)%3],1);
        Point  p({vec,clr});
        p.rn =  getRandomNormal(p.x);
        vertices.push_back(p);
    }

    std::vector<int[3]> indexes(20);
    for (int i=0; i<12; i++){
        indexes[i][0] = i;
        indexes[i][1] = ((i&1) << 1)   + (((((i >> 2)&3) +1) % 3) << 2);
        indexes[i][2] = indexes[i][1] +1 ;
    }

    for (int i=0;i < 8;  i++){
        indexes[i+12][0] = ((i&6) >> 1);
        indexes[i+12][1] = 4 +  (i&3);
        indexes[i+12][2] = 8 + ((i&4)>>2) +   ((i&1) << 1);
    }

    for (int i=0;i<20;i++){
        Point p[3];
        for (int j=0; j <3; j++){ p[j] =  vertices[indexes[i][j]]; }
        glm::vec3 normal2=  glm::triangleNormal(glm::vec3(p[0].x), glm::vec3(p[1].x), glm::vec3(p[2].x));
        glm::vec3 normal  = (glm::dot(normal2, glm::vec3(p[0].x)) > 0? 1.0f :-1.0f)* normal2;
        if (glm::dot(normal2, glm::vec3(p[0].x)) > 0){ 
            for (int j=0; j <3; j++){p[j].n = normal; points.push_back(p[j]);}
        }
        else {
            for (int j=2; j >=0; j--){p[j].n = normal; points.push_back(p[j]);}
        }
    }

}

Point getPoint(glm::vec4 vec, float radius, glm::vec4 clr){
    glm::vec4 v2 = radius*vec/float(sqrt(glm::dot(glm::vec3(vec), glm::vec3(vec))));
    v2[3] =  1;
    Point p({v2,clr ,glm::vec3(v2 )}) ;
    p.rn  = getRandomNormal(p.x);
    return p;
}

glm::vec2 getTex(Point point, float radius){
    float theta = atan2(point.x.z,point.x.x);
    float u =  0.5 + theta/(2*M_PI);
    float v  = 0.5 - asin(point.x.y/radius)/M_PI;
    return glm::vec2(u,v);
}

Sphere::Sphere(float radius, int iterCount){
    srand(time(NULL));
    //This generate Sphere from icosahedron
    float slen = 2*radius/(sqrt(GOLDEN_RATIO*GOLDEN_RATIO+1));
    Icosahedron ico(slen);
    std::vector<Point> tmpPoints = ico.getPoints();
    glm::vec4 clr(0.0,1.0,1.0,1);

    for (int t=0; t<iterCount; t++)
    {
        points = std::vector<Point>();
        for (auto & point:tmpPoints){
            point.n = glm::vec3(point.x) ;
            point.c = clr ;
        }
        glm::vec4 a,b;
        Point m[3];
        for (int i=0; i<tmpPoints.size()/3; i++){
            for (int j=0; j < 3; j++){
                a = tmpPoints[3*i + ((j+1)%3) ].x ;
                b = tmpPoints[3*i + ((j+2)%3)].x; 
                m[j] = getPoint((a+b)/2.0f,radius,clr);
                points.push_back(m[j]);
            }
            for (int j=0; j<3; j++){
                points.push_back(tmpPoints[3*i+ j]);
                points.push_back(m[(j+1)%3]);
                points.push_back(m[(j+2)%3]);
            }
        }
        tmpPoints = points;
    }
    points = tmpPoints;

    for (int i=0; i < points.size()/3;i++){
        glm::vec3 normal=  glm::triangleNormal(glm::vec3(points[3*i].x), glm::vec3(points[3*i+1].x), glm::vec3(points[3*i+2].x));
        if (glm::dot(normal, glm::vec3(points[3*i].x)) < 0){
            Point p = points[3*i];
            points[3*i] =  points[3*i+1];
            points[3*i+1] = p;
        }

    }




    //UV Mapping
    for (int i=0;i<points.size()/3;i++){
        glm::vec2 t[3];
        t[0] = getTex(points[3*i],radius);
        t[1] = getTex(points[3*i+1],radius);
        t[2] = getTex(points[3*i+2],radius);
        float mx = std::max(std::max(t[0].x, t[1].x), t[2].x);
        float mn = std::min(std::min(t[0].x, t[1].x),t[2].x);
        for (int j=0;j<3;j++){
            if (mx-mn > 0.5){
                if (t[j].x <= 0.5){
                    t[j].x = 1.0 +  t[j].x;
                }
            }
            t[j].x = (1/1.5)*t[j].x;
            points[3*i+j].t  = t[j]; 
        }
    }



}


Frustum::Frustum(float rTop, float rBot, float height)
{
    Circle topCircle(rTop);
    Circle bottomCircle(rBot);
    color = glm::vec4(1.0,1.0,0.0,1.0);
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
