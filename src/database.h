#define MAX_DATA_KEY 30

typedef enum {
  BARCODE,
  QRCODE,
  UNDEFINED
} data_type;

typedef struct card_entry {
  data_type data_type;
  char *title;
  char *data;
} card_entry;

int get_free_data_key();
uint16_t num_entries();
struct card_entry get_nth_entry(unsigned int n);
void clear_persist();
void add_entry(struct card_entry entry);
struct card_entry get_entry( int key );
