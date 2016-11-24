// Precompiled parameters
#define CONF_NAME0          "wash0.conf"
#define CONF_NAME1          "wash1.conf"
#define ORDER_NAME          "wash.order"
#define MAX_DISH_ID         255
#define MAX_TABLE_LIMIT     65535

#define dish_t              unsigned char

// Declarations
void parent_proc ( );
void child_proc  ( );
void getconf     ( const char*, int );
void getlim      ( );