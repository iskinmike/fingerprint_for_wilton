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
 * File:   ftrScan.cpp
 * Author: mike
 *
 * Created on February 21, 2018, 4:39 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory>

#include <time.h>
#include <chrono>
#include <thread>
#include <atomic>

#include "ftrScanAPI.h" // modified
#include "ftrScan.hpp"

void debugPrint(const std::string& message);

void PrintErrorMessage( unsigned long nErrCode )
{
  printf("Failed to obtain image. ");

  char stError[64];

    switch( nErrCode )
  {
    case 0:
        strcpy( stError, "OK" );
        break;
    case FTR_ERROR_EMPTY_FRAME: // ERROR_EMPTY
        strcpy( stError, "- Empty frame -" );
        break;
    case FTR_ERROR_MOVABLE_FINGER:
        strcpy( stError, "- Movable finger -" );
        break;
    case FTR_ERROR_NO_FRAME:
        strcpy( stError, "- Fake finger -" );
        break;
    case FTR_ERROR_HARDWARE_INCOMPATIBLE:
        strcpy( stError, "- Incompatible hardware -" );
        break;
    case FTR_ERROR_FIRMWARE_INCOMPATIBLE:
        strcpy( stError, "- Incompatible firmware -" );
        break;
    case FTR_ERROR_INVALID_AUTHORIZATION_CODE:
        strcpy( stError, "- Invalid authorization code -" );
        break;
    default:
        sprintf( stError, "Unknown return code - %lu", nErrCode );
  }
  printf("%s\n", stError);
}

namespace {
  void *hDevice;
  FTRSCAN_IMAGE_SIZE ImageSize;
  static unsigned char *pBuffer;

  const useconds_t sleep_microseconds = 500;
  std::atomic<bool> is_time_expired(false);
}

int ftr_open_device() {
  hDevice = ftrScanOpenDevice();
  if( hDevice == NULL )
  {
    printf("Failed to open device!\n");
    return -1;
  }
  return 0;
}

void sleep_thread(uint32_t msecs){
    std::this_thread::sleep_for(std::chrono::milliseconds(msecs));
    is_time_expired.exchange(true);
}

int ftr_get_scan(unsigned char **out_buffer, int& out_width, int& out_height, uint32_t timeout_ms) {
  int result = 0;

  auto waiter = std::thread(sleep_thread, timeout_ms);
  waiter.detach(); // detach to not join if finger occured

  if( !ftrScanGetImageSize( hDevice, &ImageSize ) )
  {
    printf("Failed to get image size\n");
    ftrScanCloseDevice( hDevice );
    return -1;
  }
  else
  {
    printf("Please put your finger on the scanner. We wait you for [%d] ms:\n", timeout_ms);

    auto start = std::chrono::system_clock::now();

    int count = 0;
    while(1)
    {
      ftrScanSetDiodesStatus(hDevice, (unsigned int)100/2, (unsigned int)100/2);
      if( ftrScanIsFingerPresent( hDevice, NULL ) )
        break;
      usleep(sleep_microseconds);
      count++;
      if (is_time_expired.load()) {
          auto end = std::chrono::system_clock::now();
          std::chrono::duration<double> diff = end - start;
          printf("No fingerprint in '%3.3f' ms\n", diff.count());
          return result;
      }
    }

    printf("Image size is %d\n", ImageSize.nImageSize);
    pBuffer = (unsigned char *)malloc(ImageSize.nImageSize );

    printf("Capturing fingerprint ......\n");
    while(1)
    {
      if( ftrScanGetFrame(hDevice, pBuffer, NULL) )
      {
        printf("Done!\n");
        *out_buffer = pBuffer;
        out_width = ImageSize.nWidth;
        out_height = ImageSize.nHeight;
        result = ImageSize.nImageSize;
        break;
      }
      else
      {
        PrintErrorMessage( ftrScanGetLastError() );
        usleep(sleep_microseconds);
      }
    }
  }

  return result;
}

void ftr_close_device() {
  ftrScanSetDiodesStatus(hDevice, 0, 0);
  free( pBuffer );
  ftrScanCloseDevice( hDevice );
}
