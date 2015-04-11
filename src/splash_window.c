#include <pebble.h>
#include "database.h"

Window* splash_window;
TextLayer *splash_logo;
static GBitmap *splash_cat_bitmap;
static BitmapLayer *splash_cat_bitmap_layer;
InverterLayer *cat_inverter_layer;

void long_click_handler(ClickRecognizerRef recognizer, void *context)
{
  clear_persist();
}

void config_provider(Window *window)
{
  window_long_click_subscribe(BUTTON_ID_UP, 0, long_click_handler, NULL);
}

void splash_window_unload(Window *window)
{
  window_destroy(window);
  text_layer_destroy(splash_logo);
}

void splash_timer_callback()
{
  window_stack_pop(true);
}

void splash_window_load(Window *window)
{
  GRect cat_bounds= GRect(22, 65, 99, 77);

  // Create time TextLayer
  splash_logo = text_layer_create(GRect(0, 10, 144, 50));
  text_layer_set_background_color(splash_logo, GColorClear);
  text_layer_set_text_color(splash_logo, GColorClear);
  text_layer_set_text(splash_logo, "CATS");

  // Improve the layout to be more like a watchface
  text_layer_set_font(splash_logo, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(splash_logo, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(splash_window), text_layer_get_layer(splash_logo));

  // cat image
  splash_cat_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SPLASH_CAT);

  splash_cat_bitmap_layer = bitmap_layer_create(cat_bounds);
  bitmap_layer_set_bitmap(splash_cat_bitmap_layer, splash_cat_bitmap);
  layer_add_child(window_get_root_layer(splash_window), bitmap_layer_get_layer(splash_cat_bitmap_layer));

  cat_inverter_layer = inverter_layer_create(cat_bounds);
  layer_add_child(window_get_root_layer(splash_window), inverter_layer_get_layer(cat_inverter_layer));

  window_set_click_config_provider(splash_window, (ClickConfigProvider) config_provider);

  app_timer_register(2000, splash_timer_callback, NULL);
}

void splash_window_init()
{
  splash_window = window_create();

  WindowHandlers handlers = {
    .load = splash_window_load,
    .unload = splash_window_unload
  };

  window_set_window_handlers( splash_window, (WindowHandlers) handlers );
  window_set_fullscreen(splash_window, true);
  window_set_background_color(splash_window, GColorBlack);
  window_stack_push(splash_window, false);
}
