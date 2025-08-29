#ifndef TEST_H
#define TEST_H

#include "../test_utils/test-utils.h"
#include "./ejs.h"


/**
 * El mensaje [DONE] escrito en verde.
 */
#define DONE "[\033[32;1mDONE\033[0m] "

/**
 * El mensaje [FAIL] escrito en rojo.
 */
#define FAIL "[\033[31;1mFAIL\033[0m] "

/**
 * El mensaje [INFO] escrito en amarillo.
 */
#define INFO "[\033[33;1mINFO\033[0m] "

/**
 * El mensaje [SKIP] escrito en magenta.
 */
#define SKIP "[\033[95;1mSKIP\033[0m] "

caso_t crear_caso(uint32_t id, uint32_t nivel, const char* categoria, uint16_t estado);

void liberar_casos(caso_t* casos, int largo);


#endif