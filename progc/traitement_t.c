#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <err.h>

// Structure pour stocker les statistiques sur une ville dans l'AVL
typedef struct {
    char cityName[100];
    int NbTrajets;
    int NbDeparts;
    int *routeIDs;  // Tableau dynamique d'entiers pour stocker les RouteID
    int tailleTabRoute;
} CityStats;


// Structure pour les nœuds de l'AVL
typedef struct AVLNode {
    CityStats data;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;




// Fonction pour calculer la hauteur d'un nœud
int height(AVLNode *node) {
    if (node == NULL) return 0;
    return node->height;
}

// Fonction pour obtenir le maximum de deux nombres
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fonction pour effectuer une rotation à droite
AVLNode *rotateRight(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // Effectuer la rotation
    x->right = y;
    y->left = T2;

    // Mettre à jour les hauteurs
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Retourner le nouveau nœud racine
    return x;
}

// Fonction pour effectuer une rotation à gauche
AVLNode *rotateLeft(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // Effectuer la rotation
    y->left = x;
    x->right = T2;

    // Mettre à jour les hauteurs
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Retourner le nouveau nœud racine
    return y;
}



// Fonction pour obtenir le facteur d'équilibre d'un nœud
int getBalance(AVLNode *node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}

// Fonction pour insérer un élément dans un AVL
AVLNode *insertAVLDeparts(AVLNode *root, const char *cityName) {
    // Si l'arbre est vide, créer un nouveau nœud
    if (root == NULL) {
        AVLNode *newNode = (AVLNode *)malloc(sizeof(AVLNode));
        if (newNode == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(newNode->data.cityName, cityName);
        newNode->data.NbDeparts = 1;
        newNode->height = 1;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    // Sinon, comparer et insérer récursivement
    int compare = strcmp(cityName, root->data.cityName);

    if (compare < 0) {
        root->left = insertAVLDeparts(root->left, cityName);
    } else if (compare > 0) {
        root->right = insertAVLDeparts(root->right, cityName);
    } else {
        // La ville existe déjà, mettez à jour le compteur ou effectuez une autre action
        printf("Ville déjà existante : %s, mise à jour du compteur.\n", cityName);
        (root->data.NbDeparts)++;
        return root;
    }

    // Mettre à jour la hauteur du nœud actuel
    root->height = 1 + max(height(root->left), height(root->right));

    // Équilibrer l'arbre après l'insertion
    int balance = getBalance(root);

    // Cas Gauche-Gauche
    if (balance > 1 && strcmp(cityName, root->left->data.cityName) < 0) {
        return rotateRight(root);
    }

    // Cas Droit-Droit
    if (balance < -1 && strcmp(cityName, root->right->data.cityName) > 0) {
        return rotateLeft(root);
    }

    // Cas Gauche-Droite
    if (balance > 1 && strcmp(cityName, root->left->data.cityName) > 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // Cas Droit-Gauche
    if (balance < -1 && strcmp(cityName, root->right->data.cityName) < 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// Fonction pour insérer un élément dans un AVL
AVLNode *insertAVLTrajets(AVLNode *root, const char *cityName, int count, int routeID) {
    // Si l'arbre est vide, créer un nouveau nœud
    if (root == NULL) {
        AVLNode *newNode = (AVLNode *)malloc(sizeof(AVLNode));
        if (newNode == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(newNode->data.cityName, cityName);
        newNode->data.NbTrajets = count;
        newNode->data.tailleTabRoute = 1;  // Initialiser le compteur à un
        newNode->data.routeIDs = (int *)malloc(sizeof(int));  // Allouer mémoire pour le tableau
        if (newNode->data.routeIDs == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour routeIDs.\n");
            exit(EXIT_FAILURE);
        }
        newNode->data.routeIDs[0] = routeID;  // Ajouter le RouteID au tableau

        newNode->height = 1;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    // Sinon, comparer et insérer récursivement
    int compare = strcmp(cityName, root->data.cityName);

    if (compare < 0) {
        root->left = insertAVLTrajets(root->left, cityName, count, routeID);
    } else if (compare > 0) {
        root->right = insertAVLTrajets(root->right, cityName, count, routeID);
    } else {
        // La ville existe déjà, mettez à jour le compteur ou effectuez une autre action
        printf("Ville déjà existante : %s, mise à jour du compteur.\n", cityName);
        (root->data.NbTrajets)++;
        root->data.tailleTabRoute++;
        root->data.routeIDs = (int *)realloc(root->data.routeIDs, sizeof(int) * root->data.tailleTabRoute);
        if (root->data.routeIDs == NULL) {
            fprintf(stderr, "Erreur d'allocation dynamique de mémoire pour routeIDs.\n");
            exit(EXIT_FAILURE);
        }
        root->data.routeIDs[root->data.tailleTabRoute - 1] = routeID;
        return root;
    }

    // Mettre à jour la hauteur du nœud actuel
    root->height = 1 + max(height(root->left), height(root->right));

    // Équilibrer l'arbre après l'insertion
    int balance = getBalance(root);

    // Cas Gauche-Gauche
    if (balance > 1 && strcmp(cityName, root->left->data.cityName) < 0) {
        return rotateRight(root);
    }

    // Cas Droit-Droit
    if (balance < -1 && strcmp(cityName, root->right->data.cityName) > 0) {
        return rotateLeft(root);
    }

    // Cas Gauche-Droite
    if (balance > 1 && strcmp(cityName, root->left->data.cityName) > 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // Cas Droit-Gauche
    if (balance < -1 && strcmp(cityName, root->right->data.cityName) < 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// Fonction de recherche dans un AVL
AVLNode *searchAVL(AVLNode *root, const char *cityName) {
    // Parcours récursif de l'arbre
    if (root == NULL){
        return NULL;
    }
  
    if (strcmp(cityName, root->data.cityName) == 0) {
        return root;
    }
  
    if (strcmp(cityName, root->data.cityName) < 0) {
        return searchAVL(root->left, cityName);
    } else {
        return searchAVL(root->right, cityName);
    }
}

int IsRouteIDFound(int *tab, int routeID, int tailleTabRoute){
    for (int i = 0; i < tailleTabRoute; i++) {
        if (tab[i] == routeID) {
            return 1;  // RouteID trouvé
        }
    }
    return 0;
}
// Fonction pour libérer récursivement la mémoire d'un arbre AVL
void freeAVL(AVLNode *root) {
    if (root == NULL) {
        return;
    }

    // Libérer les sous-arbres gauche et droit
    freeAVL(root->left);
    freeAVL(root->right);

    // Libérer la mémoire du nœud courant, y compris routeIDs
    free(root->data.routeIDs);
    free(root);
}

// Fonction pour libérer toute la structure AVL, y compris la racine
void freeAVLTree(AVLNode **root) {
    if (*root != NULL) {
        freeAVL(*root);  // Libérer la mémoire des sous-arbres gauche et droit
        *root = NULL;    // Définir la racine sur NULL après la libération de la sous-structure AVL
    }
}

void printTopCities(CityStats topCities[], int count) {
    printf("Top 10 des villes avec le nombre total de Trajets et Départs :\n");
    for (int i = 0; i < count; i++) {
        printf("%s : %d trajets,  %d départs\n", topCities[i].cityName, topCities[i].NbTrajets, topCities[i].NbDeparts);
    }
    printf("\n");
}

void Departures(AVLNode **avl, const char *line) {
    int stepID;
    char townA[256];

    if (sscanf(line, "%*d;%d;%255[^;];%*[^;];%*f;%*[^;\n]", &stepID, townA) == 2) {
        if (stepID==1){
            AVLNode *existingNode = searchAVL(*avl, townA);

            if (existingNode == NULL) {
                *avl = insertAVLDeparts(*avl, townA);
            } else {
                // Ville existante, incrémenter le compteur de départs
                existingNode->data.NbDeparts++;
            }
        }
     
    } else {
        fprintf(stderr, "Erreur de lecture de la ligne du fichier.\n");
        exit(EXIT_FAILURE);
    }
}

void Trips(AVLNode **avl, const char *line) {
    int routeID;
    char townA[256], townB[256];

    if (sscanf(line, "%d;%*d;%255[^;];%255[^;];%*f;%*[^;\n]", &routeID, townA, townB) == 3) {
        AVLNode *existingNodeA = searchAVL(*avl, townA);
        if (existingNodeA == NULL) {
            *avl = insertAVLTrajets(*avl, townA, 1, routeID);
        } else {
            int ExistingRouteID = IsRouteIDFound(existingNodeA->data.routeIDs, routeID, existingNodeA->data.tailleTabRoute);
        
            if(ExistingRouteID==0){
                existingNodeA->data.NbTrajets++;
                existingNodeA->data.tailleTabRoute++;
                existingNodeA->data.routeIDs = (int *)realloc(existingNodeA->data.routeIDs,
                                                            sizeof(int) * existingNodeA->data.tailleTabRoute);
                if (existingNodeA->data.routeIDs == NULL) {
                    fprintf(stderr, "Erreur d'allocation dynamique de mémoire pour routeIDs.\n");
                    exit(EXIT_FAILURE);
                }
                existingNodeA->data.routeIDs[existingNodeA->data.tailleTabRoute - 1] = routeID;
            } 

        }      

        AVLNode *existingNodeB = searchAVL(*avl, townB);
        if (existingNodeB == NULL) {
            *avl = insertAVLTrajets(*avl, townB, 1, routeID);
        } else {
            int ExistingRouteID = IsRouteIDFound(existingNodeB->data.routeIDs, routeID, existingNodeB->data.tailleTabRoute);
            

            if(ExistingRouteID==0){
                existingNodeB->data.NbTrajets++;
                existingNodeB->data.tailleTabRoute++;
                existingNodeB->data.routeIDs = (int *)realloc(existingNodeB->data.routeIDs,
                                                            sizeof(int) * existingNodeB->data.tailleTabRoute);
                if (existingNodeB->data.routeIDs == NULL) {
                    fprintf(stderr, "Erreur d'allocation dynamique de mémoire pour routeIDs.\n");
                    exit(EXIT_FAILURE);
                }
                existingNodeB->data.routeIDs[existingNodeB->data.tailleTabRoute - 1] = routeID;
            }

        }    
    } else {
        fprintf(stderr, "Erreur de lecture de la ligne du fichier.\n");
        exit(EXIT_FAILURE);
    }
}

void process(FILE *fichier, AVLNode **avl) {
    setlocale(LC_ALL, "");

    char buffer[256];
    if (fgets(buffer, sizeof(buffer), fichier) == NULL) {
        fprintf(stderr, "Erreur lors de la lecture de la première ligne du fichier.\n");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), fichier) != NULL) {
        Trips(avl, line);
        Departures(avl, line);
        
        
    }
}

// Fonction pour récupérer les 10 plus grandes valeurs de count dans un AVL
void getTop10Counts(const AVLNode *root, CityStats *result, int *index) {
    if (root != NULL) {
        // Parcours récursif de l'arbre
        getTop10Counts(root->left, result, index);

        // Traitement du nœud courant
        if (*index < 10 || root->data.NbTrajets > result[9].NbTrajets ||
            (root->data.NbTrajets == result[9].NbTrajets && strcmp(root->data.cityName, result[9].cityName) < 0)) {
            // Si le tableau n'est pas plein, ou la valeur est plus grande que la plus petite du tableau,
            // ou la valeur est égale mais le nom de ville est plus petit alphabétiquement
            if (*index == 10) {
                // Si le tableau est plein, enlever la dernière valeur
                (*index)--;
            }

            // Trouver la bonne position pour insérer le nœud courant dans le tableau
            int i = *index - 1;
            while (i >= 0 && (root->data.NbTrajets > result[i].NbTrajets ||
                              (root->data.NbTrajets == result[i].NbTrajets && strcmp(root->data.cityName, result[i].cityName) < 0))) {
                result[i + 1] = result[i];
                i--;
            }

            // Insérer le nœud courant dans le tableau à la bonne position
            result[i + 1] = root->data;
            (*index)++;
        }

        getTop10Counts(root->right, result, index);
    }
}

// Fonction de comparaison insensible à la casse pour qsort
int compareCityNamesCaseInsensitive(const void *a, const void *b) {
    return strcasecmp(((CityStats *)a)->cityName, ((CityStats *)b)->cityName);
}

void saveResultsToFile(const char *filename, const CityStats *topCities, int topCitiesCount) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier de sauvegarde.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < topCitiesCount; i++) {
        fprintf(file, "%s;%d;%d\n", topCities[i].cityName, topCities[i].NbTrajets, topCities[i].NbDeparts);
    }

    fclose(file);
}

int main() {
    // Revenir en arrière d'un dossier (aller dans le dossier "projet")
    int err = chdir("..");
    if (err)
        errx(1, "main: chdir error");

    // Ouvrir le fichier CSV en mode lecture
    const char *filename = "data/data.csv";
    FILE *fichier = fopen(filename, "r");

    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier CSV.\n");
        return EXIT_FAILURE;
    }
    
    AVLNode *avl = NULL;

    process(fichier, &avl);

    CityStats topCities[10];
    int topCitiesCount = 0;

    getTop10Counts(avl, topCities, &topCitiesCount);

    qsort(topCities, topCitiesCount, sizeof(CityStats), compareCityNamesCaseInsensitive);
    // Affichage du résultat
    printTopCities(topCities,topCitiesCount);
    saveResultsToFile("temp/data_t.txt", topCities, topCitiesCount);
    // Fermer le fichier
    fclose(fichier);
    
    // Libérer la mémoire de l'arbre AVL des départs
    freeAVLTree(&avl);

    return 0; // Terminer le programme avec succès
}
