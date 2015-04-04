#include <pebble.h>
#include "database.h"

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

void free_entry_db()
{
  int i;
  int num_entries = get_free_data_key();
  for (i=1; i<num_entries; i++)
  {
    free(entry_db[i-1].title);
    free(entry_db[i-1].data);
  }
  free(entry_db);
}

void setup_entry_db()
{
  int i;
  int num_entries = get_free_data_key();
  char *data_entry = malloc(32);
  int name_len = 0;
  
  if (entry_db != NULL)
    free_entry_db();
  
  entry_db = malloc( sizeof(card_entry)*num_entries  );
  for (i=1; i<num_entries; i++)
  {
    persist_read_data(i, data_entry, 32);
    
    APP_LOG( APP_LOG_LEVEL_DEBUG, "data_entry: %s", data_entry);
    entry_db[i-1].data_type = BARCODE;
    
    name_len = strlen(data_entry);
    APP_LOG( APP_LOG_LEVEL_DEBUG, "name_len: %i", name_len );
    
    entry_db[i-1].title = malloc( name_len+1 );    
    strncpy( entry_db[i-1].title, data_entry, name_len );

    entry_db[i-1].data = malloc( strlen(data_entry+11) + 1 );
    strncpy( entry_db[i-1].data, data_entry+11, strlen(data_entry+11) );
    
    APP_LOG( APP_LOG_LEVEL_DEBUG, "entry: %s, %s", entry_db[i-1].title, entry_db[i-1].data );
  }
}

void clear_persist()
{
  unsigned int i;
  for (i=0; i<MAX_DATA_KEY; i++)
  {
    if (persist_exists(i))
      persist_delete(i);
  }
}

void add_entry(char *data)
{
  unsigned data_len = 32;
  char *byte_array;
  int ret;

  byte_array = malloc(data_len);

  ret = persist_read_int( 0 );
  if (ret == 0)
    persist_write_int(0, 1);
  
  memcpy( byte_array, data, 32);
  
  ret = get_free_data_key();
  persist_write_data(ret, byte_array, data_len);
  
  free(byte_array);
}

struct card_entry get_entry( int key )
{
	return entry_db[key];
}
