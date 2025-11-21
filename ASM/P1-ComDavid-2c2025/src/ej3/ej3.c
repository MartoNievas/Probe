#include "../ejs.h"
#include <stdint.h>

usuario_t **
asignarNivelesParaNuevosUsuarios(uint32_t *ids, uint32_t cantidadDeIds,
                                 uint8_t (*deQueNivelEs)(uint32_t))
{

  if (cantidadDeIds == 0)
    return NULL;

  usuario_t **res = malloc(sizeof(usuario_t *) * cantidadDeIds);

  for (uint32_t i = 0; i < cantidadDeIds; i++)
  {
    uint8_t user_level = (*deQueNivelEs)(ids[i]);
    usuario_t *new_user = malloc(sizeof(usuario_t *));
    new_user->id = ids[i];
    new_user->nivel = user_level;
    res[i] = new_user;
  }
  return res;
}
