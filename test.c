#include "headers/todolist.h"

int main()
{
    ListeTaches *liste = creerListeTaches();

    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Ajouter une tache\n");
        printf("2. Afficher les taches\n");
        printf("3. Mettre a jour les taches\n");
        printf("4. Commencer les taches en attente\n");
        printf("5. Retirer les taches terminees\n");
        printf("6. Quitter\n");
        printf("Choisissez une option: ");

        int choix;
        scanf("%d", &choix);

        switch (choix)
        {
        case 1:
        {
            char nom[100];
            int joursPourTerminer;
            printf("Entrez le nom de la tache: ");
            scanf("%s", nom);
            printf("Entrez le nombre de jours pour terminer la tache: ");
            scanf("%d", &joursPourTerminer);
            Tache *tache = creerTache(nom, joursPourTerminer);
            ajouterTache(liste, tache);
            commencerTachesEnAttente(liste); // Ajoutez cette ligne
            break;
        }
        case 2:
            afficherTaches(liste);
            break;
        case 3:
            mettreAJourTaches(liste);
            break;
        case 4:
            commencerTachesEnAttente(liste);
            break;
        case 5:
            retirerTachesTerminees(liste);
            break;
        case 6:
            return 0;
        default:
            printf("Option non valide. Veuillez choisir une option entre 1 et 6.\n");
        }
    }

    return 0;
}