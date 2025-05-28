#include <n7OS/minishell.h>
#include <n7OS/keyboard.h>
#include <n7OS/process.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

/**
 * Affiche les commandes possible du minishell
 */
void print_help()
{
    printf("Available commands:\n");
    printf("\thelp - Show help message\n");
    printf("\tclear - Clear console screen\n");
    printf("\tpoweroff - Power off\n");
    printf("\tps - Show all process\n");
    printf("\tblock X - Block process X\n");
    printf("\tunblock X - Unblock process X\n");
    printf("\tkill X - Kill process X\n");
}

/**
 * Teste si la commande cmd est une commande valide
 */
void check_cmd(const char *cmd)
{
    if (strcmp(cmd, "help") == 0)
    {
        print_help();
    }
    else if (strcmp(cmd, "clear") == 0)
    {
        printf("\f");
    }
    else if (strcmp(cmd, "poweroff") == 0)
    {
        printf("Power off...\n");
        shutdown(1);
    }
    else if (strcmp(cmd, "ps") == 0)
    {
        print_process();
    }
    else
    {
        // Récuperer le debut de la commande (on récupère tous les caractères jusqu'à un espace)
        char *s = strtok(cmd, " ");
        //  (on récupère tous les caractères jusqu'à un espace)
        char *n = strtok(NULL, " ");

        uint8_t pid = atoi(n);

        if (strcmp(s, "block") == 0)
        {
            block_proc(pid);
        }
        else if (strcmp(s, "unblock") == 0)
        {
            unblock_proc(pid);
        }
        else if (strcmp(s, "kill") == 0)
        {
            terminate_proc(pid);
        }
        else
        {
            printf("Unknown command: %s\n", cmd);
            print_help();
        }
    }
}

/**
 * Récupère les caractères du clavier pour les mettre dans un buffer
 * Si le caractère récupéré est \n ou \r on regarde si la commande entrée est valide
 */
char *read_cmd()
{
    static char cmd[BUFFER_SIZE];
    uint8_t i = 0;
    while (1)
    {
        char c = kgetch();
        if (c != 0)
        {
            if (c == '\n' || c == '\r')
            {
                cmd[i] = '\0';
                printf("\n");
                return cmd;
            }
            else if (c == '\b')
            {
                if (i != 0)
                {
                    i--;
                    cmd[i] = '\0';
                    printf("\b");
                }
            }
            else if (i < BUFFER_SIZE - 1)
            {
                cmd[i++] = c;
                printf("%c", c);
            }
        }
    }
}

/**
 * Fonction pour le processus du minishell
 */
void minishell()
{
    printf("Welcome to AntOS\n");
    printf("Type 'help' to see all the commands.\n");
    while (1)
    {
        printf("AntOS> ");
        char *cmd = read_cmd();
        if (strlen(cmd) != 0)
        {
            check_cmd(cmd);
        }
    }
}