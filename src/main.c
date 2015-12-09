#include <pebble.h>
#include <inttypes.h>

#include "barcode.h"
#include "database.h"
#include "splash_window.h"
#include "menu_window.h"
#include "app_comms.h"

#define NDEBUG 1

void init()
{
  struct card_entry entry;

  menu_window_init();
  splash_window_init();
  app_comms_init();

#ifndef NDEBUG
  clear_persist();
  entry.data_type = BARCODE;
  entry.title = "Tesco1";
  entry.data = "1234567890";
  APP_LOG(APP_LOG_LEVEL_DEBUG, "entry %s, %s", entry.title, entry.data);
  add_entry( entry );
  entry.title = "Tesco2";
  entry.data = "0123456789";
  add_entry( entry );
  entry = get_nth_entry(2);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "entry %s, %s", entry.title, entry.data);
#endif
}

void deinit()
{
  menu_window_deinit();
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}
