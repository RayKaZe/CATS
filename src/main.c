#include <pebble.h>
#include <inttypes.h>

#include "barcode.h"
#include "database.h"
#include "splash_window.h"

Window* menu_window;
Window* bar_code_window;
MenuLayer *menu_layer;
BitmapLayer *barcode;
TextLayer *footer;
GBitmap *bmp1;

#define MARGIN 5
#define KEY_CARDNAME 0
#define KEY_CARDNUMBER 1

void bar_code_window_load(Window *window)
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
  
  APP_LOG(APP_LOG_LEVEL_INFO, "bar_code_window_load, data: %s", data->data);
  
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

void bar_code_window_unload(Window *window)
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
    .load = bar_code_window_load,
    .unload = bar_code_window_unload
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

static void in_recv_handler(DictionaryIterator *iterator, void *context)
{
  //Get Tuple
  Tuple *t = dict_read_first(iterator);
  char *name = NULL;
  char *number = NULL;
  int name_len = 0;
  int number_len = 0;
  char *data = NULL;
  
  while (t != NULL)
  {
    //app_log(APP_LOG_LEVEL_INFO,"main.c",44,"%" PRIu32,t->key);
    switch(t->key)
    {
    case KEY_CARDNAME:
        app_log(APP_LOG_LEVEL_INFO,"main.c",48,"%s",t->value->cstring);
        name_len = strlen( t->value->cstring );
        name = malloc(name_len);
        strcpy( name, t->value->cstring );
        break;
    case KEY_CARDNUMBER:
        app_log(APP_LOG_LEVEL_INFO,"main.c",53,"%s",t->value->cstring);
        number_len = strlen( t->value->cstring );
        number = malloc(number_len);
        strcpy( number, t->value->cstring );
        break;
    }
    // Get next pair, if any
    t = dict_read_next(iterator);
  }
  
  if (name != NULL && number != NULL)
  {
    data = malloc(32);
    strcpy( data, name );
    strcpy( data + 11, number );
    APP_LOG(APP_LOG_LEVEL_DEBUG, "adding data %s, %s", data, data+11);
    add_entry(data);
    free(data);
  }
  setup_entry_db();
}

void init()
{
    /*clear_persist();
    char data1[32];
    char data2[32];
    memset(data1, 0, 32);
    memset(data2, 0, 32);
    strcpy(data1, "Tesco1");
    strcpy(data1+11, "1234567890");
    strcpy(data2, "Tesco");
    strcpy(data2+11, "0987654321");

    APP_LOG(APP_LOG_LEVEL_DEBUG, "data1 %s, %s", data1, data1+11);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "data2 %s, %s", data2, data2+11);
    add_entry( data1 );
    add_entry( data2 );
    */
    setup_entry_db();
    menu_window = window_create();
    WindowHandlers handlers = {
        .load = menu_window_load,
        .unload = menu_window_unload
    };
    window_set_window_handlers( menu_window, (WindowHandlers) handlers );
    window_stack_push(menu_window, true);

    splash_window_init();

    app_message_register_inbox_received((AppMessageInboxReceived) in_recv_handler);
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}
 
void deinit()
{
  window_destroy(menu_window);
}
 
int main(void)
{
    init();
    app_event_loop();
    deinit();
}