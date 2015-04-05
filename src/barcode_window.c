#include <pebble.h>
#include "database.h"
#include "barcode.h"

#define MARGIN 5

Window* bar_code_window;
BitmapLayer *barcode;
TextLayer *footer;
GBitmap *bmp1;

void barcode_window_load(Window *window)
{
  Layer *windowLayer = window_get_root_layer(window);
	card_entry *data = (card_entry *) window_get_user_data(window);
  GRect bounds = layer_get_bounds(windowLayer);
	bounds.origin.y += MARGIN;
	bounds.size.h -= 2 * MARGIN;
  bmp1 = gbitmap_create_blank(bounds.size);
  barcode = bitmap_layer_create(bounds);
	
	bitmap_layer_set_alignment(barcode, GAlignCenter);
	
	// Width in bytes, aligned to multiples of 4.
	bmp1->row_size_bytes = (bounds.size.w/8+3) & ~3;
	bmp1->addr = malloc(bounds.size.h * bmp1->row_size_bytes);
  
  APP_LOG(APP_LOG_LEVEL_INFO, "barcode_window_load, data: %s", data->data);
  
  bmp1->bounds.size.h = drawCode128(data->data);
  
  bitmap_layer_set_bitmap(barcode, bmp1);
  
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(barcode));
  
  // Create time TextLayer
  footer = text_layer_create(GRect(0, 140, 144, 30));
  text_layer_set_background_color(footer, GColorClear);
  text_layer_set_text_color(footer, GColorBlack);
  
  // Improve the layout to be more like a watchface
  text_layer_set_font(footer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(footer, GTextAlignmentCenter);
  text_layer_set_text(footer, data->title);
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(footer));
}

void barcode_window_unload(Window *window)
{
    bitmap_layer_destroy(barcode);
    gbitmap_destroy(bmp1);
    text_layer_destroy(footer);
}

void display_bar_code( struct card_entry * data )
{
  APP_LOG(APP_LOG_LEVEL_INFO, "Displaying BARCODE\n");
  bar_code_window = window_create();
  WindowHandlers handlers = {
    .load = barcode_window_load,
    .unload = barcode_window_unload
  };
  window_set_window_handlers(bar_code_window, (WindowHandlers) handlers);
  window_set_fullscreen(bar_code_window, true);
  window_set_user_data(bar_code_window, data);
  window_stack_push(bar_code_window, true);
}

void display_qr_code( char *data)
{
  /* placeholder for when we want to implement the qr code */
  APP_LOG(APP_LOG_LEVEL_INFO, "Displaying QRCODE\n");
}
