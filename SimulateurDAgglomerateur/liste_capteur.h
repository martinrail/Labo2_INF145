#ifndef LISTE_CAPTEUR
#define LISTE_CAPTEUR
/*
Nom du fichier: liste_capteur.h
Auteur: Martin Rail, ETS, Hiver 2017
Description: Fichier source contenant la liste de capteurs
*/

#include "liste_capteur.cpp"

/*
* init_liste_capteur
* @brief initie la liste de capteurs
* @param liste_capteur * pointeur_liste, le pointeur de la liste
* @param int taille, taille de la liste
*/
void init_liste_capteur(liste_capteur * pointeur_liste, int taille);

capteur trouver_ajouter_capteur(liste_capteur * pointeur_liste, char ID);

void ajouter_mesure(capteur * pointeur_capteur, t_message message);

#endif