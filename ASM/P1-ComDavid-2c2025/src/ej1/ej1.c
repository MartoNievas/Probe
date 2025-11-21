#include "../ejs.h"

// Vamos a definir una funcion auxiliar para verificar si un producto cumple las condiciones

uint32_t contar_productos_que_cumplen_condiciones(catalogo_t *catalogo)
{
    uint32_t res = 0;

    publicacion_t *curr = catalogo->first;

    while (curr != NULL)
    {
        producto_t *prod = curr->value;
        usuario_t *user = prod->usuario;
        if (prod->estado == 1 && user->nivel >= 1)
        {
            res += 1;
        }
        curr = curr->next;
    }
    return res;
}

producto_t *filtrarPublicacionesNuevasDeUsuariosVerificados(catalogo_t *catalogo)
{
    // Primero contamos la catidad de elementos que cumple
    uint32_t cantidad_que_cumplen = contar_productos_que_cumplen_condiciones(catalogo);

    // Ahora reservamos memoria
    producto_t **arr = (producto_t **)malloc(sizeof(producto_t *) * (cantidad_que_cumplen + 1));

    arr[cantidad_que_cumplen] = NULL;

    // El ultimo lo ponemos nulo

    // Ahora recorremos
    publicacion_t *curr = catalogo->first;
    uint32_t i = 0;
    while (curr != NULL)
    {
        producto_t *prod = curr->value;
        usuario_t *user = prod->usuario;
        if (prod->estado == 1 && user->nivel >= 1)
        {
            arr[i] = prod;
            i++;
        }
        curr = curr->next;
    }
    return (producto_t *)arr;
}
