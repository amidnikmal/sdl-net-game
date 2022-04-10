#include "utils.h"

void print_uuid(uuid_t uuid, char *msg)
{
  char uuid_str[37];
  uuid_unparse_lower(uuid, uuid_str);

  if (msg != NULL)
  {
    printf("%s %s\n", msg, uuid_str);
    return;
  }

  printf("uuid = %s\n", uuid_str);
}
