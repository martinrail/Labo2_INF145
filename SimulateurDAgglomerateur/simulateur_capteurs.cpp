/*
Nom du fichier: simulateur_capteurs.cpp
Auteur: Fred Simard, ETS, Hiver 2017
Description: Fait la simulation d'un groupe de capteurs qui se connecte � un agglom�rateur de fa�on asynchrone.
			 Les simulateurs ont une fr�quence d'�chantillon fixe et des donn�es bidon sont g�n�r�s pour combler l'intervale
			 entre deux connections avec l'agglom�rateur.
 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <Windows.h>

#include "utils.h"
#include "interface.h"
#include "simulateur_capteurs.h"

#define COULEUR_DEFAUT 0x07

const unsigned char COULEUR_CAPTEUR[8] = {0x09,0x08,0x06,0x05,0x04,0x03,0x02,0x01};

// fonctions de gestions des �tats
static void gestion_etat_en_operation(t_simulateur_capteur* simul_capteurs, t_capteur* capteur);
static void gestion_etat_transmission_status(t_simulateur_capteur* simul_capteurs, t_capteur* capteur);
static void gestion_etat_transmission_mesures(t_simulateur_capteur* simul_capteurs, t_capteur* capteur);

// utilitaires capteur
static void efface_mesures(t_capteur* capteur, int nb_mesures);
static void genere_id_alea(char* id_capteur);
static void active_capteur_alea(t_simulateur_capteur* simul_capteurs);

/*
 * initialiser_simulateur_capteurs
 * @brief initialise le tableau de capteurs et tous les capteurs avec des valeurs par d�faut
 * @param nb_capteurs(int), nombre de capteurs � simuler
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
*/
void initialiser_simulateur_capteurs(t_simulateur_capteur* simul_capteurs, t_mode_operation mode_operation, int nb_capteurs) {

	int i = 0;
	simul_capteurs->mode_operation = mode_operation;

	// d�finit le nombre de capteur en fonction du mode
	// d'operation
	switch (simul_capteurs->mode_operation) {
		case UN_SEUL_CAPTEUR:
			simul_capteurs->nb_capteurs = 1;
			break;
		case MESURES_EN_SEQUENCE:
		case MESURES_EN_SEQUENCE_PAR_CAPTEUR:
		case NORMAL:
			simul_capteurs->nb_capteurs = nb_capteurs;
			break;
		default:
			printf("Mode operation invalide\n");
			exit(-1);
	}

	//alloue la m�moire pour les capteurs
	simul_capteurs->tab_capteurs = (t_capteur*)malloc(sizeof(t_capteur)*simul_capteurs->nb_capteurs);
	
	// donne des valeurs par d�faut aux capteurs
	for (i = 0; i < simul_capteurs->nb_capteurs; i++) {

		// etat par d�faut
		simul_capteurs->tab_capteurs[i].etat_courant = EN_OPERATION;

		// g�n�re un nom al�atoire
		genere_id_alea(simul_capteurs->tab_capteurs[i].id_capteur); 

		// prend note du temps actuel
		time(&(simul_capteurs->tab_capteurs[i].temps_derniere_transmission));

		// tableau de mesures vide
		simul_capteurs->tab_capteurs[i].temp = 0;
		simul_capteurs->tab_capteurs[i].nb_mesures = 0;
		memset(simul_capteurs->tab_capteurs[i].mesures, 0, sizeof(double)*NB_MESURES_MAX);

		// donne une couleur al�atoire au capteur
		simul_capteurs->tab_capteurs[i].couleur = COULEUR_CAPTEUR[i];
	}
}


/*
* liberer_simulateur_capteurs
* @brief libere la memoire du simulateur de capteurs
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
*/
void liberer_simulateur_capteurs(t_simulateur_capteur* simul_capteurs){
	free(simul_capteurs->tab_capteurs);
}


/*
* evenement_capteurs
* @brief permet de savoir s'il y a un �v�nement asynchrone qui a pris place
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @return 1 s'il y a un �v�nement, 0 sinon
*/
unsigned char evenement_capteurs(t_simulateur_capteur* simul_capteurs) {

	unsigned char evenement = 0x00;

	// determine s'il y a un �v�nement?
	if (reel_alea() <= PROB_EVENEMENT) {

		// oui, actualise la valeur de retour
		evenement = 0x01;

		// active un des capteurs
		active_capteur_alea(simul_capteurs);
	}

	return evenement;
}

/*
* simulateur_traitement_message
* @brief appel demandand au simulateur de traiter le message entr� (�quivalent d'un interrupt software)
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
*/
void simulateur_traitement_message(t_simulateur_capteur* simul_capteurs) {

	int i = 0;
	int trouve = 0;
	t_message message_entrant;

	// capture le message
	recevoir_message_d_agglomerateur(simul_capteurs, &message_entrant);

	// trouve le capteur � qui le message s'adresse
	while (!trouve && i < simul_capteurs->nb_capteurs) {

		if (!strcmp(message_entrant.entete.id_capteur, simul_capteurs->tab_capteurs[i].id_capteur)) {
			trouve = 0x01;
		}
		else {
			i++;
		}
	}

	// si on ne trouve pas, on ne retourne rien
	if (!trouve) {
		perror("[SIMULATEUR_CAPTEUR] Le capteur id est invalide\n");
		return;
	}

	// change la couleur de l'affichage
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, simul_capteurs->tab_capteurs[i].couleur);
	
	// si on le capteur est en operation, alors ce n'est pas normal
	if (simul_capteurs->tab_capteurs[i].etat_courant != ATTENTE_CENTRALE) {
		printf("Le capteur n'est pas en attente centrale\n");
		return;
	}

	// change l'�tat du capteur en fonction de la commande re�us
	switch (message_entrant.entete.commande) {
		case MSG_DEMANDE_STATUS:
			printf("Capteur: Reception demande status\n\n");
			simul_capteurs->tab_capteurs[i].etat_courant = TRANSMISSION_STATUS;
			break;
		case MSG_DEMANDE_MESURES:
			printf("Capteur: Reception demande mesures\n\n");
			simul_capteurs->tab_capteurs[i].etat_courant = TRANSMISSION_MESURES;
			break;
		case MSG_TERMINE_COMM:
			printf("Capteur: Termine communication\n\n");
			simul_capteurs->tab_capteurs[i].etat_courant = EN_OPERATION;
			break;
		default:
			printf("Etat invalide %i\n", message_entrant.entete.commande);
	}

	SetConsoleTextAttribute(hConsole, COULEUR_DEFAUT);
	printf("-------------------------------------------------------------------------------\n");
}

/*
* active_capteur_alea
* @brief choisit un capteur al�atoirement et lui fait faire une op�ration en fonction de son �tat
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
*/
static void active_capteur_alea(t_simulateur_capteur* simul_capteurs) {

	// choisie un des capteurs au hasard et effectue une ronde d'operation
	int idx_capteur = rand() % simul_capteurs->nb_capteurs;

	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, simul_capteurs->tab_capteurs[idx_capteur].couleur);
	
	// � partir de l'�tat courant du capteur, 
	// appel la m�thode de gestion �ch�ante
	switch (simul_capteurs->tab_capteurs[idx_capteur].etat_courant) {

		case EN_OPERATION:
			gestion_etat_en_operation(simul_capteurs, &simul_capteurs->tab_capteurs[idx_capteur]);
			break;
		case TRANSMISSION_STATUS:
			gestion_etat_transmission_status(simul_capteurs, &simul_capteurs->tab_capteurs[idx_capteur]);
			break;
		case TRANSMISSION_MESURES:
			gestion_etat_transmission_mesures(simul_capteurs, &simul_capteurs->tab_capteurs[idx_capteur]);
			break;
		default:
			// c'est une erreur de se retrouver ici
			perror("[SIMULATEUR_CAPTEURS] etat capteur invalide\n");
			break;
	}

	SetConsoleTextAttribute(hConsole, COULEUR_DEFAUT);
}

/*
* gestion_etat_en_operation
* @brief fait la gestion d'un capteur en op�ration, informe l'agglom�rateur qu'il est pr�t � commencer la communication
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @param capteur(t_capteur), r�f�rence au capteur pour laquelle on veut faire la gestion
*/
static void gestion_etat_en_operation(t_simulateur_capteur* simul_capteurs, t_capteur* capteur) {

	// informe l'utilisateur
	printf("\nCapteur: %s, debut comm\n", capteur->id_capteur);

	// prepare le message sortant
	t_message message_sortant;

	// �criture du message
	strcpy(message_sortant.entete.id_capteur, capteur->id_capteur);
	message_sortant.entete.commande = MSG_DEBUT_COMM;

	// envoi � l'agglomerateur
	envoyer_vers_agglomerateur(simul_capteurs, &message_sortant);

	// vers prochain �tat
	capteur->etat_courant = ATTENTE_CENTRALE;
}

/*
* gestion_etat_transmission_status
* @brief fait la gestion d'un capteur en transmission de status, envoit son status � l'agglom�rateur
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @param capteur(t_capteur), r�f�rence au capteur pour laquelle on veut faire la gestion
*/
static void gestion_etat_transmission_status(t_simulateur_capteur* simul_capteurs, t_capteur* capteur) {

	// informe l'utilisateur
	printf("\nCapteur: %s, transmission status\n", capteur->id_capteur);
	
	int i = 0;
	t_message message_sortant;

	// calcul le temps �coul� depuis la derniere transmission
	time_t temps_courant = time(NULL);
	double difference_temps = difftime(temps_courant,capteur->temps_derniere_transmission);
	capteur->temps_derniere_transmission = temps_courant;

	// d�termine le nombre de mesures
	capteur->nb_mesures = (int)floor(difference_temps * FREQ_ECHANTILLONNAGE);

	// detecte s'il y a un trop grand nombre d'�chantillons
	if (capteur->nb_mesures > NB_MESURES_MAX) {
		capteur->nb_mesures = NB_MESURES_MAX;
		perror("[SIMULATEUR_CAPTEURS] Des echantillons ont ete perdu\n");
	}

	// remplie le tableau de mesures
	for (i = 0; i < capteur->nb_mesures; i++) {

		switch (simul_capteurs->mode_operation) {
			case UN_SEUL_CAPTEUR:
			case MESURES_EN_SEQUENCE:
				capteur->mesures[i] = simul_capteurs->temp++;
				break;
			case MESURES_EN_SEQUENCE_PAR_CAPTEUR:
				capteur->mesures[i] = capteur->temp++;
				break;
			case NORMAL:
				capteur->mesures[i] = reel_alea() * 5 + 15;
				break;

		}
	}

	// �criture et envoi du message
	strcpy(message_sortant.entete.id_capteur, capteur->id_capteur);
	message_sortant.entete.commande = MSG_ENVOI_STATUS;
	((t_contenu_status*)message_sortant.contenu)->nb_mesures_totales = capteur->nb_mesures;
	envoyer_vers_agglomerateur(simul_capteurs, &message_sortant);

	// se met en attente
	capteur->etat_courant = ATTENTE_CENTRALE;
}

/*
* gestion_etat_transmission_mesures
* @brief fait la gestion d'un capteur en transmission de mesures, envoit ses mesures � l'agglom�rateur
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @param capteur(t_capteur), r�f�rence au capteur pour laquelle on veut faire la gestion
*/
static void gestion_etat_transmission_mesures(t_simulateur_capteur* simul_capteurs, t_capteur* capteur) {

	// informe l'utilisateur
	printf("\nCapteur: %s, transmission mesures\n", capteur->id_capteur);

	int i = 0;
	t_message message_sortant;

	// �criture de l'ent�te du message
	strcpy(message_sortant.entete.id_capteur, capteur->id_capteur);
	message_sortant.entete.commande = MSG_ENVOI_MESURES;

	// copie les mesures dans le message
	if (capteur->nb_mesures >= NB_MESURES_TX_MAX) {

		// cas o� on n'a plus de mesures que le maximum 
		((t_contenu_mesures*)message_sortant.contenu)->nb_mesures_envoi = NB_MESURES_TX_MAX;
		memcpy(((t_contenu_mesures*)message_sortant.contenu)->mesures, capteur->mesures, NB_MESURES_TX_MAX*sizeof(double));

		// efface les mesures transfer�
		efface_mesures(capteur, NB_MESURES_TX_MAX);
	}
	else {
		// cas o� on n'a plus de mesures que le maximum 
		((t_contenu_mesures*)message_sortant.contenu)->nb_mesures_envoi = capteur->nb_mesures;
		memcpy(((t_contenu_mesures*)message_sortant.contenu)->mesures, capteur->mesures, capteur->nb_mesures*sizeof(double));

		// pas de besoin de d�placer les mesures, il n'y en a plus...
		capteur->nb_mesures = 0;
	}
	
	// envoi le message � l'agglom�rateur
	envoyer_vers_agglomerateur(simul_capteurs, &message_sortant);

	// informe l'utilisateur sur l'op�ration
	printf("Transmission Mesures:\n");
	printf("Nb mesures envoye: %i\n", ((t_contenu_mesures*)message_sortant.contenu)->nb_mesures_envoi);
	printf("Nb mesures restante: %i\n", capteur->nb_mesures);

	// se met en attente
	capteur->etat_courant = ATTENTE_CENTRALE;
}

/*
* efface_mesures
* @brief fait la gestion d'un capteur en transmission de mesures, envoit ses mesures � l'agglom�rateur
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @param capteur(t_capteur), r�f�rence au capteur pour lequel on veut effacer des mesures
* @param nb_mesures(int), nombre de mesures � effacer
*/
static void efface_mesures(t_capteur* capteur, int nb_mesures) {

	// efface les mesures transmises
	for (int i = 0; i < (capteur->nb_mesures - nb_mesures); i++) {
		capteur->mesures[i] = capteur->mesures[i + nb_mesures];
	}
	capteur->nb_mesures -= nb_mesures;

}


/*
* genere_id_alea
* @brief g�n�re un id al�atoire pour les capteurs du style "id455", ou l'entier est aleatoire
* @param id_capteur(char*), nom du capteur g�n�r�
*/
static void genere_id_alea(char* id_capteur) {

	char buf[100];
	_itoa(rand() % 1000, buf, 10);
	strcpy(id_capteur, "id");
	strcat(id_capteur, buf);

}
