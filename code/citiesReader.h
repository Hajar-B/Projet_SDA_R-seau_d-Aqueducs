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
  int est_visite; //0 si on pas visité l'arête sinon 1
};

typedef struct tas tas;

struct tas{
  int nb_element;   //nombre d'élément dans le tas
  int capacite_max; // nombre maximum d'élément dans le tas
  arete* tab;         //tableau
};

ListOfCities* citiesReader(int popMin);
void freeListOfCities(ListOfCities * cities);

float distance(float lon1, float lat1, float lon2, float lat2);
arete* creer_arete(int ville_d, int ville_a, float lon1, float lat1, float lon2, float lat2);
void free_arete(arete* a);

tas* creer_tas(int capacite_max);
void free_tas(tas* t);

void entasser(tas* t, int pos);
int parent(int pos);
void echanger(tas* t, int pos1, int pos2);
void inserer_tas(tas* t, arete* a);

int filsDroit(int pos);
int filsGauche(int pos);
int plusGrandEnfant(tas* t, int pos);
void supprimer_tas(tas* t);

void affichage(tas* t);

#endif
 
