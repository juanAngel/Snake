/** @file
 * Funciones para la gestion de records
 * @author juan angel
*/
#ifndef RECORDS_H
#define RECORDS_H
#include "snake_global.h"
#include "containerTypes.h"

/**
 * @brief Lista de Records
*/
typedef struct recodList:linkList{
    char* PlayerName;/**< Nombre del jugador */
    uint Points;/**< Puntos conseguidos */
}recodList;
/**
 * @brief loadRecord Carga los record del archivo
 * @return Lista de records
 */
recodList* loadRecord();
/**
 * @brief saveRecord Guarda en el archivo de recors la lista de records
 * @param r Lista de records
 */
void saveRecord(recodList* r);
/**
 * @brief freeRecordList Libera la lista de recors
 * @param r Lista de records
 */
void freeRecordList(recodList* r);
/**
 * @brief manageRecord Comprueva y gestiona si hay un nuevo record
 *
 * Si la nueva puntuacion esta entre los 5 primeros, modifica la lista de records para añadir el nuevo y dejar solo 5
 * @param records Lista de records
 * @param newPoint Nueva puntuacion
 * @param playerName Nombre del jugador
 * @param recordIndex Si hay un record, aqui se guarda la posicion en el ranking que ocupa
 * @return La lista modificada si hay record, si no lo hay es la misma lista pasada como parametro
 */
recodList* manageRecord(recodList* records, uint newPoint, const char* playerName, int* recordIndex);


#endif // RECORDS_H
