#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char nom[50];
    int age;
    float poids;
    char type_abonnement[20];
    int est_actif;
} Membre;

void ajouterMembre();
void afficherMembres();
void rechercherMembre();
void modifierMembre();
void supprimerMembre();
void viderBuffer();
void menu();

const char* FILE_NAME = "gym_data.dat";

int main() {
    printf("====================================\n");
    printf("    BIENVENUE DANS EASY GYM v1.1    \n");
    printf("====================================\n");
    menu();
    return 0;
}

void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menu() {
    int choix;
    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Ajouter un membre\n");
        printf("2. Afficher tous les membres\n");
        printf("3. Rechercher par ID\n");
        printf("4. Modifier un membre\n");
        printf("5. Supprimer un membre\n");
        printf("0. Quitter\n");
        printf("Choix : ");
        
        if (scanf("%d", &choix) != 1) {
            printf("Erreur: Entrez un nombre !\n");
            viderBuffer();
            choix = -1;
            continue;
        }
        viderBuffer();

        switch (choix) {
            case 1: ajouterMembre(); break;
            case 2: afficherMembres(); break;
            case 3: rechercherMembre(); break;
            case 4: modifierMembre(); break;
            case 5: supprimerMembre(); break;
            case 0: printf("Bye bro! Stay fit!\n"); break;
            default: printf("Choix invalide !\n");
        }
    } while (choix != 0);
}

void ajouterMembre() {
    FILE *file = fopen(FILE_NAME, "ab");
    if (!file) return;

    Membre m;
    printf("ID : "); scanf("%d", &m.id);
    printf("Nom : "); scanf("%49s", m.nom);
    printf("Age : "); scanf("%d", &m.age);
    printf("Poids (kg) : "); scanf("%f", &m.poids);
    printf("Abonnement (Silver/Gold) : "); scanf("%19s", m.type_abonnement);
    m.est_actif = 1;

    fwrite(&m, sizeof(Membre), 1, file);
    fclose(file);
    printf("\n>>> Membre ajoute avec succes !\n");
}

void afficherMembres() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) {
        printf("\nAucune donnee disponible.\n");
        return;
    }

    Membre m;
    printf("\n%-5s %-20s %-5s %-7s %-15s %-10s\n", "ID", "NOM", "AGE", "POIDS", "ABONNEMENT", "STATUT");
    printf("----------------------------------------------------------------------\n");
    while (fread(&m, sizeof(Membre), 1, file)) {
        printf("%-5d %-20s %-5d %-7.1f %-15s %-10s\n",
               m.id, m.nom, m.age, m.poids, m.type_abonnement,
               m.est_actif ? "Actif" : "Expire");
    }
    fclose(file);
}

void rechercherMembre() {
    int id;
    printf("ID a rechercher : ");
    scanf("%d", &id);

    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) return;

    Membre m;
    int trouve = 0;
    while (fread(&m, sizeof(Membre), 1, file)) {
        if (m.id == id) {
            printf("\nTrouve : %s, %d ans, Poids: %.1fkg [%s]\n", m.nom, m.age, m.poids, m.type_abonnement);
            trouve = 1;
            break;
        }
    }
    if (!trouve) printf("Membre introuvable.\n");
    fclose(file);
}

void modifierMembre() {
    int id;
    printf("ID du membre a modifier : ");
    scanf("%d", &id);

    FILE *file = fopen(FILE_NAME, "rb+");
    if (!file) return;

    Membre m;
    int trouve = 0;
    while (fread(&m, sizeof(Membre), 1, file)) {
        if (m.id == id) {
            trouve = 1;
            printf("Nouveau nom : "); scanf("%49s", m.nom);
            printf("Nouvel age : "); scanf("%d", &m.age);
            printf("Nouveau poids : "); scanf("%f", &m.poids);
            printf("Nouveau type (Silver/Gold) : "); scanf("%19s", m.type_abonnement);
            printf("Statut (1=Actif, 0=Expire) : "); scanf("%d", &m.est_actif);
            fseek(file, -(long)sizeof(Membre), SEEK_CUR);
            fwrite(&m, sizeof(Membre), 1, file);
            printf("Modifications enregistrees !\n");
            break;
        }
    }
    if (!trouve) printf("ID inconnu.\n");
    fclose(file);
}

void supprimerMembre() {
    int id;
    printf("ID du membre a supprimer : ");
    scanf("%d", &id);

    FILE *file = fopen(FILE_NAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (!file || !temp) {
        printf("Erreur de fichier.\n");
        return;
    }

    Membre m;
    int trouve = 0;
    while (fread(&m, sizeof(Membre), 1, file)) {
        if (m.id != id) {
            fwrite(&m, sizeof(Membre), 1, temp);
        } else {
            trouve = 1;
        }
    }
    fclose(file);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (trouve) printf("Membre supprime.\n");
    else printf("ID non trouve.\n");
}
