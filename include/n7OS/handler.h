#ifndef __HANDLER_IT_H__
#define __HANDLER_IT_H__

// Initialise les différents handlers
void init_handlers();

// Fonctions des handlers des différentes intéruptions
void handler_IT_50_C();
void handler_IT_32_C();
void handler_IT_33_C();

#endif