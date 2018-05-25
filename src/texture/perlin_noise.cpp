#include <texture/perlin_noise.hpp>
#include <iostream>
#include <glm/gtx/normal.hpp>
#include <generator.hpp>
#include <glm/gtx/intersect.hpp>
#include <algorithm>
#include <opencv2/opencv.hpp>

namespace apollo{

Image getPerlin(int iterCount=2){
    float radius = 0.4f;
    Sphere sphere =  Sphere(radius,iterCount);
    std::vector<Point> points = sphere.getPoints();
    std::cout << "Initializing the Texture \n";
    std::cout << "points " <<  points.size() << "\n";
    int w = 640;
    int h = w/2;
    Image image;
    image.width = w;
    image.height =  h;
    image.channels =  3;
    image.data = new unsigned char[3*w*h]; 
    for (int i=0; i < w*h*3; i++){
            image.data[i] = 0;
    }
    int k=0;
    //for (int i=0; i<1 ;i++){
    float noise[w*h] = {0.0f};
    float noise2[w*h] = {0.0f};
    float maxNoise = -100.0f; float minNoise=100.0f;
 
    for (int i=0; i<points.size()/3 ;i++){
        Point a = points[3*i];
        Point b = points[3*i+1];
        Point c = points[3*i+2];
        glm::vec3 p1 = glm::vec3(a.x);
        glm::vec3 p2 = glm::vec3(b.x);
        glm::vec3 p3 = glm::vec3(c.x);

        int maxx = (int)(std::max(std::max(a.t.x,b.t.x),c.t.x) *w);
        int minx = (int)(std::min(std::min(a.t.x,b.t.x),c.t.x) *w);
        int maxy = (int)(std::max(std::max(a.t.y,b.t.y),c.t.y) *h);
        int miny = (int)(std::min(std::min(a.t.y,b.t.y),c.t.y) *h);
           
        if (maxx-minx >= 0.5*w){
            std::cout <<"HERE\n";
            continue;
        }
        for (int x=minx; x<=maxx+1;x++){

            glm::vec3 pp =  glm::triangleNormal(p1,p2,p3);
            pp = pp/glm::length(pp);
            float d = glm::dot(pp,p1);
 

            int tmpx = 1.5*x;
            if (tmpx>=w){
                tmpx = tmpx-w;

            }
            float theta  = (tmpx*(1.0/w) -0.5f)*2*M_PI;
            for (int y=miny; y<=maxy; y++){
                float phi  =  (0.5f - y*(1.0/h))*M_PI;
                float y1 =  radius*sin(phi);
                float x1  = radius*cos(theta)*cos(phi);
                float z1  = radius*sin(theta)*cos(phi);
                glm::vec3 v(x1,y1,z1); 
                float t = d/glm::dot(pp,v);
                v =  t*v;

                float mb = glm::dot(glm::cross(v-p1, p3-p1),pp)/glm::dot(glm::cross(p2-p1, p3-p1),pp);
                float mc = glm::dot(glm::cross(p2-p1, v-p1),pp)/glm::dot(glm::cross(p2-p1, p3-p1),pp);
                

                float ma =  1 - mb -mc;
                if ( (mb>=0 && mc >= 0 && ma >= 0 )  && (y*w + x  < h*w)  ) {
                    ma  = 6*ma*ma*ma*ma*ma-15*ma*ma*ma*ma+10*ma*ma*ma; 
                    mb  = 6*mb*mb*mb*mb*mb-15*mb*mb*mb*mb+10*mb*mb*mb; 
                    mc  = 6*mc*mc*mc*mc*mc-15*mc*mc*mc*mc+10*mc*mc*mc; 
                    float val = ma*glm::dot(v-p1,a.rn) + mb*glm::dot(v-p2,b.rn) + mc*glm::dot(v-p3,c.rn);
                    //val  = mb;
                    noise[y*w+x] = val;
                    if (tmpx < 1.5*x){
                        noise[y*w+ (int)(std::round(tmpx/1.5))] = val;
                    }
                    if (val > maxNoise){
                        maxNoise = val;
                    }
                    if (val < minNoise){
                        minNoise = val;
                        
                    }

                   k++;
                }

            }
        }
    }

    cv::Mat src = cv::Mat(h, w, CV_32FC1, noise);  
    
    GaussianBlur(src, src, cv::Size(5,5),1.0, 1.0, cv::BORDER_DEFAULT);
    minNoise = 100.0f;
    maxNoise = -100.0f;
    for (int i=0 ; i < w*h; i++){
         if (maxNoise < noise[i]){
            maxNoise = noise[i];
         }
        if (minNoise > noise[i]){
            minNoise = noise[i];
         }

    }


    
    
    
    
      for (int i=0 ; i < w*h; i++){
         noise [i] =   (noise[i] - minNoise)/(maxNoise-minNoise);
    }


    cv::imshow("ANSHUMAN",src);
    cv::waitKey(0);

    for (int i=0 ; i < w*h; i++){
    
         image.data[3*i+0] =   noise[i]*255;
         image.data[3*i+1] =   noise[i]*255;
         image.data[3*i+2] =   noise[i]*255;
  
    }            

    std::cout << k << "\n"; 
    return image;
}
Image PerlinNoise::load(){
    Image perlin1  = getPerlin(1);
    Image perlin2  = getPerlin(3);
    Image perlin3  = getPerlin(4);
    Image perlin4  = getPerlin(5);
    
    Image image;
    int w = perlin1.width;
    int h = perlin1.height;
    image.width = w;
    image.height = h;
    image.channels = perlin1.channels;
    image.data = new unsigned char[3*w*h];
    int min = 0;
    for (int i=0; i <  w*h; i++){
            image.data[3*i+0] = 180 + std::max(-50,std::min(75, 
                    (int)(0.75*(perlin1.data[3*i]-128) + 0.25*(perlin2.data[3*i]-128) + 0.25*(perlin3.data[3*i]-128) + 0.25* (perlin4.data[3*i]-128) )));
             
            image.data[3*i+1] = 80 + std::max(-30,std::min(30, 
                    (int)(0.55*(perlin1.data[3*i]-128) + 0.25*(perlin2.data[3*i]-128) + 0.15*(perlin3.data[3*i]-128) + 0.35* (perlin4.data[3*i]-128) )));
            //image.data[3*i] = perlin1.data[3*i] ;
            //image.data[3*i+1] = perlin1.data[3*i] ;
            //image.data[3*i+2] = perlin1.data[3*i] ;
            image.data[3*i+2] = 0;
    }
    return image; 
    
}


}
