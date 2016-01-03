#include <pebble.h>
#include "database.h"
#include "barcode_window.h"

MenuLayer *menu_layer;
Window* menu_window;

static Window *help_text_window = NULL;
static TextLayer *help_text_layer;

static void help_text_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  help_text_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 5, window_bounds.size.h));
  text_layer_set_text(help_text_layer, "Use \"Settings\" button for the CATS app on your pebble mobile phone app to add cards.");
  text_layer_set_overflow_mode(help_text_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(help_text_layer));
}

static void help_text_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(help_text_layer);
}

static void help_text_window_init() {
  // Create main Window
  if (help_text_window == NULL) {
    help_text_window = window_create();
    window_set_window_handlers(help_text_window, (WindowHandlers) {
      .load = help_text_window_load,
      .unload = help_text_window_unload
    });
  }

  window_stack_push(help_text_window, true);
}

static void help_text_window_deinit() {
  window_destroy(help_text_window);
}

void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
  struct card_entry entry;
  unsigned int which = cell_index->row+1;
  APP_LOG(APP_LOG_LEVEL_INFO, "which: %i", which);

  entry = get_nth_entry(which);

  if (entry.data_type == UNDEFINED)
    // draw help text
    menu_cell_basic_draw(ctx, cell_layer, "Add card",  "Use \"Settings\" button", NULL);
  else
    menu_cell_basic_draw(ctx, cell_layer, entry.title, entry.data, NULL);
}

uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
  uint16_t list_len = num_entries() + 1; // add 1 row for the help text

  APP_LOG(APP_LOG_LEVEL_DEBUG, "list length: %i", list_len);
  return list_len;
}

void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
  //Get which row
  int which = cell_index->row+1;
  struct card_entry entry = get_nth_entry(which);

  if (entry.data_type != UNDEFINED) {
    switch (entry.data_type) {
      case BARCODE:
        display_bar_code( &entry );
        break;
      case QRCODE:
        //display_qr_code(entry_db[which].data);
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_WARNING, "Unrecognised Data Type");
    }
  } else {
    help_text_window_init();
  }
}

void menu_window_load(Window *window)
{
  //Create it - 12 is approx height of the top bar
  menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));

  //Let it receive clicks
  menu_layer_set_click_config_onto_window(menu_layer, window);

  //Give it its callbacks
  MenuLayerCallbacks callbacks = {
      .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
      .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
      .select_click = (MenuLayerSelectCallback) select_click_callback
  };
  menu_layer_set_callbacks(menu_layer, NULL, callbacks);

  //Add to Window
  layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));
}

void menu_window_unload(Window *window)
{
  menu_layer_destroy(menu_layer);
}

void menu_window_reload()
{
  menu_layer_reload_data(menu_layer);
}

void menu_window_init()
{
  menu_window = window_create();
  WindowHandlers handlers = {
      .load = menu_window_load,
      .unload = menu_window_unload
  };
  window_set_window_handlers( menu_window, (WindowHandlers) handlers );
  window_stack_push(menu_window, true);
}

void menu_window_deinit()
{
  window_destroy(menu_window);
  help_text_window_deinit();
}
