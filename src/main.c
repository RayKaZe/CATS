#include <pebble.h>
#include <inttypes.h>

#include "barcode.h"
#include "database.h"
#include "splash_window.h"
#include "menu_window.h"
#include "app_comms.h"

void init()
{
  setup_entry_db();
  menu_window_init();
  splash_window_init();
  app_comms_init();
  
  clear_persist();

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
