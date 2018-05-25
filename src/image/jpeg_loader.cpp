#include <image/jpeg_loader.hpp>
#include <iostream>

namespace apollo{
Image  JpegLoader::load(){
    std::cout << "LOADING IMAGES " <<  this-> filename << "\n";
    FILE *infile =  fopen(filename.c_str(),"rb");
    ErrorManager errorManager;

    struct jpeg_decompress_struct cinfo;
    cinfo.err = jpeg_std_error(&errorManager.jem);
    errorManager.jem.error_exit = JpegLoader::exit;
    errorManager.jem.output_message = JpegLoader::message;

    if (setjmp(errorManager.jumpBuffer)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        Image image;
        return  image;
    }


    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, true);
    jpeg_start_decompress(&cinfo);


    Image image;
	image.width = cinfo.image_width;
	image.height = cinfo.image_height;
	image.channels = cinfo.num_components;
    image.data = new unsigned char[image.width*image.height*image.channels];

	while(cinfo.output_scanline < cinfo.image_height){
		unsigned char* p = image.data + cinfo.output_scanline*image.channels*image.width;
		jpeg_read_scanlines(&cinfo, &p, 1);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);;

	return image;
}


void JpegLoader::exit(j_common_ptr cinfo)
{
    // This work because of Pointer magic I guess
    ErrorManager* pErrorManager = (ErrorManager*) cinfo->err;
    (*cinfo->err->output_message)(cinfo); 
    longjmp(pErrorManager->jumpBuffer, 1);
}


void JpegLoader::message(j_common_ptr cinfo)
{
    char buffer[JMSG_LENGTH_MAX];
    (*cinfo->err->format_message) (cinfo, buffer);
    std::cerr << buffer << std::endl;
}
}
