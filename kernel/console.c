#include <n7OS/console.h>
#include <n7OS/handler.h>
#include <n7OS/cpu.h>
#include <stdio.h>

uint16_t *scr_tab;

// Ligne courante de l'écran
int ligne;

// se souvenir de la taille de la derniere ligne remplace la colonne
uint32_t taille_ligne[VGA_HEIGHT];

extern uint32_t curr_time;

void init_console()
{
    scr_tab = (uint16_t *)SCREEN_ADDR;
    ligne = 1;
    taille_ligne[ligne] = 0;
    clear_screen(1);
    console_putheader();
    console_puttime();
}

/**
 * Remonte l'écran de 1 ligne quand on arrive en bas
 */
void scroll()
{
    // On copie les 24 dernieres lignes sur les 24 premières
    for (int i = 1; i < VGA_HEIGHT - 1; i++)
    {
        for (int j = 0; j < VGA_WIDTH; j++)
        {
            int posN = VGA_WIDTH * i + j;
            int posO = VGA_WIDTH * (i + 1) + j;
            scr_tab[posN] = scr_tab[posO];
        }
        taille_ligne[i] = taille_ligne[i + 1];
    }
    // On libère la derniere ligne
    for (int j = 0; j < VGA_WIDTH; j++)
    {
        int pos = VGA_WIDTH * (VGA_HEIGHT - 1) + j;
        scr_tab[pos] = CHAR_COLOR << 8 | ' ';
    }
    taille_ligne[VGA_HEIGHT - 1] = 0;
}

/**
 * Calcule la position du curseur
 */
void compute_pos()
{
    if (taille_ligne[ligne] == VGA_WIDTH)
    {
        ligne++;
        taille_ligne[ligne] = 0;
    }
    if (ligne == VGA_HEIGHT)
    {
        scroll();
        ligne--;
    }
}

/**
 * Affiche le curseur à la bonne position
 */
void set_cursor()
{
    int pos = VGA_WIDTH * ligne + taille_ligne[ligne];
    outb(CMD_LOW, PORT_CMD);
    outb(pos & 255, PORT_DATA);
    outb(CMD_HIGH, PORT_CMD);
    outb(pos >> 8, PORT_DATA);
}

/**
 * Vide l'écran
 */
void clear_screen(int full)
{
    ligne = 1;
    if (full)
        ligne = 0;
    taille_ligne[ligne] = 0;
    // On affiche des espaces partout
    for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - (1 - full)); i++)
    {
        console_putchar(' ');
    }
    ligne = 0;
    taille_ligne[ligne] = 0;
}

/**
 * Affiche une tabulation
 */
void tab()
{
    for (int i = 0; i < 4; i++)
    {
        // Permet d'éviter que la tabulation continue sur la ligne suivante
        if (taille_ligne[ligne] == VGA_WIDTH - 1)
        {
            console_putchar(' ');
            break;
        }
        console_putchar(' ');
    }
}

/**
 * Affiche le header vide
 */
void console_putheader()
{
    for (int i = 0; i < VGA_WIDTH; i++)
    {
        scr_tab[i] = HEADER_CHAR_COLOR << 8 | 0;
    }
}

/**
 * Affiche le temps sur le header
 */
void console_puttime()
{
    char header[16];

    sprintf(header, "AntOS - %02d:%02d:%02d", (((curr_time % (3600000 * 24)) / 1000) / 60) / 60, ((curr_time % 3600000) / 1000) / 60, (curr_time % 60000) / 1000);

    for (int col = 0; col < HEADER_SIZE; col++)
    {
        scr_tab[col] = HEADER_CHAR_COLOR << 8 | header[col];
    }
}

void console_putchar(const char c)
{
    int pos = VGA_WIDTH * ligne + taille_ligne[ligne];
    if ((31 < c) && (c < 127))
    {
        scr_tab[pos] = CHAR_COLOR << 8 | c;
        taille_ligne[ligne]++;
    }
    if (c == 8)
    {
        if (taille_ligne[ligne] > 0)
        {
            taille_ligne[ligne]--;
            scr_tab[pos - 1] = CHAR_COLOR << 8 | ' ';
        }
        else
        {
            if (ligne > 1)
            {
                taille_ligne[ligne] = VGA_WIDTH - 1;
                scr_tab[pos - 1] = CHAR_COLOR << 8 | ' ';
                ligne--;
            }
        }
    }
    if (c == 9)
    { // tab
        tab();
    }
    if (c == 10)
    { // retour à la ligne
        ligne++;
        taille_ligne[ligne] = 0;
    }
    if (c == 12)
    { // Vider l'écran
        clear_screen(0);
    }
    if (c == 13)
    { // retour au début de la ligne
        taille_ligne[ligne] = 0;
    }
    compute_pos();
}

void console_putbytes(const char *s, int len)
{
    for (int i = 0; i < len; i++)
    {
        console_putchar(s[i]);
        set_cursor();
    }
}