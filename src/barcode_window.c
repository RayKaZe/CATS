#include <pebble.h>
#include "database.h"
#include "barcode.h"

static BitmapLayer *barcode;
static TextLayer *footer;
static uint8_t *addr;
static GBitmap *bmp;

void barcode_window_load(Window *window)
{
  Layer *windowLayer = window_get_root_layer(window);
  card_entry *data = (card_entry *) window_get_user_data(window);
  GRect bounds = layer_get_bounds(windowLayer);

  bmp = gbitmap_create_blank(bounds.size, GBitmapFormat1Bit);
  barcode = bitmap_layer_create(bounds);
  bitmap_layer_set_alignment(barcode, GAlignCenter);

  bounds.size.h = drawCode128(data->data, bmp);
  gbitmap_set_bounds(bmp, bounds);
  bitmap_layer_set_bitmap(barcode, bmp);

  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(barcode));

  // Create time TextLayer
  footer = text_layer_create(GRect(0, 130, 144, 38));
  text_layer_set_background_color(footer, GColorClear);
  text_layer_set_text_color(footer, GColorBlack);

  // Improve the layout to be more like a watchface
  text_layer_set_font(footer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(footer, GTextAlignmentCenter);
  text_layer_set_text(footer, data->title);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(footer));
}

void barcode_window_destory(void *window) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Destroy window");
  window_destroy((Window *) window);
}

void barcode_window_unload(Window *window)
{
  card_entry *data = (card_entry *) window_get_user_data(window);
  free(data->data);
  free(data->title);
  free(data);

  bitmap_layer_destroy(barcode);
  gbitmap_destroy(bmp);
  text_layer_destroy(footer);
  app_timer_register(100, barcode_window_destory, window);
}

void display_bar_code( struct card_entry * data )
{
  Window* bar_code_window;
  card_entry *entry = malloc(sizeof(card_entry));
  memcpy(entry, data, sizeof(card_entry));

  APP_LOG(APP_LOG_LEVEL_INFO, "Displaying BARCODE");
  bar_code_window = window_create();
  WindowHandlers handlers = {
    .load = barcode_window_load,
    .unload = barcode_window_unload
  };
  window_set_window_handlers(bar_code_window, (WindowHandlers) handlers);
#ifdef PBL_SDK_2
  window_set_fullscreen(bar_code_window, true);
#endif
  window_set_user_data(bar_code_window, entry);
  window_stack_push(bar_code_window, true);
}

void display_qr_code( char *data )
{
  /* placeholder for when we want to implement the qr code */
  APP_LOG(APP_LOG_LEVEL_INFO, "Displaying QRCODE");
}

