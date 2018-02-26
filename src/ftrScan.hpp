#ifndef WILTON_FINGERPRINT_FTRSCAN_H
#define WILTON_FINGERPRINT_FTRSCAN_H

#include <vector>
#include <stdint.h>

int ftr_open_device();
int ftr_get_scan(unsigned char **out_buffer, int& out_width, int& out_height, uint32_t timeout_ms);
void ftr_close_device();

#endif  /* WILTON_FINGERPRINT_FTRSCAN_H */
