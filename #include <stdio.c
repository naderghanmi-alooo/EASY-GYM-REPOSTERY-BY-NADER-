#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int id;
    char nom[50];
    int age;
    float poids;
    char type_abonnement[20];
    int est_actif;
} Membre;

const char* FILE_NAME = "gym_data.dat";
const int ID_MIN = 1;
const int ID_MAX = 999999;
const int AGE_MIN = 13;
const int AGE_MAX = 120;
const float POIDS_MIN = 30.0f;
const float POIDS_MAX = 300.0f;

void viderBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validerID(int id) {
    return (id >= ID_MIN && id <= ID_MAX);
}

int validerAge(int age) {
    return (age >= AGE_MIN && age <= AGE_MAX);
}

int validerPoids(float poids) {
    return (poids >= POIDS_MIN && poids <= POIDS_MAX);
}

int validerAbonnement(const char* abonnement) {
    return (strcmp(abonnement, "Silver") == 0 ||
            strcmp(abonnement, "Gold") == 0 ||
            strcmp(abonnement, "Platinum") == 0);
}

void afficherMembre(const Membre *m) {
    printf("%d  |  %s  |  %d  |  %.1f kg  |  %s\n", 
           m->id, m->nom, m->age, m->poids, m->type_abonnement);
}


void ajouterMembre(void) {
    FILE *f = fopen(FILE_NAME, "ab");
    if (!f) {
        printf("Error: Cannot open file for writing.\n");
        return;
    }
    
    Membre m;
    int valide = 0;
    
    while (!valide) {
        printf("\n[AJOUT] ID (%d-%d): ", ID_MIN, ID_MAX);
        if (scanf("%d", &m.id) != 1) {
            printf("Invalid input. Please enter a number.\n");
            viderBuffer();
            continue;
        }
        viderBuffer();
        if (!validerID(m.id)) {
            printf("Error: ID must be between %d and %d.\n", ID_MIN, ID_MAX);
            continue;
        }
        valide = 1;
    }

    valide = 0;
    while (!valide) {
        printf("Name (max 49 characters): ");
        if (scanf("%49s", m.nom) != 1) {
            printf("Invalid input.\n");
            viderBuffer();
            continue;
        }
        viderBuffer();
        if (strlen(m.nom) == 0) {
            printf("Error: Name cannot be empty.\n");
            continue;
        }
        valide = 1;
    }
    
    valide = 0;
    while (!valide) {
        printf("Age (%d-%d): ", AGE_MIN, AGE_MAX);
        if (scanf("%d", &m.age) != 1) {
            printf("Invalid input. Please enter a number.\n");
            viderBuffer();
            continue;
        }
        viderBuffer();
        if (!validerAge(m.age)) {
            printf("Error: Age must be between %d and %d.\n", AGE_MIN, AGE_MAX);
            continue;
        }
        valide = 1;
    }
    
    valide = 0;
    while (!valide) {
        printf("Weight in kg (%.1f-%.1f): ", POIDS_MIN, POIDS_MAX);
        if (scanf("%f", &m.poids) != 1) {
            printf("Invalid input. Please enter a number.\n");
            viderBuffer();
            continue;
        }
        viderBuffer();
        if (!validerPoids(m.poids)) {
            printf("Error: Weight must be between %.1f and %.1f kg.\n", POIDS_MIN, POIDS_MAX);
            continue;
        }
        valide = 1;
    }
    
    valide = 0;
    while (!valide) {
        printf("Subscription type (Silver/Gold/Platinum): ");
        if (scanf("%19s", m.type_abonnement) != 1) {
            printf("Invalid input.\n");
            viderBuffer();
            continue;
        }
        viderBuffer();
        if (!validerAbonnement(m.type_abonnement)) {
            printf("Error: Subscription must be Silver, Gold, or Platinum.\n");
            continue;
        }
        valide = 1;
    }
    
    m.est_actif = 1;
    
    if (fwrite(&m, sizeof(Membre), 1, f) != 1) {
        printf("Error: Failed to write member to file.\n");
    } else {
        printf("\n✓ Member added successfully!\n");
    }
    
    fclose(f);
}

void afficherMembres(void) {
    FILE *f = fopen(FILE_NAME, "rb");
    if (!f) {
        printf("No data found.\n");
        return;
    }
    
    Membre m;
    int count = 0;
    
    printf("\n========================================\n");
    printf("ID  |  NOM  |  AGE  |  WEIGHT  |  TYPE\n");
    printf("========================================\n");
    
    while (fread(&m, sizeof(Membre), 1, f) == 1) {
        afficherMembre(&m);
        count++;
    }
    
    printf("========================================\n");
    printf("Total members: %d\n", count);
    
    if (ferror(f)) {
        printf("Error: Failed to read file.\n");
    }
    
    fclose(f);
}

void supprimerMembre(void) {
    int id;
    int valide = 0;
    
    while (!valide) {
        printf("\nID to delete (%d-%d): ", ID_MIN, ID_MAX);
        if (scanf("%d", &id) != 1) {
            printf("Invalid input. Please enter a number.\n");
            viderBuffer();
            continue;
        }
        viderBuffer();
        if (!validerID(id)) {
            printf("Error: ID must be between %d and %d.\n", ID_MIN, ID_MAX);
            continue;
        }
        valide = 1;
    }
    
    FILE *f = fopen(FILE_NAME, "rb");
    if (!f) {
        printf("No data found.\n");
        return;
    }
    
    FILE *t = fopen("temp.dat", "wb");
    if (!t) {
        printf("Error: Cannot create temporary file.\n");
        fclose(f);
        return;
    }
    
    Membre m;
    int found = 0;
    
    while (fread(&m, sizeof(Membre), 1, f) == 1) {
        if (m.id != id) {
            if (fwrite(&m, sizeof(Membre), 1, t) != 1) {
                printf("Error: Failed to write to temporary file.\n");
                fclose(f);
                fclose(t);
                return;
            }
        } else {
            found = 1;
        }
    }
    
    fclose(f);
    fclose(t);
    
    if (found) {
        remove(FILE_NAME);
        rename("temp.dat", FILE_NAME);
        printf("✓ Member with ID %d deleted successfully.\n", id);
    } else {
        printf("Error: Member with ID %d not found.\n", id);
        remove("temp.dat");
    }
}

int main(void) {
    int choice;
    
    while (1) {
        printf("\n===== EASY GYM MANAGEMENT =====\n");
        printf("1. Add Member\n");
        printf("2. Show All Members\n");
        printf("3. Delete Member\n");
        printf("0. Exit\n");
        printf("==============================\n");
        printf("Your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            viderBuffer();
            continue;
        }
        viderBuffer();
        
        switch (choice) {
            case 1:
                ajouterMembre();
                break;
            case 2:
                afficherMembres();
                break;
            case 3:
                supprimerMembre();
                break;
            case 0:
                printf("\nGoodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}