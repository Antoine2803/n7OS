#include <n7OS/mem.h>
#include <stdio.h>

/**
 * Bit map des pages marqués en mémoire
 */
uint32_t page_bitmap_table[SIZE_BITMAP_TABLE];

/**
 * @brief Marque la page allouée
 *
 * Lorsque la page a été choisie, cette fonction permet de la marquer allouée
 *
 * @param addr Adresse de la page à allouer
 */
void setPage(uint32_t addr)
{
    int index = addr / 32;
    int offset = addr % 32;
    int value = 1 << offset;

    page_bitmap_table[index] |= value;
}

/**
 * @brief Désalloue la page
 *
 * Libère la page allouée.
 *
 * @param addr Adresse de la page à libérer
 */
void clearPage(uint32_t addr)
{
    int index = addr / 32;
    int offset = addr % 32;
    int value = ~(1 << offset);

    page_bitmap_table[index] &= value;
}

/**
 * @brief Fourni la première page libre de la mémoire physique tout en l'allouant
 *
 * @return uint32_t Adresse de la page sélectionnée
 */
uint32_t findfreePage()
{
    for (int addr = 0; addr < NB_PAGE_MEM; addr++)
    {
        int index = addr / 32;
        int offset = index % 32;
        uint32_t current = page_bitmap_table[index];
        if ((current & (1 << (offset))) == 0)
            return addr;
    }

    return LAST_MEMORY_INDEX;
}

/**
 * @brief Initialise le gestionnaire de mémoire physique
 *
 */
void init_mem()
{
    for (int i = 0; i < SIZE_BITMAP_TABLE; i++)
    {
        page_bitmap_table[i] = 0x0;
    }
}

/**
 * @brief Affiche l'état de la mémoire physique
 *
 */
void print_mem()
{
    for (int i = 0; i < SIZE_BITMAP_TABLE; i++)
    {
        printf("0x%08x ", page_bitmap_table[i]);

        if ((i + 1)% 7 == 0) {
            printf("\n");
        }
    }
}