#ifndef APOLLO_JPEG_LOADER_HPP
#define APOLLO_JPEG_LOADER_HPP

#include <image/image_loader.hpp>
#include <jpeglib.h>
#include <setjmp.h>

namespace apollo{
class JpegLoader:public ImageLoader{
    private:
    struct ErrorManager
    {
        jpeg_error_mgr jem;
        jmp_buf jumpBuffer;
    };
    static void exit(j_common_ptr cinfo);
	static void message(j_common_ptr cinfo);
    public:
    using ImageLoader::ImageLoader;
    Image load();
};
}

#endif
