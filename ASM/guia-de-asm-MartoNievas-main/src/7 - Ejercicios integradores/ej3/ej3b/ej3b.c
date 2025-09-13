#include "../ejs.h"
#include <stdint.h>
#include <string.h>
#include <unistd.h>

void resolver_automaticamente(funcionCierraCasos_t *funcion,
                              caso_t *arreglo_casos, caso_t *casos_a_revisar,
                              int largo) {
  if (funcion == NULL || arreglo_casos == NULL || casos_a_revisar == NULL) {
    return;
  }

  int i1 = 0;
  for (int i = 0; i < largo; i++) {
    // Verificar que el usuario no sea NULL
    if (arreglo_casos[i].usuario == NULL) {
      continue; // Saltar casos con usuario NULL
    }

    uint32_t nivel_actual = arreglo_casos[i].usuario->nivel;

    // Solo procesar niveles 1 y 2
    if (nivel_actual == 1 || nivel_actual == 2) {
      int res_funcion = (*funcion)(&arreglo_casos[i]);

      if (res_funcion == 1) {
        arreglo_casos[i].estado = 1;
      } else {
        // Verificar categorías correctamente
        if (strncmp(arreglo_casos[i].categoria, "CLT", 4) == 0 ||
            strncmp(arreglo_casos[i].categoria, "RBO", 4) == 0) {
          arreglo_casos[i].estado = 2;
        }
        // Si no cumple ninguna condición, el estado permanece igual
      }

      // Solo copiar a casos_a_revisar los casos procesados (niveles 1 y 2)
      casos_a_revisar[i1] = arreglo_casos[i];
      i1++;
    }
  }
}
