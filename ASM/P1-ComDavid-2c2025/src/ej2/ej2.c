#include "../ejs.h"
#include <string.h>
// Vamos con la funcion auxiliar

void removerAparicionesPosterioresDe(publicacion_t *publicacion)
{
   producto_t *prodOriginal = publicacion->value;
   usuario_t *userOriginal = prodOriginal->usuario;
   publicacion_t *prev = publicacion;
   publicacion_t *curr = publicacion->next;

   while (curr != NULL)
   {

      producto_t *prodCurr = curr->value;
      usuario_t *userCurr = prodCurr->usuario;

      if (strncmp(prodCurr->nombre, prodOriginal->nombre, 25) == 0 && userOriginal->id == userCurr->id)
      {
         publicacion_t *temp = curr;
         free(temp->value);
         curr = curr->next;

         prev->next = curr;
         free(temp);
      }
      else
      {
         prev = curr;
         curr = curr->next;
      }
   }
}

catalogo_t *removerCopias(catalogo_t *h)
{
   publicacion_t *curr = h->first;

   while (curr != NULL)
   {
      removerAparicionesPosterioresDe(curr);
      curr = curr->next;
   }
   return h;
}
