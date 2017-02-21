#ifndef UTILS_H
#define UTILS_H
/*
Nom du fichier: utils.h
Auteur: Fred Simard, ETS, Hiver 2017
Description: Fonctions utilitaires qui ne sont pas li� � un module en particulier
*/

/* 
* init_alea
* @brief Fonction pour initialiser le g�n�rateur al�atoire. 
*/
void init_alea();

/*
* reel_alea
* @brief retourne un r�el entre 0 et 1
*/
double reel_alea();

/*
* delai
* @brief Effectuer un delai de "msec" millisecondes.
*/
void delai(int msec);

#endif