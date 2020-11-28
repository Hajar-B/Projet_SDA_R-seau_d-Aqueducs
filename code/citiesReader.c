#include "citiesReader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


/**
   Create a data structure ListOfCities and allocates the necessary space to store
   a given number of cities.
   @param number is the number of cities the data structure will store.
   @return a reference to a ListOfCities.
   @requires number > 0
 */
ListOfCities* createCities(int number){
    ListOfCities* cities = malloc(sizeof(ListOfCities));
    int i;

    cities->number = number;
    // Allocate arrays
    cities->name = malloc(cities->number*sizeof(char*));
    for(i = 0; i < number; i++)
      cities->name[i] = (char*) malloc(32*sizeof(char));
    cities->pop  = malloc(cities->number*sizeof(int));
    cities->lon  = malloc(cities->number*sizeof(float));
    cities->lat  = malloc(cities->number*sizeof(float));

    return cities;
}

/**
   Liberate space occupied by a ListOfCities
   @param cities is a ListOfCities.
   @requires cities != NULL
*/
void freeListOfCities(ListOfCities * cities){
  free(cities->name);
  free(cities->pop);
  free(cities->lon);
  free(cities->lat);
  free(cities);
}



/**
   Count the number of cities with population greater than or equal to 'popMin'
   @param inputFile is a CSV file containing a list of cities with their
   population and coordinates.
   @param popMin is a threshold above which a city will be counted
   @return number of cities with a population above popMin
   @requires inputFile != NULL
   @requires popMin > 0
*/
int countCitiesWithMinPopulation(FILE* inputFile, int popMin){
  int number = 0, myPop , i;
  char line[512];
  const char s[2] = ",";
  char *token;

  while(fgets(line, 512, inputFile) != NULL){
    /* Select the 14th coma separated value and stores it in 'token' */
    token = strtok(line, s);
    for(i = 0; i < 14; i++) token = strtok(NULL, s);

    myPop = atoi(token);
    if(myPop >= popMin) number++;
  }
  /* Rewind file */
  fseek(inputFile, 0, SEEK_SET);

  return number;
}

/**
   Load cities population and coordinates from file to a ListOfCities data structure.
   @param inputFile is the file from which data are loaded.
   @param popMin is the minimum polution a city has to have in order to be loaded.
   @param cities is a ListOfCities data structure in which data will be loaded.
   @requires inputFile != NULL
   @requires popMin > 0
   @requires cities != NULL and cities->number = countCitiesWithMinPopulation(inputFile, popMin)
 */
void loadListOfCities(FILE* inputFile, int popMin, ListOfCities* cities){
  // Reading/Saving data
  int index=0, i, myPop;
  char line[512];
  const char s[2] = ",";
  char *token;
  char myName[32];

  while(fgets(line, 512, inputFile) != NULL){

    token = strtok(line, s);
    for(i=0; i<3;  i++) token = strtok(NULL, s);
    strncpy(myName, token, 32);
    for(i=0; i<11; i++) token = strtok(NULL, s);
    myPop = atoi(token);

    if(myPop >= popMin){
      for(i=0; i<5;  i++) token = strtok(NULL, s);
      cities->lon[index] = atof(token);
      for(i=0; i<1;  i++) token = strtok(NULL, s);
      cities->lat[index] = atof(token);

      strncpy(cities->name[index], myName, 32);
      cities->pop[index] = myPop;
      index++;
    }
  }

}

/**
   Writes content of a ListOfCities data structure in the 'resuCities.dat' file.
   @param cities is the ListOfCities data structure to save of disk.
   @requires cities != NULL
*/
void saveListOfCities(ListOfCities* cities){
  FILE* outputFile = NULL;
  if( (outputFile = fopen("resuCities.dat", "w")) == NULL){
    perror("Could not open file resuCities.dat");
    exit(-1);
  }

  for(int i=0; i<cities->number; i++)
    fprintf(outputFile, "%i %f %f\n", cities->pop[i], cities->lon[i], cities->lat[i]);
  fclose(outputFile);
}


/**
   Loads the name, population and coordinates of all the cities above
   a given population threshold, store the result in a file named 'resuCities.dat'
   and return a ListOfCities data structure containing the list.
   @param popMin is the population threshold for a city to be considered.
   @return a reference to a ListOfCities containing the name, population and coordinates of all the    cities whose population is above 'popMin'.
   @requires popMin > 0
   @requires 'citiesList.csv' file to exist on disk and to be readable by the program.
*/
ListOfCities* citiesReader(int popMin){
  /* READING cities with population greater than or equal to 'popMin' */
  printf("== Reading cities with population >= %i from 'citiesList.csv' ==\n", popMin);

  FILE* inputFile = NULL;
  if( (inputFile = fopen("citiesList.csv", "r")) == NULL){
    perror("Could not open file citiesList.csv");
    exit(-1);
  }

  ListOfCities* cities = createCities(countCitiesWithMinPopulation(inputFile, popMin));
  loadListOfCities(inputFile, popMin, cities);

  fclose(inputFile);

  /* WRITING cities with population greater than or equal to 'popMin' */
  printf("== Writing cities with population >= %i in 'resuCities.dat' ==\n", popMin);
  saveListOfCities(cities);

  return cities;
}

/* ***************************************************************************************************** */
float distance(float lon1, float lat1, float lon2, float lat2){
  float distance, val;
  val = 3.14159265/180;
  distance = 6371 * acos(sin(lat1*val) * sin(lat2*val) + cos(lon1*val - lon2*val) * cos(lat1*val) * cos(lat2*val));
  return distance;
}

arete* creer_arete(int ville_d, int ville_a, float lon1, float lat1, float lon2, float lat2){
  arete* a = malloc(sizeof(arete*));
  a->Ville_D = ville_d;
  a->Ville_A = ville_a;
  a->distance = distance(lon1, lat1, lon2, lat2);
  a->est_visite = 0;
}

void free_arete(arete* a){
  free(a);
}

tas* creer_tas(int capacite_max){
  tas* t = malloc(sizeof(tas*));
  t->nb_element = 0;
  t->capacite_max = capacite_max;
  t->tab = malloc(capacite_max*sizeof(arete*));
  return t;
}

void free_tas(tas* t){
  /*int i;
  for (i=0; i<t->nb_element; i++)
    free_arete(&(t->tab[i]));*/
  free(t);
}

void entasser(tas* t, int pos){
  //int i;
  //printf("debut entasser\n");

  //for(i=0; i<t->nb_element; i++)
  //  printf("%d - %d = %fkm\n", t->tab[i].Ville_D, t->tab[i].Ville_A, t->tab[i].distance);

  while(t->tab[parent(pos)].distance > t->tab[pos].distance){
    //printf("%f / %f\n",t->tab[parent(pos)].distance, t->tab[pos].distance);
    echanger(t, parent(pos), pos);
    pos = parent(pos);
  }

  //printf("fin entasser\n");
}

int parent(int pos){
  return (pos-1)/2;
}

void echanger(tas* t, int pos1, int pos2){
  arete tmp;

  tmp = (t->tab[pos1]);
  t->tab[pos1] = t->tab[pos2];
  t->tab[pos2] = tmp;
}

void inserer_tas(tas* t, arete* a){
  //int i;
  if(t->nb_element == t->capacite_max){
    printf("tableau plein\n");
    return;
  }
  t->tab[t->nb_element] = *a;
  entasser(t, t->nb_element);
  t->nb_element++;
  //for(i=0; i<t->nb_element; i++)
    //printf("%d - %d -> %fkm\n", t->tab[i].Ville_D, t->tab[i].Ville_A, t->tab[i].distance);
}

int filsDroit(int pos){
  return 2*pos+2;
}

int filsGauche(int pos){
  return 2*pos+1;
}

int plusGrandEnfant(tas* t, int pos){
  if(t->tab[filsGauche(pos)].distance < t->tab[filsDroit(pos)].distance)
    return filsGauche(pos);
  return filsDroit(pos);
}

arete supprimer_tas(tas* t){
  int pos=0;
  int tmp=0;
  arete a = t->tab[0];
  printf("\nEXTRACTION DE : %d - %d => %fkm\n", t->tab[0].Ville_D, t->tab[0].Ville_A, t->tab[0].distance);
  //printf("taille = %d\n", t->nb_element-1);

  echanger(t, 0, t->nb_element-1);

  while((t->tab[pos].distance > t->tab[plusGrandEnfant(t,pos)].distance) && (plusGrandEnfant(t,pos) < t->nb_element-1)){
    tmp = plusGrandEnfant(t,pos);
    //printf("%d %d\n", pos, plusGrandEnfant(t,pos));
    echanger(t, pos, plusGrandEnfant(t,pos));

    pos = tmp;
    //affichage(t);
    //printf("\n*********-----------------------------*********\n");
  }
  t->nb_element--;
  return a;
}

int find(int* parent, int sommet){

  if(parent[sommet] == -1)
    return sommet;
  return find(parent, parent[sommet]);
    //sommet = find(parent, parent[sommet]);
  //return sommet;
}

int union_find(arete a, int* parent){
  int x, y, xset, yset;

  x = find(parent, a.Ville_D);
  //printf("x = %d\n", x);
  y = find(parent, a.Ville_A);
  //printf("y = %d\n", y);
  if(x == y)
    return 0;
  xset = find(parent, x);
  //printf("xset = %d\n", xset);
  yset = find(parent, y);
  //printf("yset = %d\n", yset);
  if(xset != yset){
    parent[xset] = yset;
    return 1;
  }
  return 0;
}

void kruskal_algo(ListOfCities * cities){
  int nb_arete = (cities->number*(cities->number-1))/2;
  tas* t = creer_tas(nb_arete);
  arete tmp, *a;
  int ext;



  for(int i=0; i<cities->number; i++){
    for(int j=i+1; j<cities->number; j++){
      a = creer_arete(i,j,cities->lon[i],cities->lat[i], cities->lon[j],cities->lat[j]);
      inserer_tas(t,a);
    }
  }
  int* parent = (int*)malloc(cities->number*sizeof(int));
  memset(parent, -1, sizeof(int)*cities->number);
  tmp = supprimer_tas(t);
  //printf("%d - %d\n", tmp.Ville_D, tmp.Ville_A);
  while(t->nb_element != 0){
    ext = union_find(tmp, parent);
    if(ext == 1)
      printf("\narete %d - %d est un success\n", tmp.Ville_D, tmp.Ville_A);
    /*
    if(ext == 0){
      printf("\narete %d - %d forme un cycle\n", tmp.Ville_D, tmp.Ville_A);
    }
    else{
      printf("\narete %d - %d est un success\n", tmp.Ville_D, tmp.Ville_A);
    }*/
    //for(int k=0; k<cities->number; k++)
      //printf(" %d ", parent[k]);
    tmp = supprimer_tas(t);
    //printf("\n***********arete %d - %d forme un cyle\n", tmp.Ville_D, tmp.Ville_A);
    //for(int k=0; k<cities->number; k++)
    //printf("hajar\n");
    //printf(" %d ", parent[1]);
  }

  free_tas(t);
  //free(tmp);
  //free(ext);
  free(a);
  free(parent);
}

void affichage(tas* t){
  int i;
  for(i=0; i<t->nb_element; i++)
      printf("%d - %d == %fkm\n", t->tab[i].Ville_D, t->tab[i].Ville_A, t->tab[i].distance);
}
