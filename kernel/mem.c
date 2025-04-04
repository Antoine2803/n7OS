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
    uint32_t index = addr >> 12;
    uint32_t offset = index % 32;

    page_bitmap_table[index / 32] |= 1 << offset;
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
    uint32_t index = addr >> 12;
    uint32_t offset = index % 32;

    page_bitmap_table[index / 32] &= ~(1 << offset);
}

/**
 * @brief Fourni la première page libre de la mémoire physique tout en l'allouant
 *
 * @return uint32_t Adresse de la page sélectionnée
 */
uint32_t findfreePage()
{
    for (uint32_t page_index = 0; page_index < NB_PAGE_MEM; page_index++)
    {
        uint32_t index = page_index / 32;
        uint32_t offset = page_index % 32;
        uint32_t current = page_bitmap_table[index];
        if (!(current & (1 << (offset))))
        {
            setPage(page_index << 12);
            return page_index << 12;
        }
    }

    return -1;
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
        if (i % 7 == 0)
        {
            printf("  ");
        }

        printf("0x%08x ", page_bitmap_table[i]);

        if ((i + 1) % 7 == 0)
        {
            printf("\n");
        }
    }
}