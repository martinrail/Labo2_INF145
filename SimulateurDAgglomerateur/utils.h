#ifndef UTILS_H
#define UTILS_H
/*
Nom du fichier: utils.h
Auteur: Fred Simard, ETS, Hiver 2017
Description: Fonctions utilitaires qui ne sont pas lié à un module en particulier
*/

/* 
* init_alea
* @brief Fonction pour initialiser le générateur aléatoire. 
*/
void init_alea();

/*
* reel_alea
* @brief retourne un réel entre 0 et 1
*/
double reel_alea();

/*
* delai
* @brief Effectuer un delai de "msec" millisecondes.
*/
void delai(int msec);

#endif