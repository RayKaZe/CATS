#include <pebble.h>

#include "barcode.h"

const char* charLookup[] = {
  "11011001100",
  "11001101100",
  "11001100110",
  "10010011000",
  "10010001100",
  "10001001100",
  "10011001000",
  "10011000100",
  "10001100100",
  "11001001000",
  "11001000100",
  "11000100100",
  "10110011100",
  "10011011100",
  "10011001110",
  "10111001100",
  "10011101100",
  "10011100110",
  "11001110010",
  "11001011100",
  "11001001110",
  "11011100100",
  "11001110100",
  "11101101110",
  "11101001100",
  "11100101100",
  "11100100110",
  "11101100100",
  "11100110100",
  "11100110010",
  "11011011000",
  "11011000110",
  "11000110110",
  "10100011000",
  "10001011000",
  "10001000110",
  "10110001000",
  "10001101000",
  "10001100010",
  "11010001000",
  "11000101000",
  "11000100010",
  "10110111000",
  "10110001110",
  "10001101110",
  "10111011000",
  "10111000110",
  "10001110110",
  "11101110110",
  "11010001110",
  "11000101110",
  "11011101000",
  "11011100010",
  "11011101110",
  "11101011000",
  "11101000110",
  "11100010110",
  "11101101000",
  "11101100010",
  "11100011010",
  "11101111010",
  "11001000010",
  "11110001010",
  "10100110000",
  "10100001100",
  "10010110000",
  "10010000110",
  "10000101100",
  "10000100110",
  "10110010000",
  "10110000100",
  "10011010000",
  "10011000010",
  "10000110100",
  "10000110010",
  "11000010010",
  "11001010000",
  "11110111010",
  "11000010100",
  "10001111010",
  "10100111100",
  "10010111100",
  "10010011110",
  "10111100100",
  "10011110100",
  "10011110010",
  "11110100100",
  "11110010100",
  "11110010010",
  "11011011110",
  "11011110110",
  "11110110110",
  "10101111000",
  "10100011110",
  "10001011110",
  "10111101000",
  "10111100010",
  "11110101000",
  "11110100010",
  "10111011110",
};

//static char* startA = "11010000100";
//static char* startC = "11010010000";
const char* startC = "11010011100";
const char* fnc    = "11110101110";
const char* stop   = "1100011101011";

static char* drawBar(char *buf, char val, GBitmap *bmp) {
  uint8_t row_size = gbitmap_get_bytes_per_row(bmp);
  for (int i = 0; i < row_size; i++) {
    buf[i] = val;
  }
  return buf + row_size;
}

static char* drawChar(char *buf, const char *c, GBitmap *bmp) {
  while (*c != '\0') {
    switch (*c) {
    case '0':
      buf = drawBar(buf, 0xFF, bmp);
      break;
    case '1':
      buf = drawBar(buf, 0x00, bmp);
      break;
    }
    c++;
  }
  return buf;
}

static int charWidth = 11;

int drawCode128(char *c, GBitmap *bmp) {
  int count = 2, sum = 207;
  char *buf = (char *) gbitmap_get_data(bmp);
  GRect bounds = gbitmap_get_bounds(bmp);
  uint8_t row_size = gbitmap_get_bytes_per_row(bmp);
  memset(buf, 0xFF, bounds.size.h * row_size);

  //Start sign
  buf = drawChar(buf, startC, bmp);
  buf = drawChar(buf, fnc, bmp);

  while (*c != '\0') {
    int in1 = *c-48;
    c++;
    int in2 = *c-48;
    int number = in1 * 10 + in2;

    buf = drawChar(buf, charLookup[number], bmp);
    APP_LOG(APP_LOG_LEVEL_INFO, "Number: %d", number);

    sum += count * (number);
    count++;
    c++;
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "Sum0: %d", sum);

  sum %= 103;

  APP_LOG(APP_LOG_LEVEL_INFO, "Sum: %d", sum);
  buf = drawChar(buf, charLookup[sum], bmp);
  buf = drawChar(buf, stop, bmp);

  return (count+3)*charWidth +1;
}
