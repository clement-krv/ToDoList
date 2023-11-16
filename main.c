#include "headers/todolist.h"

int main()
{
    ListeTaches *liste = creerListeTaches();

    while (1)
    {
        printf("\n===== Menu =====\n");
        printf("1. Ajouter une tache\n");
        printf("2. Afficher les taches\n");
        printf("3. Retirer les taches terminees\n");
        printf("4. Quitter\n");
        printf("Choisissez une option: ");

        int choix;
        scanf("%d", &choix);

        switch (choix)
        {
        case 1:
        {
            char nom[100];
            int joursPourTerminer;
            printf("\n===== Ajouter une tache =====\n");
            printf("Entrez le nom de la tache: ");
            scanf("%s", nom);
            printf("Entrez le nombre de jours pour terminer la tache: ");
            scanf("%d", &joursPourTerminer);
            Tache *tache = creerTache(nom, joursPourTerminer);
            ajouterTache(liste, tache);
            mettreAJourTaches(liste);
            break;
        }
        case 2:
            printf("\n===== Afficher les taches =====\n");
            afficherTaches(liste);
            break;
        case 3:
            printf("\n===== Taches terminees retiree =====\n");
            retirerTachesTerminees(liste);
            break;
        case 4:
            libererListeTaches(liste);
            return 0;
        default:
            printf("Option non valide. Veuillez choisir une option entre 1 et 6.\n");
        }
    }

    return 0;
}