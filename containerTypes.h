/** @file
 * @brief Conriene tipos de contenedores genericos
 */
#ifndef CONTAINERTYPES_H
#define CONTAINERTYPES_H

#include <stdio.h>
#include "snake_global.h"

/**
 * @brief Estructura basica de una lista enlazada simple
**/
typedef struct linkList{
    linkList* next;/**< Elemento siguiente de la lista */
}linkList;
/**
 * @brief addNodeBefore Añade el nodo next despues de first
 * @param first Punto de la lista a partir del que se inerta el nodo next
 * @param next Nodo a insertar
 */
void addNodeBefore(linkList* first,linkList* next);
/**
 * @brief insertNode inserta un nodo
 * @param listNode
 * @param node
 */
void insertNode(linkList* listNode,linkList* node);
/**
 * @brief delNextNode borra el elemento node de la lista
 * @param node nodo a eliminar de la lista
 * @return
 */
linkList* delNextNode(linkList *node);
/**
 * @brief vector Estructura que representa a un vector dinamico
 *
 */
typedef struct{
    byte* buff;/**< Puntero a la zona de memoria donde se guardan los datos */
    ulong typeSize;/**< Tamaño del cada elemento del array */
    ulong nElementMax;/**< Numero maximo de elemento del array */
    ulong nElementUsaje;/**< Numero de elemento del array que estan en uso */
}vector;
/**
 * @brief recupera el puntero del buffer y lo convierte al typo especificado
 **/
#define getDataVector(v,type) ((type*)(&v)->buff)
/**
 * @brief initVector Inicializa un vector
 * @param v Vector
 * @param nElementMax Numero de elementos Maximo inicial del array
 * @param typeSize Tamaño del cada elemento del array
 */
void initVector(vector* v,ulong nElementMax,ulong typeSize = sizeof(byte));
/**
 * @brief uinitVector Livera la memoria asociada al vector
 * @param v Vector
 */
void uinitVector(vector* v);
/**
 * @brief resizeVector Cambia el numero maximo de elementos del vector
 *
 * Si el nuevo maximo es menor que el anterior, reduce el espacio del array y desecha los elementos que s quedan fuera
 * @param v Vector
 * @param nElementMax Nuevo maximo
 */
void resizeVector(vector* v,ulong nElementMax);

#endif // CONTAINERTYPES_H
