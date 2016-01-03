#include <pebble.h>
#include "database.h"

typedef enum {
  KEY_CARDNAME   = 0,
  KEY_CARDNUMBER = 1,
  NUM_ENTRIES    = 2,
  GET_NTH_ENTRY  = 3,
  CLEAR_PERSIST  = 4
} AppKey;

static void send_num_entries() {
  DictionaryIterator* dictionaryIterator = NULL;
  AppMessageResult retval;

  uint16_t n = num_entries();

  retval = app_message_outbox_begin (&dictionaryIterator);
  if (retval != APP_MSG_OK) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Begin outbox failed. retval: %i", retval);
  }

  dict_write_uint8 (dictionaryIterator, NUM_ENTRIES, n);

  retval = app_message_outbox_send ();
  if (retval != APP_MSG_OK) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Begin send failed. retval: %i", retval);
  }
}

static void send_nth_entry(uint8_t *n) {
  DictionaryIterator* dictionaryIterator = NULL;
  struct card_entry entry;

  entry = get_nth_entry((unsigned int) *n);

  app_message_outbox_begin (&dictionaryIterator);
  dict_write_cstring(dictionaryIterator, KEY_CARDNAME, entry.title);
  dict_write_cstring(dictionaryIterator, KEY_CARDNUMBER, entry.data);
  dict_write_uint8(dictionaryIterator, GET_NTH_ENTRY, *n);
  app_message_outbox_send ();
}

static void ack_clear_persist() {
  DictionaryIterator* dictionaryIterator = NULL;

  app_message_outbox_begin(&dictionaryIterator);
  dict_write_uint8(dictionaryIterator, CLEAR_PERSIST, 1);
  app_message_outbox_send();
}

static void in_recv_handler(DictionaryIterator *iterator, void *context)
{
  //Get Tuple
  Tuple *t = dict_read_first(iterator);
  char *name = NULL;
  char *number = NULL;
  int name_len = 0;
  int number_len = 0;
  struct card_entry entry;

  while (t != NULL)
  {
    //app_log(APP_LOG_LEVEL_INFO,"main.c",44,"%" PRIu32,t->key);
    switch(t->key)
    {
      case KEY_CARDNAME:
        APP_LOG(APP_LOG_LEVEL_INFO, "%s", t->value->cstring);
        name_len = strlen( t->value->cstring );
        name = malloc(name_len+1);
        strcpy( name, t->value->cstring );
        break;
      case KEY_CARDNUMBER:
        APP_LOG(APP_LOG_LEVEL_INFO, "%s", t->value->cstring);
        number_len = strlen( t->value->cstring );
        number = malloc(number_len+1);
        strcpy( number, t->value->cstring );
        break;
      case NUM_ENTRIES:
        send_num_entries();
        break;
      case GET_NTH_ENTRY:
        send_nth_entry(t->value->data);
        break;
      case CLEAR_PERSIST:
        clear_persist();
        ack_clear_persist();
        break;
    }
    // Get next pair, if any
    t = dict_read_next(iterator);
  }

  if (name != NULL && number != NULL)
  {
    entry.data_type = BARCODE;
    entry.title = name;
    entry.data = number;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "adding data %s, %s", entry.title, entry.data);
    add_entry(entry);
    free(name);
    free(number);
  }
}

void appMessageOutboxSent(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "App Message Outbox Sent.");
}

void appMessageOutboxFailed(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_INFO, "App Message Failed. %d", (int) reason);
    if (reason == APP_MSG_SEND_TIMEOUT){
        APP_LOG(APP_LOG_LEVEL_INFO, "Timeout");
    }
}

void app_comms_init()
{
  app_message_register_outbox_failed(appMessageOutboxFailed);
  app_message_register_outbox_sent(appMessageOutboxSent);
  app_message_register_inbox_received((AppMessageInboxReceived) in_recv_handler);
  app_message_open(128, 128);
}
