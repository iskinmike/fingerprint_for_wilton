
#include <stdio.h>
#include <iostream>
#include <popt.h>

#include <png.h>
// must go after png.h
#include <csetjmp>

#include "ftrScan.hpp"
#include "bitmap_to_png.h"

#include <vector>
#include <string>
#include <memory>

int main(int argc, char const *argv[])
{
    char* file_name_ptr = nullptr;
    auto file_name = std::string("image.png");
    auto timeout_ms = std::uint32_t(10000);
    int help = 0;

    std::vector<struct poptOption> table = {
        { "file", 'f', POPT_ARG_STRING, std::addressof(file_name_ptr), static_cast<int> ('f'), "Name of file to save image. By default: [image.png]", nullptr},
        { "timeout", 't', POPT_ARG_INT, std::addressof(timeout_ms), static_cast<int> ('t'), "Timeout for fingerprint reading (ms). By default: [10000]", nullptr},
        { "help", 'h', POPT_ARG_NONE, std::addressof(help), static_cast<int> ('h'), "Show this help message", nullptr},
        { nullptr, 0, 0, nullptr, 0, nullptr, nullptr}};

    auto ctx = poptGetContext(nullptr, argc, const_cast<const char**> (argv), table.data(), POPT_CONTEXT_NO_EXEC);

    { // parse options
        int val;
        while ((val = poptGetNextOpt(ctx)) >= 0){
            switch (val) {
            case 'h':
                poptPrintUsage(ctx, stdout, 0);
                poptPrintHelp(ctx, stdout, 0);
                return 0;
            case 'f':
                if (nullptr != file_name_ptr) {
                    file_name = std::string(file_name_ptr);
                }
                break;
            default:
                break;
            }
        };
        if (val < -1) {
            return 1;
        }
    }

    std::cout << "filename: " << file_name << std::endl;
    std::cout << "timeout(ms): " << timeout_ms << std::endl;

    int res = 0;
    res = ftr_open_device();
    std::cout << "open device res:" << res << std::endl;
    if (0 != res){
        return res;
    }

    int width = 0;
    int height = 0;
    unsigned char* buffer;

    res = ftr_get_scan(std::addressof(buffer), width, height, timeout_ms);
    if (1 > res){
        std::cout << "Failed to get fingerprint" << std::endl;
        ftr_close_device();
        return res;
    }

    BitmapConverter converter;
    res = converter.init();
    if (!res){
        std::cout << converter.getLastError() << std::endl;
        ftr_close_device();
        return 1;
    }
    res = converter.convertBitmapToPNGFile(file_name, buffer, width, height);
    if (!res){
        std::cout << converter.getLastError() << std::endl;
        ftr_close_device();
        return 1;
    }

    ftr_close_device();

    return 0;
}
