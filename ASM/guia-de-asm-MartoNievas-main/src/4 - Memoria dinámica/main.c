#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../test-utils.h"
#include "Memoria.h"

int main() {
  /* Acá pueden realizar sus propias pruebas */

  char *a = "sar";
  char *b = "sar";
  strCmp(a, b);
  return 0;
}
