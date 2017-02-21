#ifndef SIMULATEUR_CAPTEURS_H
#define SIMULATEUR_CAPTEURS_H
/*
Nom du fichier: simulateur_capteurs.h
Auteur: Fred Simard, ETS, Hiver 2017
Description: Ce module définit le simulateur de capteurs. Il simule le comportement de 1 ou plusieurs capteurs
			 tel que spécifié dans le document de conception (fictif) et sert à émuler les capteurs pour
			 aider au développement de l'agglomérateur.
*/

#include "messages.h"

#define PROB_EVENEMENT 0.5 // probabilité qu'il y ait un événement capteur
#define FREQ_ECHANTILLONNAGE 20 // fréquence d'échantillonnage des capteurs
#define NB_MESURES_MAX 2000 // nb de mesures maximal qu'un capteur peut mémoriser


// liste des modes d'opération du simulateur de capteur
typedef enum s_mode_operation {
	UN_SEUL_CAPTEUR,				// ne simule qu'un seul capteur
	MESURES_EN_SEQUENCE,			// les mesures vont de 0++ independamment du capteur qui les rapportent
	MESURES_EN_SEQUENCE_PAR_CAPTEUR,// les mesures vont de 0++ par capteur
	NORMAL							// les mesures sont généré aléatoirement
}t_mode_operation;


// etats possible pour un capteur
typedef enum e_etat_comm {
	EN_OPERATION,
	ATTENTE_CENTRALE,
	TRANSMISSION_STATUS,
	TRANSMISSION_MESURES,
	ATTENTE_CONFIRMATION
}t_etat_comm;

// enregistrement définissant un capteur
typedef struct s_capteur {

	// nom du capteur, utilisé pour identifier l'origine des messages
	char id_capteur[LONGUEUR_ID_MAX];

	// etat du capteur
	t_etat_comm etat_courant;

	// utilise pour le mode generation par capteur
	int temp;

	// temps de la dernière connection
	time_t temps_derniere_transmission;

	// nombre de mesures et tableau de mesure
	int nb_mesures;
	double mesures[NB_MESURES_MAX];

	// couleur de l'affichage
	unsigned char couleur;

}t_capteur;


// enregistrement definissant le simulateur de capteur
typedef struct s_simulateur_capteur {
	int temp;
	int nb_capteurs;
	t_capteur* tab_capteurs;
	int mode_operation;
	t_message tampon_msg;
}t_simulateur_capteur;


/*
* initialiser_simulateur_capteurs
* @brief initialise le tableau de capteurs et tous les capteurs avec des valeurs par défaut
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @param mode_operation(t_mode_operation), le mode d'operation
* @param nb_capteurs(int), nombre de capteurs à simuler
*/
void initialiser_simulateur_capteurs(t_simulateur_capteur* simul_capteurs, t_mode_operation mode_operation, int nb_capteur);


/*
* evenement_capteurs
* @brief permet de savoir s'il y a un événement asynchrone qui a pris place
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @return 1 s'il y a un événement, 0 sinon
*/
unsigned char evenement_capteurs(t_simulateur_capteur* simul_capteurs);


/*
* simulateur_traitement_message
* @brief appel demandand au simulateur de traiter le message entré (équivalent d'un interrupt software)
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
*/
void simulateur_traitement_message(t_simulateur_capteur* simul_capteurs);

/*
* liberer_simulateur_capteurs
* @brief libere la memoire du simulateur de capteurs
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
*/
void liberer_simulateur_capteurs(t_simulateur_capteur* simul_capteurs);

#endif