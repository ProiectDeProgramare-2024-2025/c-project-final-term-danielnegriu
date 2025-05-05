#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
#else
    #define CLEAR_COMMAND "clear"
#endif

#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define BOLDWHITE   "\033[1m"

void consoleClear() {
    system(CLEAR_COMMAND);
}

typedef struct {
    char marca[50];
    char tip[50];
    char nrInmatriculare[20];
} Autoturism;

int validareCamp(const char *camp) {
    return strlen(camp) > 0;
}

int validareNumarInmatriculare(const char *nr) {
    return strlen(nr) >= 5 && strlen(nr) <= 19; // exemplu simplu
}

int existaNumarInmatriculare(const char *nr) {
    FILE *file = fopen("autoturisme.txt", "r");
    if (!file) return 0;

    Autoturism autoturism;
    while (fscanf(file, "%49[^,],%49[^,],%19s\n", autoturism.marca, autoturism.tip, autoturism.nrInmatriculare) == 3) {
        if (strcmp(autoturism.nrInmatriculare, nr) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void adaugaAutoturism() {
    consoleClear();
    printf(GREEN "---- Adauga autoturism ----\n" RESET);

    Autoturism autoturism;
    printf("Marca autoturismului (sir de caractere): ");
    scanf("%49s", autoturism.marca);
    if (!validareCamp(autoturism.marca)) {
        printf(RED "Marca invalida!\n" RESET);
        return;
    }

    printf("Tipul autoturismului (sir de caractere): ");
    scanf("%49s", autoturism.tip);
    if (!validareCamp(autoturism.tip)) {
        printf(RED "Tip invalid!\n" RESET);
        return;
    }

    printf("Numarul de inmatriculare (sir de caractere, 5-19 caractere): ");
    scanf("%19s", autoturism.nrInmatriculare);
    if (!validareNumarInmatriculare(autoturism.nrInmatriculare)) {
        printf(RED "Numar de inmatriculare invalid!\n" RESET);
        return;
    }

    if (existaNumarInmatriculare(autoturism.nrInmatriculare)) {
        printf(RED "Exista deja un autoturism cu acest numar de inmatriculare!\n" RESET);
        return;
    }

    FILE *file = fopen("autoturisme.txt", "a");
    if (file == NULL) {
        printf(RED "Eroare la deschiderea fisierului!\n" RESET);
        return;
    }
    fprintf(file, "%s,%s,%s\n", autoturism.marca, autoturism.tip, autoturism.nrInmatriculare);
    fclose(file);

    printf(GREEN "Autoturismul a fost adaugat cu succes!\n" RESET);
}

void actualizeazaAutoturism() {
    consoleClear();
    printf(GREEN "---- Actualizeaza autoturism ----\n" RESET);

    char nrCautat[20];
    printf("Introduceti numarul de inmatriculare al autoturismului de actualizat (sir de caractere): ");
    scanf("%19s", nrCautat);

    FILE *file = fopen("autoturisme.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!file || !temp) {
        printf(RED "Eroare la deschiderea fisierelor!\n" RESET);
        return;
    }

    Autoturism autoturism;
    int gasit = 0;

    while (fscanf(file, "%49[^,],%49[^,],%19s\n", autoturism.marca, autoturism.tip, autoturism.nrInmatriculare) == 3) {
        if (strcmp(autoturism.nrInmatriculare, nrCautat) == 0) {
            printf("Noua marca (sir de caractere): ");
            scanf("%49s", autoturism.marca);
            if (!validareCamp(autoturism.marca)) {
                printf(RED "Marca invalida!\n" RESET);
                fclose(file);
                fclose(temp);
                remove("temp.txt");
                return;
            }

            printf("Noul tip (sir de caractere): ");
            scanf("%49s", autoturism.tip);
            if (!validareCamp(autoturism.tip)) {
                printf(RED "Tip invalid!\n" RESET);
                fclose(file);
                fclose(temp);
                remove("temp.txt");
                return;
            }
            gasit = 1;
        }
        fprintf(temp, "%s,%s,%s\n", autoturism.marca, autoturism.tip, autoturism.nrInmatriculare);
    }

    fclose(file);
    fclose(temp);

    remove("autoturisme.txt");
    rename("temp.txt", "autoturisme.txt");

    if (gasit)
        printf(GREEN "Autoturism actualizat cu succes!\n" RESET);
    else
        printf(RED "Autoturismul nu a fost gasit.\n" RESET);
}

void stergeAutoturism() {
    consoleClear();
    printf(GREEN "---- Sterge autoturism ----\n" RESET);

    char nrCautat[20];
    printf("Introduceti numarul de inmatriculare al autoturismului de sters: ");
    scanf("%19s", nrCautat);

    FILE *file = fopen("autoturisme.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!file || !temp) {
        printf(RED "Eroare la deschiderea fisierelor!\n" RESET);
        return;
    }

    Autoturism autoturism;
    int sters = 0;

    while (fscanf(file, "%49[^,],%49[^,],%19s\n", autoturism.marca, autoturism.tip, autoturism.nrInmatriculare) == 3) {
        if (strcmp(autoturism.nrInmatriculare, nrCautat) != 0) {
            fprintf(temp, "%s,%s,%s\n", autoturism.marca, autoturism.tip, autoturism.nrInmatriculare);
        } else {
            sters = 1;
        }
    }

    fclose(file);
    fclose(temp);

    remove("autoturisme.txt");
    rename("temp.txt", "autoturisme.txt");

    if (sters)
        printf(GREEN "Autoturism sters cu succes!\n" RESET);
    else
        printf(RED "Autoturismul nu a fost gasit.\n" RESET);
}

void cautaAutoturism() {
    consoleClear();
    printf(GREEN "---- Cauta autoturism ----\n" RESET);

    char nrCautat[20];
    printf("Introduceti numarul de inmatriculare: ");
    scanf("%19s", nrCautat);

    FILE *file = fopen("autoturisme.txt", "r");
    if (!file) {
        printf(RED "Eroare la deschiderea fisierului!\n" RESET);
        return;
    }

    Autoturism autoturism;
    int gasit = 0;
    while (fscanf(file, "%49[^,],%49[^,],%19s\n", autoturism.marca, autoturism.tip, autoturism.nrInmatriculare) == 3) {
        if (strcmp(autoturism.nrInmatriculare, nrCautat) == 0) {
            printf("Gasit: Marca: " BLUE "%s" RESET ", Tip: " GREEN "%s" RESET ", Nr. Inmatriculare: " YELLOW "%s\n" RESET,
                   autoturism.marca, autoturism.tip, autoturism.nrInmatriculare);
            gasit = 1;
            break;
        }
    }

    fclose(file);

    if (!gasit)
        printf(RED "Autoturismul nu a fost gasit.\n" RESET);
}

void vizualizeazaToateAutoturismele() {
    consoleClear();
    printf(GREEN "---- Lista autoturisme ----\n" RESET);

    FILE *file = fopen("autoturisme.txt", "r");
    if (!file) {
        printf(RED "Eroare la deschiderea fisierului!\n" RESET);
        return;
    }

    Autoturism autoturism;
    while (fscanf(file, "%49[^,],%49[^,],%19s\n",
                  autoturism.marca, autoturism.tip, autoturism.nrInmatriculare) == 3) {
        printf("Marca: " BLUE "%s" RESET ", Tip: " GREEN "%s" RESET ", Nr. Inmatriculare: " YELLOW "%s\n" RESET,
               autoturism.marca, autoturism.tip, autoturism.nrInmatriculare);
    }

    fclose(file);
}

void menu();

void optiuneInvalida() {
    consoleClear();
    printf(RED "Optiune invalida.\n\n" RESET);
    printf("0. Revino la meniu\n");

    int optiune;
    printf("Selecteaza o optiune: ");
    scanf("%d", &optiune);

    if (optiune == 0) {
        menu();
    } else {
        optiuneInvalida();
    }
}

void subMenu(int optiune) {
    switch (optiune) {
        case 1: adaugaAutoturism(); break;
        case 2: actualizeazaAutoturism(); break;
        case 3: stergeAutoturism(); break;
        case 4: cautaAutoturism(); break;
        case 5: vizualizeazaToateAutoturismele(); break;
        default: optiuneInvalida(); return;
    }

    printf("\n0. Revino la meniu\n\n");
    int optiune2;
    printf("Selecteaza o optiune: ");
    scanf("%d", &optiune2);

    if (optiune2 == 0) {
        menu();
    } else {
        optiuneInvalida();
    }
}

void menu() {
    consoleClear();
    printf(BOLDWHITE "--- MENIU ---\n" RESET);
    printf("1. Adauga autoturism\n");
    printf("2. Actualizeaza autoturism\n");
    printf("3. Sterge autoturism\n");
    printf("4. Cauta autoturism\n");
    printf("5. Vizualizeaza toate autoturismele\n");
    printf("0. Iesire\n\n");

    int optiune;
    printf("Selecteaza o optiune: ");
    scanf("%d", &optiune);

    if (optiune == 0) {
        consoleClear();
        printf(YELLOW "Iesire din program...\n" RESET);
        exit(0);
    }

    subMenu(optiune);
}

int main() {
    menu();
    return 0;
}
