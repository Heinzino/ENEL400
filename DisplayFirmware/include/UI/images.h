#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

extern const uint8_t bikeicon_rle[];
extern const size_t bikeicon_rle_len;

extern const uint8_t batteryicon_rle[];
extern const size_t batteryicon_rle_len;

uint8_t* decodeRLEWithMalloc(const uint8_t *rleData, size_t rleLength, size_t *outputSize);

