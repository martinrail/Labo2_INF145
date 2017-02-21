#ifndef MESSAGES_H
#define MESSAGES_H
/*
Nom du fichier: messages.h
Auteur: Fred Simard, ETS, Hiver 2017
Description: Définition des constantes et enregistrements qui définissent les messages échangé 
			 entre les capteurs et l'agglomérateur.

			 A toute fin pratique, ce fichier définit le protocol de communication.

			 Voici un exemple de la séquence d'échange qui prend place entre capteur et agglomerateur

				S	Capteur				Dir		Agglomerateur
				1a	MSG_DEBUT_COMM		--->
				1b						<---	MSG_DEMANDE_STATUS
				2a	MSG_ENVOI_STATUS	--->
				2b						<---	MSG_DEMANDE_MESURES
				3a	MSG_ENVOI_MESURES	--->
				3b						<---	MSG_TERMINE_COMM

			Chaque t_message utilise l'entête: t_entete_message
			
			Puis le contenu du message est interprété en fonction de la commande
			MSG_ENVOI_STATUS a le contenu: t_contenu_status
			MSG_ENVOI_MESURES a le contenu: t_contenu_mesures
*/

#define NB_MESURES_TX_MAX 50
#define LONGUEUR_ID_MAX 100

// commandes définis par le protocol
typedef enum e_commande {
	MSG_DEBUT_COMM,		// envoyé par capteur
	MSG_DEMANDE_STATUS,	// envoyé par agglomerateur
	MSG_ENVOI_STATUS,	// envoyé par capteur
	MSG_DEMANDE_MESURES,// envoyé par agglomerateur
	MSG_ENVOI_MESURES,	// envoyé par capteur
	MSG_TERMINE_COMM	// envoyé par agglomerateur
}t_commande;

// entête des messages
typedef struct s_entete_message {
	char id_capteur[LONGUEUR_ID_MAX];
	t_commande commande;
}t_entete_message;

// contenu des messages de status
typedef struct s_contenu_status {
	int nb_mesures_totales;
}t_contenu_status;

// contenu des messages de mesures
typedef struct s_contenu_mesures {
	int nb_mesures_envoi;
	double mesures[NB_MESURES_TX_MAX];
}t_contenu_mesures;

// definition de la longeur maximal du contenu d'un message
#define SIZE_DOUBLE 8
#define SIZE_INT 8
#define LONGUEUR_CONTENU_MAX NB_MESURES_TX_MAX*SIZE_DOUBLE + SIZE_INT

// définition d'un message générique
typedef struct s_message {
	t_entete_message entete;
	unsigned char contenu[LONGUEUR_CONTENU_MAX];
}t_message;



#endif