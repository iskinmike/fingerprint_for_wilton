#ifndef WILTON_FINGERPRINT_BITMAP_CONVERTER
#define WILTON_FINGERPRINT_BITMAP_CONVERTER

#include <png.h>
// must go after png.h
#include <csetjmp>
#include <string>

class BitmapConverter
{
public:
    struct BitmapConverterSettings {
        std::int32_t bits; // 8
        std::int32_t color; //PNG_COLOR_TYPE_GRAY;
        std::int32_t interscale; //PNG_INTERLACE_NONE;
        std::int32_t compression; //PNG_COMPRESSION_TYPE_DEFAULT;
        std::int32_t filter; //PNG_FILTER_TYPE_DEFAULT;

        BitmapConverterSettings();
    } settings;
private:
    png_structp png;
    png_infop info;
    std::string error;
    bool error_state;

public:

    BitmapConverter();

    bool init();
    bool convertBitmapToPNGFile(const std::string& filename,
            unsigned char* bitmap, uint32_t width, uint32_t height);
    std::string getLastError();

};

#endif  /* WILTON_FINGERPRINT_BITMAP_CONVERTER */
