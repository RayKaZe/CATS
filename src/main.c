#include <pebble.h>
#include <barcode.h>
#include <inttypes.h>

Window* menu_window;
Window* bar_code_window;
Window* splash_window;
MenuLayer *menu_layer;
TextLayer *splash_logo;
static GBitmap *splash_cat_bitmap;
static BitmapLayer *splash_cat_bitmap_layer;
InverterLayer *cat_inverter_layer;
BitmapLayer *barcode;

#define MARGIN 5
#define NUM_ENTRIES 7
#define MAX_DATA_KEY 30
#define KEY_CARDNAME 0
#define KEY_CARDNUMBER 1

typedef enum {
  BARCODE, 
  QRCODE,
} data_type;

typedef struct card_entry {
  data_type data_type;
  char *title;
  char *data;
} card_entry;

char label[] = "card";

struct card_entry entry_db[NUM_ENTRIES];

void setup_entry_db()
{
  int i;
  for (i=0; i<NUM_ENTRIES; i++)
  {
    entry_db[i] = (card_entry) {
      .data_type = BARCODE,
      .title = label,
      .data = NULL
    };
  }
}

void bar_code_window_load(Window *window)
{
  Layer *windowLayer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(windowLayer);
	bounds.origin.y += MARGIN;
	bounds.size.h -= 2 * MARGIN;
  bmp = gbitmap_create_blank(bounds.size);
  barcode = bitmap_layer_create(bounds);
	
	bitmap_layer_set_alignment(barcode, GAlignCenter);
	
	// Width in bytes, aligned to multiples of 4.
	bmp->row_size_bytes = (bounds.size.w/8+3) & ~3;
	bmp->addr = malloc(bounds.size.h * bmp->row_size_bytes);
  bmp->bounds.size.h = drawCode128("9794024334524784");
  
  bitmap_layer_set_bitmap(barcode, bmp);
  
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(barcode)); 
}

void bar_code_window_unload(Window *window)
{
    bitmap_layer_destroy(barcode);
    gbitmap_destroy(bmp);
}

void display_bar_code( char *data )
{
  APP_LOG(APP_LOG_LEVEL_INFO, "Displaying BARCODE\n");
  bar_code_window = window_create();
  WindowHandlers handlers = {
    .load = bar_code_window_load,
    .unload = bar_code_window_unload
  };
  window_set_window_handlers(bar_code_window, (WindowHandlers) handlers);
  window_set_fullscreen(bar_code_window, true);
  window_stack_push(bar_code_window, true);
}

void display_qr_code( char *data)
{
  /* placeholder for when we want to implement the qr code */
  APP_LOG(APP_LOG_LEVEL_INFO, "Displaying QRCODE\n");
}

void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
  APP_LOG(APP_LOG_LEVEL_INFO, "string %s", (entry_db[cell_index->row].title));
  menu_cell_basic_draw(ctx, cell_layer, entry_db[cell_index->row].title, "SUB", NULL);
}
 
uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
  return 7;
}
 
void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
  //Get which row
  int which = cell_index->row;

  switch (entry_db[which].data_type) {
    case BARCODE:  
      display_bar_code(entry_db[which].data);
      break;
    case QRCODE:
      display_qr_code(entry_db[which].data);
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
  text_layer_set_text(splash_logo, "CARD");

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
  
  app_timer_register(2000, splash_timer_callback, NULL);
}

void splash_window_unload(Window *window)
{
  window_destroy(window);
  text_layer_destroy(splash_logo);
}

int get_free_data_key()
{
  unsigned int i;
  for (i=0; i<MAX_DATA_KEY; i++)
  {
    if (!persist_exists(i))
      return i;
  }
  return -1;
}

void add_entry(char *data)
{
  unsigned data_len = strlen(data);
  char *byte_array;
  int ret;

  byte_array = malloc(data_len);

  ret = persist_read_int( 0 );
  if (ret == 0)
    persist_write_int(0, 1);
  
  ret = get_free_data_key();
  persist_write_data(ret, byte_array, data_len);
  
  free(byte_array);
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
    app_log(APP_LOG_LEVEL_INFO,"main.c",44,"%" PRIu32,t->key);
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
    
    add_entry(data);
    free(data);
  }
}

void init()
{
    setup_entry_db();
    menu_window = window_create();
    splash_window = window_create();
    WindowHandlers handlers = {
        .load = menu_window_load,
        .unload = menu_window_unload
    };
    window_set_window_handlers( menu_window, (WindowHandlers) handlers );
    window_stack_push(menu_window, true);
  
    handlers.load = splash_window_load;
    handlers.unload = splash_window_unload;
    window_set_window_handlers( splash_window, (WindowHandlers) handlers );
    window_set_fullscreen(splash_window, true);
    window_set_background_color(splash_window, GColorBlack);
    window_stack_push(splash_window, false);
  
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