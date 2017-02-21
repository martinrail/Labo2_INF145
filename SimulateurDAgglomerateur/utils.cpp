/*
Nom du fichier: simulateur_capteurs.cpp
Auteur: Fred Simard, ETS, Hiver 2017
Description: Fonctions utilitaires qui ne sont pas li� � un module en particulier
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 
* init_alea
* @brief Fonction pour initialiser le g�n�rateur al�atoire. 
*/
void init_alea(){
  srand((unsigned int)time(NULL));
}

/*
* reel_alea
* @brief retourne un r�el entre 0 et 1
*/
double reel_alea() {
	return (double)rand() / RAND_MAX;
}

/*
* delai
* @brief Effectuer un delai de "msec" millisecondes.
* @param msec(int), le temps � attendre en millisecondes
*/
void delai(int msec){
  double heure_depart = clock();    //fonction-horloge de <time.h>
  double temps;

  /* Boucle vide selon le nombre de millisecondes d�sir� */
  do {
    temps = (clock() - heure_depart)/CLOCKS_PER_SEC;  //constante de <time.h>
  }while ((temps * 1000) < msec); 
}
