/*
Nom du fichier: programme_principal.cpp
Auteur: Fred Simard, ETS, Hiver 2017
Description: Lance le simulateur de capteurs et l'agglomérateur de données
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"
#include "simulateur_capteurs.h"

#define UN_CAPTEUR 1

static int demande_nb_capteurs();
static t_mode_operation selection_mode_operation();

/*
* main
* @brief main du projet
*/
int main() {

	t_simulateur_capteur simul_capteurs;
	t_mode_operation mode_operation;

	int nb_capteurs = UN_CAPTEUR; // valeur par défaut

	// initialise générateur nombre aléatoire
	init_alea();

	// configuration de la simulation
	char* nom_fichier = "Enregistrement.txt";

	// demande le mode d'opération à l'utilisateur
	mode_operation = selection_mode_operation();

	// si le mode d'opération n'est pas avec un seul capteur
	if (mode_operation != UN_SEUL_CAPTEUR) {
		// on demande le nombre de capteurs
		nb_capteurs = demande_nb_capteurs();
	}

	// lance le simulateur de capteur
	initialiser_simulateur_capteurs(&simul_capteurs, mode_operation, nb_capteurs);

	// initialise et lance l'agglommérateur
	
	// libere la mémoire du simulateur de capteurs
	liberer_simulateur_capteurs(&simul_capteurs);

	// affiche un message à l'utilisateur
	printf("/******************************/\n");
	printf("/*    FIN DE LA SIMULATION    */\n");
	printf("/******************************/\n");

	system("pause");
	return EXIT_SUCCESS;
}



/*
* t_mode_operation selection_mode_operation() 
* @brief demande le mode d'operation à l'utilisateur
* @return (t_mode_operation) le mode d'opération
*/
static t_mode_operation selection_mode_operation() {

	int choix = 0;

	// affiche le menu
	printf("Selectionner le mode d'operation\n");
	printf("0 - UN_SEUL_CAPTEUR\n");
	printf("1 - MESURES_EN_SEQUENCE\n");
	printf("2 - MESURES_EN_SEQUENCE_PAR_CAPTEUR\n");
	printf("3 - NORMAL\n");
	printf("Entrer votre choix:\n");
	scanf(" %d", &choix);

	// interroge l'utilisateur
	while(choix < 0 || choix > 3){
		printf("Entre 0 et 3, s.v.p.:\n");
		scanf(" %d", &choix);
	}

	return (t_mode_operation)choix;
}


/*
* int demande_nb_capteurs()
* @brief demande le nombre de capteurs de la simulation à l'utilisateur
* @return (int) le nombre de capteurs
*/
static int demande_nb_capteurs() {

	int choix = 0;

	// isntruction à l'utilisateur
	printf("Entrer le nombre de capteur [1-5]:\n");
	scanf(" %d", &choix);

	// interroge l'utilisateur
	while(choix < 1 || choix > 5){
		printf("Entre 1 et 5, s.v.p.:\n");
		scanf(" %d", &choix);
	}

	return choix;
}

