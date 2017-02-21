#ifndef INTERFACE_H
#define INTERFACE_H
/*
Nom du fichier: interface.h
Auteur: Fred Simard, ETS, Hiver 2017
Description: Interface definissant le liens entre les capteurs et l'agglomérateur.
*/

#include "messages.h"
#include "simulateur_capteurs.h"

/*
* recevoir_message_d_agglomerateur
* @brief permet de recevoir un message envoyé par l'agglomerateur
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @param msg(t_message*) tampon où copier le message
*/
void recevoir_message_d_agglomerateur(t_simulateur_capteur* simul_capteurs, t_message *msg);

/*
* envoyer_vers_agglomerateur
* @brief permet d'envoyer un message vers l'agglomérateur
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @param msg(t_message*) message à envoyer
*/
void envoyer_vers_agglomerateur(t_simulateur_capteur* simul_capteurs, t_message *msg);

/*
* detection_evenement
* @brief permet de savoir s'il y a un événement asynchrone
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @retourne 1 s'il y a un événement, et 0 sinon
*/
unsigned char detection_evenement(t_simulateur_capteur* simul_capteurs);

/*
* envoyer_vers_capteur
* @brief permet d'envoyer un message vers un capteur
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @param msg(t_message*) message à envoyer
*/
void envoyer_vers_capteur(t_simulateur_capteur* simul_capteurs, t_message *msg);

/*
* recevoir_message_de_capteur
* @brief permet de recevoir un message envoyé par un capteur
* @param (t_simulateur_capteur*) simul_capteurs, le simulateur de capteurs
* @param msg(t_message*) tampon où copier le message
*/
void recevoir_message_de_capteur(t_simulateur_capteur* simul_capteurs, t_message *msg);


#endif