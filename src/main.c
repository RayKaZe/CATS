#include <pebble.h>
#include <inttypes.h>

#include "barcode.h"
#include "database.h"
#include "splash_window.h"
#include "menu_window.h"
#include "app_comms.h"

void init()
{
  struct card_entry entry;

  menu_window_init();
  splash_window_init();
  app_comms_init();
  
  clear_persist();
  entry.data_type = BARCODE;
  entry.title = "Tesco1";
  entry.data = "1234567890";
  APP_LOG(APP_LOG_LEVEL_DEBUG, "entry %s, %s", entry.title, entry.data);
  add_entry( entry );
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
