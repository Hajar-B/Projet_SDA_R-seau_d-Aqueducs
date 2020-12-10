#ifndef CITIES_READER
#define CITIES_READER

typedef struct ListOfCities ListOfCities;

struct ListOfCities
{
  int    number;
  char** name;
  int*   pop;
  float* lon;
  float* lat;
};

typedef struct arete arete;

struct arete{
  int Ville_D;    //ville de depart
  int Ville_A;    //ville d'arrivé
  float distance;   //distance entre les deux villes
};

typedef struct tas tas;

struct tas{
  int nb_element;   //nombre d'élément dans le tas
  int capacite_max; // nombre maximum d'élément dans le tas
  arete* tab;         //tableau
};

typedef struct graphe graphe;

struct graphe{
  int nb_sommet;
  int* tab_sommet;
};

typedef struct compressionC compressionC;

struct compressionC{
  int parent;
  int rank;
};

ListOfCities* citiesReader(int popMin);
void freeListOfCities(ListOfCities * cities);

float distance(float lon1, float lat1, float lon2, float lat2);
arete* creer_arete(int ville_d, int ville_a, float lon1, float lat1, float lon2, float lat2);
tas* creer_tas(int capacite_max);
graphe* creer_graphe(int n);

void entasser(tas* t, int pos);
int parent(int pos);
void echanger(tas* t, int pos1, int pos2);
void inserer_tas(tas* t, arete* a);
void inserer_tas2(tas* t, arete* a);
//void inserer_tas2(tas* t, arete* a);

int filsDroit(int pos);
int filsGauche(int pos);
int plusPetitEnfant(tas* t, int pos);
arete supprimer_tas(tas* t);
arete supprimer_tas2(tas* t);

int find(int* parent, int sommet);
int union_find(arete a, int* parent);

int find2(compressionC* p, int sommet);
int union_find2(arete a, compressionC* parent);

float kruskal_algo(ListOfCities * cities, graphe* g);
float kruskal_algo2(ListOfCities * cities, graphe* g);
float kruskal_algo3(ListOfCities * cities, graphe* g);

float kruskal_algo4(ListOfCities * cities, graphe* g);
float kruskal_algo5(ListOfCities * cities, graphe* g);

float kruskal_algo6(ListOfCities * cities, graphe* g);
float kruskal_algo7(ListOfCities * cities, graphe* g);

void affichage(tas* t);

#endif
