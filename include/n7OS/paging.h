/**
 * @file paging.h
 * @brief Gestion de la pagination dans le noyau
 */
#ifndef _PAGING_H
#define _PAGING_H

#include <inttypes.h>

#define PRESENT 1;

/*         Table de page         */

/**
 * @brief Une entrée dans la table de page peut être manipulée en utilisant
 *        la structure page_table_entry_t ou directement la valeur
 */
typedef union
{
    /**
     * @brief Description d'une ligne de la table de page
     *
     */
    struct
    {
        uint32_t addr : 20; // Adresse de la page
        uint8_t AVAIL : 3;  //
        uint8_t RSVD2 : 2;  // Réservé
        uint8_t D : 1;      // Dirty
        uint8_t A : 1;      // Accessed
        uint8_t RSVD1 : 2;  // Réservé
        uint8_t U : 1;      // Mode
        uint8_t W : 1;      // Accéssibilité
        uint8_t P : 1;      // Présence
    };
    uint32_t value;
} PTE; // PTE = Page Table Entry

/**
 * @brief Une table de page (PageTable) est un tableau de descripteurs de page
 *
 */
typedef PTE *PageTable;

/*         Répertoire de page         */

/**
 * @brief Une entrée dans le répertoire de page peut être manipulée en utilisant
 *        la structure page_directory_entry_t ou directement la valeur
 */
typedef union
{
    /**
     * @brief Description d'une ligne du répertoire de page
     *
     */
    struct
    {
        uint32_t addr : 20; // Adresse de la page table
        uint16_t RSVD : 9;  // Reserve
        uint8_t U : 1;      // Mode
        uint8_t W : 1;      // Accéssibilité
        uint8_t P : 1;      // Présence
    };
    uint32_t value;
} PDE; // PDE = Page Directory Entry

/**
 * @brief Une table de page (PageDirectory) est un tableau de descripteurs de page
 *
 */
typedef PDE *PageDirectory;

typedef union
{
    /**
     * @brief Structure d'une adresse
     */
    struct
    {
        uint16_t indice_page : 12;
        uint16_t indice_table : 10;
        uint16_t indice_rep : 10;
    };
    uint32_t value;
} virtual_addr;

/**
 * @brief Cette fonction initialise le répertoire de page, alloue les pages de table du noyau
 *        et active la pagination
 *
 */
void initialise_paging();

/**
 * @brief Cette fonction alloue une page de la mémoire physique à une adresse de la mémoire virtuelle
 *
 * @param address       Adresse de la mémoire virtuelle à mapper
 * @param is_writeable  Si is_writeable == 1, la page est accessible en écriture
 * @param is_kernel     Si is_kernel == 1, la page ne peut être accédée que par le noyau
 * @return PageTable    La table de page modifiée
 */
PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel);
#endif