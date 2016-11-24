#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define  DEBUG
#include "alerts.h"
#include "wash.h"

// Ресурсы
int timing[2][MAX_DISH_ID+1];
int semid;
int TABLE_LIMIT;
int pip[2],  // Прямой канал
    rev[2];  // Обратная связь


int main ( int argc, char **argv )
{
  // Конфиги
  getconf ( CONF_NAME0, 0 );
  getconf ( CONF_NAME1, 1 );
  getlim  ( );

  // Инициализация
  if ( ( pipe ( pip ) == -1 ) ||
       ( pipe ( rev ) == -1 ) )
    ERROR ( "Pipe failed" );

  // Fork
  switch ( fork ( ) )
  {
    case -1 :  ERROR ( "Fork failed" );
    case  0 :  child_proc ( );
    default :  parent_proc ( );
  }
}


void child_proc ( )
{
  close ( rev[1] );
  PRINT ( "Worker 0" );

  // Читаем задание
  FILE *ord = fopen ( ORDER_NAME, "r" );
  if ( !ord )
    ERROR ( "IO error %d @ file %s:  %s", errno, ORDER_NAME, strerror ( errno ) );

  int cur_id, cur_cnt,  // Текущая посуда
      table_cnt = 0;    // Заполненность стола
  dish_t ds;

  while ( fscanf ( ord, "%d : %d", &cur_id, &cur_cnt ) > 0 )
  {
    if ( ( cur_id < 0 ) || ( cur_id > MAX_DISH_ID) )
      ERROR ( "DISH_ID = %d (MAX_DISH_ID = %d)", cur_id, MAX_DISH_ID );

    ds = cur_id;
    while ( cur_cnt-- )
    {
      // Берём тарелку
      PRINT ( "> %d", cur_id );
      sleep ( timing[0][cur_id] );

      // Стол заполнен - ждём
      if ( table_cnt == TABLE_LIMIT )
      {
        PRINT ( "wait" );
        size_t cnt;
        if ( read ( rev[0], &cnt, sizeof ( size_t ) ) == -1 )
          ERROR ( "IO error %d @ pipe: %s", errno, strerror ( errno ) );
        table_cnt -= cnt;
      }

      // Ставим на стол
      if ( write ( pip[1], &ds, 1 ) != 1 )
        ERROR ( "IO error %d @ pipe: %s", errno, strerror ( errno ) );
      ++table_cnt;
    }
  }

  PRINT ( EXIT "Worker 0" );
  exit ( EXIT_SUCCESS );
}

void parent_proc ( )
{
  close ( pip[1] );
  PRINT ( "Worker 1" );

  size_t cur_id; dish_t ds;
  const size_t cnt = 1;

  for ( ; ; )
  {
    // Ждём работу
    switch ( read ( pip[0], &ds, 1 ) )
    {
      case -1 :  ERROR ( "IO error %d @ pipe: %s", errno, strerror ( errno ) );
      case  0 :  PRINT ( EXIT "Worker 1" );
                 exit ( EXIT_SUCCESS );
      // ======================================================
      default :  cur_id = ds; PRINT ( ">> %lu", cur_id );

                 // Обратная связь
                 if ( write ( rev[1], &cnt, sizeof ( size_t ) ) != sizeof ( size_t ) )
                   ERROR ( "IO error %d @ pipe: %s", errno, strerror ( errno ) );

                 sleep ( timing[1][ds] );
      // ======================================================
    }
  }
}

void getconf ( const char *conf_name, int x )
{
  PRINT ( "Config file: %s", conf_name );
  FILE *config = fopen ( conf_name, "r" );
  if ( !config )
    ERROR ( "IO error %d @ file %s:  %s", errno, conf_name, strerror ( errno ) );

  int a, b, _cnt_ = 0;
  while ( fscanf ( config, "%d : %d", &a, &b ) > 0 )
    if ( ( a < 0 ) || ( a > MAX_DISH_ID ) )
      ERROR ( "DISH_ID = %d (MAX_DISH_ID = %d)", a, MAX_DISH_ID )
    else if ( b < 0 )
      ERROR ( "Negative time is prohibited by Einstein!" )
    else
    {
      ++_cnt_;
      timing[x][a] = b;
    }

  PRINT ( OK "%d entries", _cnt_ );
  fclose ( config );
}

void getlim ( )
{
  PRINT ( "Get TABLE_LIMIT" );
  char *tmp = getenv ( "TABLE_LIMIT" );
  if ( !tmp )
    ERROR ( "TABLE_LIMIT is not set" );
  TABLE_LIMIT = atoi ( tmp );

  if ( ( TABLE_LIMIT <= 0 ) || ( TABLE_LIMIT > MAX_TABLE_LIMIT ) )
    ERROR ( "TABLE_LIMIT = %d (MAX_TABLE_LIMIT = %d)", TABLE_LIMIT, MAX_TABLE_LIMIT );
  PRINT ( OK "= %d", TABLE_LIMIT );
}
