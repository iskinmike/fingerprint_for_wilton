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
 * File:   bitmap_to_png.cpp
 * Author: mike
 *
 * Created on February 21, 2018, 4:39 PM
 */

#include <stdio.h>
#include "bitmap_to_png.h"

BitmapConverter::BitmapConverter() : settings(), png(nullptr),
    info(nullptr), error(""), error_state(false) {}

bool BitmapConverter::init()
{
    png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        error = "png_create_write_struct failed";
        return false;
    }

    info = png_create_info_struct(png);
    if (!info) {
        error = "png_create_info_struct failed";
        return false;
    }
    return true;
}

bool BitmapConverter::convertBitmapToPNGFile(
        const std::string &filename, unsigned char *bitmap,
        std::uint32_t width, std::uint32_t height)
{
    FILE* fp = fopen(filename.c_str(), "wb");
    if (!fp) {
        error = "cant reopen file: " + filename;
        return false;
    }
    png_bytepp row_pointers = new png_bytep[height];

    if (setjmp(png_jmpbuf(png))) {
      error = "convertBitmapToPNGFile failed";
      delete row_pointers;
      return false;
    }

    png_init_io(png, fp);

    // set output image features
    png_set_IHDR(
        png,
        info,
        width, height,
        settings.bits,
        settings.color,
        settings.interscale,
        settings.compression,
        settings.filter
    );
    png_write_info(png, info);

    for (int i = 0; i < height; ++i) {
        row_pointers[i] = static_cast<png_bytep>(bitmap + i*width);
    }

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    fclose(fp);
    delete row_pointers;

    return true;
}

std::string BitmapConverter::getLastError()
{
    return error;
}

BitmapConverter::BitmapConverterSettings::BitmapConverterSettings() {
    bits        = 8;
    color       = PNG_COLOR_TYPE_GRAY;
    interscale  = PNG_INTERLACE_NONE;
    compression = PNG_COMPRESSION_TYPE_DEFAULT;
    filter      = PNG_FILTER_TYPE_DEFAULT;
}
