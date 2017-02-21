/*
Nom du fichier: interface.cpp
Auteur: Fred Simard, ETS, Hiver 2017
Description: Interface definissant le liens entre les capteurs et l'agglomérateur.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include "interface.h"
#include "simulateur_capteurs.h"

#define COULEUR_AGGLO 3
#define COULEUR_DEFAUT 7

// mémoire tampon de l'interface
static void afficher_message_vers_capteur(t_message *msg);

/*
* detection_evenement
* @brief permet de savoir s'il y a un événement asynchrone
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @retourne 1 s'il y a un événement, et 0 sinon
*/
unsigned char detection_evenement(t_simulateur_capteur* simul_capteurs) {
	return evenement_capteurs(simul_capteurs);
}

/*
* envoyer_vers_capteur
* @brief permet d'envoyer un message vers un capteur
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @param msg(t_message*) message à envoyer
*/
void envoyer_vers_capteur(t_simulateur_capteur* simul_capteurs, t_message *msg) {
	memcpy(&simul_capteurs->tampon_msg, msg, sizeof(t_message));
	afficher_message_vers_capteur(&simul_capteurs->tampon_msg);
	simulateur_traitement_message(simul_capteurs);
}

/*
* recevoir_message_de_capteur
* @brief permet de recevoir un message envoyé par un capteur
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @param msg(t_message*) tampon où copier le message
*/
void recevoir_message_de_capteur(t_simulateur_capteur* simul_capteurs, t_message *msg) {
	memcpy(msg, &simul_capteurs->tampon_msg, sizeof(t_message));
}


/*
* recevoir_message_d_agglomerateur
* @brief permet de recevoir un message envoyé par l'agglomerateur
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @param msg(t_message*) tampon où copier le message
*/
void recevoir_message_d_agglomerateur(t_simulateur_capteur* simul_capteurs, t_message *msg) {
	memcpy(msg, &simul_capteurs->tampon_msg, sizeof(t_message));
}

/*
* envoyer_vers_agglomerateur
* @brief permet d'envoyer un message vers l'agglomérateur
* @param msg(t_message*) message à envoyer
*/
void envoyer_vers_agglomerateur(t_simulateur_capteur* simul_capteurs, t_message *msg) {
	memcpy(&simul_capteurs->tampon_msg, msg, sizeof(t_message));
}

/*
* afficher_message_vers_capteur
* @brief affiche l'entête du message envoyer au capteur
* @param msg(t_message*) message à envoyer
*/
static void afficher_message_vers_capteur(t_message *msg) {

	// change la couleur
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, COULEUR_AGGLO);

	// affiche les informations, dans la colonne de droite (à l'écran)
	printf("\t\t\t\tAgglomerateur\n");
	printf("\t\t\t\treponse au capteur: %s\n", msg->entete.id_capteur);
	printf("\t\t\t\tContient la commmande: ");

	switch (msg->entete.commande) {
		case MSG_DEMANDE_STATUS:
			printf("MSG_DEMANDE_STATUS\n");
			break;
		case MSG_DEMANDE_MESURES:
			printf("MSG_DEMANDE_MESURES\n");
			break;
		case MSG_TERMINE_COMM:
			printf("MSG_TERMINE_COMM\n");
			break;
		default:
			printf("commande invalide\n");

	}
	SetConsoleTextAttribute(hConsole, COULEUR_DEFAUT);
}




