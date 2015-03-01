#include <pebble.h>
#include <inttypes.h>
  
#define KEY_CARDNAME 0
#define KEY_CARDNUMBER 1
 
static Window *window;
static TextLayer *text_layer_info;
static TextLayer *text_layer_name;
static TextLayer *text_layer_number;

static void new_layer(Window *window, TextLayer **layer, GRect loc, char *message) {
  *layer = text_layer_create(loc);
  text_layer_set_font(*layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text(*layer, message); 
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(*layer));
}
 
static void window_load(Window *window) 
{
  //Check for saved option
  //char card_name = persist_read_string(KEY_CARDNAME);
  //int card_number = persist_read_string(KEY_CARDNUMBER);
 
  new_layer(window, &text_layer_info, GRect(0, 0, 144, 60), "#HackLondon\nOh hai team :)\0");
  new_layer(window, &text_layer_name, GRect(0, 60, 144, 30), "Old McCharger had a farm...\nE I E I O\0");
  new_layer(window, &text_layer_number, GRect(0, 90, 144, 78), "Useful things go 'ere\0");
}
 
static void window_unload(Window *window) 
{
  text_layer_destroy(text_layer_name);
  text_layer_destroy(text_layer_number);
  text_layer_destroy(text_layer_info);
}
 
static void in_recv_handler(DictionaryIterator *iterator, void *context)
{
  //Get Tuple
  Tuple *t = dict_read_first(iterator);
  
  while (t != NULL)
  {
    app_log(APP_LOG_LEVEL_INFO,"main.c",44,"%" PRIu32,t->key);
    switch(t->key)
    {
    case KEY_CARDNAME:
        app_log(APP_LOG_LEVEL_INFO,"main.c",48,"%s",t->value->cstring);
        text_layer_set_text(text_layer_name, t->value->cstring);
        //persist_write_string(KEY_CARDNAME, t->key->value);
        break;
    case KEY_CARDNUMBER:
        app_log(APP_LOG_LEVEL_INFO,"main.c",53,"%s",t->value->cstring);
        text_layer_set_text(text_layer_number, t->value->cstring);
        //persist_write_string(KEY_CARDNUMBER, t->key->value);
        break;
    }
    // Get next pair, if any
    t = dict_read_next(iterator);
  }
}

static void init(void) 
{
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
  app_message_register_inbox_received((AppMessageInboxReceived) in_recv_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  window_stack_push(window, true);
}
 
static void deinit(void) 
{
  window_destroy(window);
}
 
int main(void) 
{
  init();
  app_event_loop();
  deinit();
}