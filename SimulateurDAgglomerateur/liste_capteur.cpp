#include <stdio.h>
#include "stdlib.h"
#include <string.h>
#include "messages.h"

#define CONSTANTE_DONNER char



struct capteur
{
		int actif;
		char id;
		int nb_mesures_transactions;
		int nb_mesures_enregistrer;
		CONSTANTE_DONNER mesures_enregistrees[1000];
};

struct liste_capteur
{
	int nb_capteur;
	capteur * liste;
};

void init_liste_capteur(liste_capteur * pointeur_liste, int taille) 
{
	pointeur_liste->nb_capteur = taille;
	pointeur_liste->liste = (capteur *)malloc(taille * sizeof(capteur));
	memset(pointeur_liste, 0, taille * sizeof(capteur));
}

capteur trouver_ajouter_capteur(liste_capteur * pointeur_liste, char ID)
{
	int i = 0;
	while (1)
	{
		if (i == pointeur_liste->nb_capteur)
		{
			return;
		}

		if (pointeur_liste->liste[i].actif == 1)
		{
			if (pointeur_liste->liste[i].id == ID)
			{
				return pointeur_liste->liste[i];
			}
		}
		else
		{
			pointeur_liste->liste[i].actif = 1;
			strcpy(&pointeur_liste->liste[i].id, &ID);
			return pointeur_liste->liste[i];
		}
		i++;
	}
} 

void ajouter_mesure(capteur * pointeur_capteur, t_message message)
{
}

