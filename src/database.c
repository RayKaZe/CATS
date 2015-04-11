#include <pebble.h>
#include "database.h"
#include "menu_window.h"
#include "string.h"

/*Data format: |Type (1 byte)|Title len (1)|Data len (1)|Title str|Data|*/

struct card_entry *entry_db = NULL;

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

uint16_t num_entries()
{
  uint16_t i;
  uint16_t count = 0;

  for (i=0; i<MAX_DATA_KEY; i++)
  {
    if (persist_exists(i))
      count++;
  }
  return count;
}

struct card_entry get_nth_entry(unsigned int n)
{
  unsigned int i;
  unsigned int count = 0;
  struct card_entry entry;
  entry.data_type = UNDEFINED;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "get %ith entry", n);

  for (i=0; i<MAX_DATA_KEY; i++)
  {
    if (persist_exists(i))
      count++;
    if (count == n)
      return get_entry(i);
  }
  return entry;
}

void clear_persist()
{
  unsigned int i;
  for (i=0; i<MAX_DATA_KEY; i++)
  {
    if (persist_exists(i))
      persist_delete(i);
  }
  menu_window_reload();
}

void add_entry(struct card_entry entry)
{
  char *byte_array;
  int ret;
  unsigned int title_len = strlen(entry.title);
  unsigned int data_len  = strlen(entry.data );

  unsigned int byte_array_len = 3 + title_len + data_len;
  byte_array = malloc(byte_array_len);

  byte_array[0] = entry.data_type;
  byte_array[1] = title_len;
  byte_array[2] = data_len;
  memcpy( byte_array+3, entry.title, title_len);
  memcpy( byte_array+3+title_len, entry.data, data_len);

  ret = get_free_data_key();

  persist_write_data(ret, byte_array, byte_array_len);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "add entry: key %i", ret);

  free(byte_array);
  menu_window_reload();

  return;
}

struct card_entry get_entry( int key )
{
  APP_LOG(APP_LOG_LEVEL_DEBUG, "get entry: key %i", key);
  struct card_entry entry;
  entry.data_type = UNDEFINED;
  const int BUF_SIZE = 128;
  char byte_array[BUF_SIZE];
  unsigned int title_len, data_len;

  if ( !persist_exists(key) )
    return entry;
  else
    persist_read_data(key, byte_array, BUF_SIZE);

    entry.data_type = byte_array[0];
    title_len       = byte_array[1];
    data_len        = byte_array[2];

    entry.title = malloc(title_len+1);
    entry.data  = malloc(data_len +1);

    memcpy( entry.title, &byte_array[3],           title_len );
    memcpy( entry.data,  &byte_array[3+title_len], data_len  );
    entry.title[title_len] = 0;
    entry.data[data_len]   = 0;

    return entry;
}
