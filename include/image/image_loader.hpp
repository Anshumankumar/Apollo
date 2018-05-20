#ifndef APOLLO_IMAGE_LOADER_HPP
#define APOLLO_IMAGE_LOADER_HPP

#include <string>

class ImageLoader{
    /* This class will load images so that it can be used by opengl for textures
     */
    protected:
    std::string filename;

    public: 
    struct Image{
        int width = 0;
        int height = 0;
        int channels = 0 ;
        unsigned char * data;
    };
    public:
    ImageLoader(std::string);
    virtual Image load() = 0;
};
#endif
