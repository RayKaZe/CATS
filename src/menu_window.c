#include <pebble.h>
#include "database.h"
#include "barcode_window.h"

MenuLayer *menu_layer;
Window* menu_window;

void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
  struct card_entry entry = get_entry(cell_index->row);
  APP_LOG(APP_LOG_LEVEL_INFO, "string %s", (entry.title));
  menu_cell_basic_draw(ctx, cell_layer, entry.title, entry.data, NULL);
}
 
uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
  int list_len =  get_free_data_key()-1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "list length: %i", list_len);
  if (list_len < 0)
    return 0;
  return list_len;
}
 
void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
  //Get which row
  int which = cell_index->row;
  struct card_entry entry = get_entry(which);

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
}
