#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "agglomerateur.h"
#include "interface.h"
#include "utils.h"
#include "messages.h"


void generer_reponse(t_message *msg_recu, t_message *msg_sortant);

void lancer_agglomerateur(t_simulateur_capteur* simul_capteurs)
{
	t_message msg_recu;
	t_message msg_sortant;


	while (1)//boucle infie qui permet d'effectuer toute les action n�c�ssaires tant et aussi longtemps que l'application est en marche
	{
		

		if (detection_evenement(simul_capteurs) == 1)// on verifie si un �v�nement s'est produit ou non 
		{

			printf("evenement \n");
			recevoir_message_de_capteur(simul_capteurs, &msg_recu);
			generer_reponse(&msg_recu, &msg_sortant);



			envoyer_vers_capteur(simul_capteurs, &msg_sortant);
			delai(100);

		}

		else
		{

			printf("non \n");

		}
			







	}
}

void generer_reponse(t_message *msg_recu, t_message * msg_sortant)
{
	int i;

	for (i = 0; i < LONGUEUR_CONTENU_MAX; i++)
	{
		msg_sortant->entete.id_capteur[i] = msg_recu->entete.id_capteur[i];
	}

	if (msg_recu->entete.commande == MSG_DEBUT_COMM)
	{
		msg_sortant->entete.commande = MSG_DEMANDE_STATUS;
	}

	if (msg_recu->entete.commande == MSG_ENVOI_STATUS)
	{
		msg_sortant->entete.commande = MSG_DEMANDE_MESURES;
	}
	if (msg_recu->entete.commande == MSG_ENVOI_MESURES)
	{
		msg_sortant->entete.commande = MSG_TERMINE_COMM;
	}
}




