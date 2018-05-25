#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP
#include<image/image_loader.hpp>

namespace apollo{
class PerlinNoise:public ImageLoader{
    public:
    using ImageLoader::ImageLoader;   
    Image load();
};
}
#endif
