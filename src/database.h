#define MAX_DATA_KEY 30

typedef enum {
  BARCODE,
  QRCODE,
} data_type;

typedef struct card_entry {
  data_type data_type;
  char *title;
  char *data;
} card_entry;

int get_free_data_key();
void free_entry_db();
void setup_entry_db();
void clear_persist();
void add_entry(char *data);
struct card_entry get_entry( int key );