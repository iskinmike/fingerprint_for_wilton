/*
 * Copyright 2018, mike at github.com/iskinmike
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * File:   bitmap_to_png.h
 * Author: mike
 *
 * Created on February 21, 2018, 4:39 PM
 */

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
