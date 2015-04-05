#include <pebble.h>
#include "database.h"

#define KEY_CARDNAME 0
#define KEY_CARDNUMBER 1

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

void app_comms_init()
{
  app_message_register_inbox_received((AppMessageInboxReceived) in_recv_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}
