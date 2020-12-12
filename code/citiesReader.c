#include "citiesReader.h"
#include "arraylist.h"
#include "analyzer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>


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
  return a;
}

tas* creer_tas(int capacite_max){
  tas* t = malloc(sizeof(tas));
  t->nb_element = 0;
  t->capacite_max = capacite_max;
  t->tab = malloc(capacite_max*sizeof(arete));
  return t;
}

graphe* creer_graphe(int n){
  graphe* g = malloc(sizeof(graphe));
  g->nb_sommet=0;
  g->tab_sommet = malloc(n*sizeof(int));
  return g;
}

void entasser(tas* t, int pos){
  while(t->tab[parent(pos)].distance > t->tab[pos].distance){
    echanger(t, parent(pos), pos);
    pos = parent(pos);
  }
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
  if(t->nb_element == t->capacite_max){
    printf("tableau plein\n");
    return;
  }
  t->tab[t->nb_element] = *a;

  entasser(t, t->nb_element);
  t->nb_element++;

}

void inserer_tas2(tas* t, arete* a){
  if(t->nb_element == t->capacite_max){
    t->capacite_max *= 1.5;
    t->tab = (arete *) realloc(t->tab, sizeof(arete) * t->capacite_max);
  }
  t->tab[t->nb_element] = *a;

  entasser(t, t->nb_element);
  t->nb_element++;

}

int filsDroit(int pos){
  return 2*pos+2;
}

int filsGauche(int pos){
  return 2*pos+1;
}

int plusPetitEnfant(tas* t, int pos){
  //printf("%d VS %d\n", filsGauche(pos), filsDroit(pos));
  if(t->tab[filsGauche(pos)].distance < t->tab[filsDroit(pos)].distance)
    return filsGauche(pos);
  return filsDroit(pos);
}

arete supprimer_tas(tas* t){
  int pos=0;
  int tmp=0;
  int continu=0;
  arete a = t->tab[0];
  //printf("\nEXTRACTION DE : %d - %d => %fkm\n", t->tab[0].Ville_D, t->tab[0].Ville_A, t->tab[0].distance);
  echanger(t, 0, t->nb_element-1);
  //printf("taille = %d\n", t->nb_element);

  while((pos < t->nb_element/2) && continu==0){
    //printf("i=%d j=%d\n", pos, plusGrandEnfant(t,pos) );
    if((t->tab[pos].distance > t->tab[plusPetitEnfant(t,pos)].distance) && (plusPetitEnfant(t,pos) < t->nb_element-1)){
      tmp = plusPetitEnfant(t,pos);
      //printf("tmp = %d\n",tmp);
      echanger(t, pos, plusPetitEnfant(t,pos));
      pos = tmp;
    }
    else
      continu = 1;
  }
  t->nb_element--;
  return a;
}

arete supprimer_tas2(tas* t){
  int pos=0;
  int tmp=0;
  int continu=0;
  arete a = t->tab[0];
  //printf("\nEXTRACTION DE : %d - %d => %fkm\n", t->tab[0].Ville_D, t->tab[0].Ville_A, t->tab[0].distance);
  echanger(t, 0, t->nb_element-1);
  //printf("taille = %d\n", t->nb_element);

  while((pos < t->nb_element/2) && continu==0){
    //printf("i=%d j=%d\n", pos, plusGrandEnfant(t,pos) );
    if((t->tab[pos].distance > t->tab[plusPetitEnfant(t,pos)].distance) && (plusPetitEnfant(t,pos) < t->nb_element-1)){
      tmp = plusPetitEnfant(t,pos);
      //printf("tmp = %d\n",tmp);
      echanger(t, pos, plusPetitEnfant(t,pos));
      pos = tmp;
    }
    else
      continu = 1;
  }

  t->nb_element--;

  if (t->nb_element<=t->capacite_max/4 && t->nb_element>4){
    t->capacite_max /=2;
    t->tab = (arete *) realloc(t->tab, sizeof(arete) * t->capacite_max);
  }

  return a;
}


int find2(compressionC* p, int sommet){
  if(p[sommet].parent != sommet)
    p[sommet].parent = find2(p, p[sommet].parent);
  return p[sommet].parent;
}
int union_find2(arete a, compressionC* parent){
  int x = find2(parent, a.Ville_D);
  int y = find2(parent, a.Ville_A);
  if (x==y)
    return 0;
  if(parent[x].rank < parent[y].rank)
    parent[x].parent = y;
  else if (parent[x].rank > parent[y].rank)
    parent[y].parent = x;
  else{
    parent[y].parent = x;
    parent[x].rank++;
  }
  return 1;
}

void affichage(tas* t){
  int i;
  for(i=0; i<t->nb_element; i++)
      printf("%d - %d == %fkm\n", t->tab[i].Ville_D, t->tab[i].Ville_A, t->tab[i].distance);
}

/* ************************************************************************************************************ */
/* ************************************************************************************************************ */

float kruskal_algo2(ListOfCities * cities, graphe* g){
  // Analyse du temps pris par les opérations.
  analyzer_t * time_analysis1 = analyzer_create(); //insertion
  // Analyse de l'espace mémoire inutilisé.
  analyzer_t * memory_analysis1 = analyzer_create(); //insertion
  // Mesure de la durée d'une opération.
  struct timespec before1, after1; //insertion
  clockid_t clk_id1 = CLOCK_REALTIME; //insertion

  tas* t = creer_tas((cities->number*(cities->number-1))/2);
  arete* a;
  arete tmp;
  float distance_total = 0;
  int ext;
  int mem = 0;
  float cout_amortie = 0;
  compressionC* tab = (compressionC*)malloc(cities->number*sizeof(compressionC));

  for(int i=0; i<cities->number; i++){
    tab[i].parent = i;
    tab[i].rank = 0;
    for(int j=i+1; j<cities->number; j++){
      a = creer_arete(i,j,cities->lon[i],cities->lat[i], cities->lon[j],cities->lat[j]);
      clock_gettime(clk_id1, &before1);
      inserer_tas(t,a);
      clock_gettime(clk_id1, &after1);
      // Enregistrement du temps pris par l'opération
      analyzer_append(time_analysis1, after1.tv_nsec - before1.tv_nsec);
      for (int j = 0; j < time_analysis1->size; ++j){
        cout_amortie = cout_amortie + get_amortized_cost(time_analysis1, j);
      }
      // Enregistrement de l'espace mémoire non-utilisé.
      analyzer_append(memory_analysis1,t->capacite_max-t->nb_element);
      mem = mem + (t->capacite_max-t->nb_element);
    }
  }
  /*
  clock_gettime(clk_id1, &before1);
  tmp = supprimer_tas(t);
  clock_gettime(clk_id1, &after1);
  analyzer_append(time_analysis1, after1.tv_nsec - before1.tv_nsec);
  analyzer_append(memory_analysis1,t->capacite_max-t->nb_element);
  mem = mem + (t->capacite_max-t->nb_element);
  */
  while(t->nb_element != 0){
    clock_gettime(clk_id1, &before1);
    tmp = supprimer_tas(t);
    clock_gettime(clk_id1, &after1);

    ext = union_find2(tmp, tab);
    if(ext == 1){
      distance_total = distance_total + tmp.distance;
      g->tab_sommet[g->nb_sommet] = tmp.Ville_D;
      g->nb_sommet++;
      g->tab_sommet[g->nb_sommet] = tmp.Ville_A;
      g->nb_sommet++;
    }

    analyzer_append(time_analysis1, after1.tv_nsec - before1.tv_nsec);
    for (int j = 0; j < time_analysis1->size; ++j){
      cout_amortie = cout_amortie + get_amortized_cost(time_analysis1, j);
    }
    analyzer_append(memory_analysis1,t->capacite_max-t->nb_element);
    mem = mem + (t->capacite_max-t->nb_element);

  }
  free(t->tab);
  free(t);
  fprintf(stderr, "Average cost: %Lf\n", get_average_cost(time_analysis1));
  analyzer_append(memory_analysis1,t->capacite_max-t->nb_element);
  mem = mem + (t->capacite_max-t->nb_element);
  printf("mem == %d\n", mem);
  printf("total amortized = %Lf\n", cout_amortie);

  save_values(time_analysis1, "../plots/temps_amortie_kruskal_fixe_250000.plot");
  save_values(memory_analysis1, "../plots/waste_memory_kruskal_fixe_250000.plot");
  analyzer_destroy(time_analysis1);
  analyzer_destroy(memory_analysis1);
  return distance_total;
}


float kruskal_algo3(ListOfCities * cities, graphe* g){
  // Analyse du temps pris par les opérations.
  analyzer_t * time_analysis1 = analyzer_create(); //insertion
  // Analyse de l'espace mémoire inutilisé.
  analyzer_t * memory_analysis1 = analyzer_create(); //insertion
  // Mesure de la durée d'une opération.
  struct timespec before1, after1; //insertion
  clockid_t clk_id1 = CLOCK_REALTIME; //insertion

  tas* t2 = creer_tas(2);
  arete* a;
  arete tmp2;
  float distance_total = 0;
  int ext;
  int mem = 0;
  float cout_amortie = 0;
  compressionC* tab = (compressionC*)malloc(cities->number*sizeof(compressionC));

  for(int i=0; i<cities->number; i++){
    tab[i].parent = i;
    tab[i].rank = 0;
    for(int j=i+1; j<cities->number; j++){
      a = creer_arete(i,j,cities->lon[i],cities->lat[i], cities->lon[j],cities->lat[j]);
      clock_gettime(clk_id1, &before1);
      inserer_tas2(t2,a);
      clock_gettime(clk_id1, &after1);
      // Enregistrement du temps pris par l'opération
      analyzer_append(time_analysis1, after1.tv_nsec - before1.tv_nsec);
      for (int j = 0; j < time_analysis1->size; ++j){
        cout_amortie = cout_amortie + get_amortized_cost(time_analysis1, j);
      }
      // Enregistrement de l'espace mémoire non-utilisé.
      analyzer_append(memory_analysis1,t2->capacite_max-t2->nb_element);
      mem = mem + (t2->capacite_max-t2->nb_element);
    }
  }
  /*
  clock_gettime(clk_id1, &before1);
  tmp2 = supprimer_tas2(t2);
  clock_gettime(clk_id1, &after1);
  analyzer_append(time_analysis1, after1.tv_nsec - before1.tv_nsec);
  analyzer_append(memory_analysis1,t2->capacite_max-t2->nb_element);
  mem = mem + (t2->capacite_max-t2->nb_element);
 */
  while(t2->nb_element != 0){
    clock_gettime(clk_id1, &before1);
    tmp2 = supprimer_tas2(t2);
    clock_gettime(clk_id1, &after1);

    ext = union_find2(tmp2, tab);
    if(ext == 1){
      distance_total = distance_total + tmp2.distance;
      g->tab_sommet[g->nb_sommet] = tmp2.Ville_D;
      g->nb_sommet++;
      g->tab_sommet[g->nb_sommet] = tmp2.Ville_A;
      g->nb_sommet++;
    }

    analyzer_append(time_analysis1, after1.tv_nsec - before1.tv_nsec);
    for (int j = 0; j < time_analysis1->size; ++j){
      cout_amortie = cout_amortie + get_amortized_cost(time_analysis1, j);
    }
    analyzer_append(memory_analysis1,t2->capacite_max-t2->nb_element);
    mem = mem + (t2->capacite_max-t2->nb_element);
  }
  free(t2->tab);
  free(t2);
  fprintf(stderr, "Average cost: %Lf\n", get_average_cost(time_analysis1));
  analyzer_append(memory_analysis1,t2->capacite_max-t2->nb_element);
  mem = mem + (t2->capacite_max-t2->nb_element);
  printf("mem == %d\n", mem);
  printf("total amortized = %Lf\n", cout_amortie);

  save_values(time_analysis1, "../plots/temps_amortie_kruskal_dynamique_250000.plot");
  save_values(memory_analysis1, "../plots/waste_memory_kruskal_dynamique_250000.plot");
  analyzer_destroy(time_analysis1);
  analyzer_destroy(memory_analysis1);
  return distance_total;
}


float kruskal_algo4(ListOfCities * cities, graphe* g){
  // Analyse du temps pris par les opérations.
  analyzer_t * time_analysis1 = analyzer_create(); //insertion
  // Analyse de l'espace mémoire inutilisé.
  analyzer_t * memory_analysis1 = analyzer_create(); //insertion
  // Mesure de la durée d'une opération.
  struct timespec before1, after1; //insertion
  clockid_t clk_id1 = CLOCK_REALTIME; //insertion

  tas* t = creer_tas((cities->number*(cities->number-1))/2);
  arete* a;
  arete tmp;
  float distance_total = 0;
  int ext;
  int mem = 0;
  float cout_amortie = 0;
  compressionC* tab = (compressionC*)malloc(cities->number*sizeof(compressionC));

  for(int i=0; i<cities->number; i++){
    tab[i].parent = i;
    tab[i].rank = 0;
    for(int j=i+1; j<cities->number; j++){
      a = creer_arete(i,j,cities->lon[i],cities->lat[i], cities->lon[j],cities->lat[j]);
      clock_gettime(clk_id1, &before1);
      inserer_tas(t,a);
      clock_gettime(clk_id1, &after1);
      // Enregistrement du temps pris par l'opération
      analyzer_append(time_analysis1, after1.tv_nsec - before1.tv_nsec);
      for (int j = 0; j < time_analysis1->size; ++j){
        cout_amortie = cout_amortie + get_amortized_cost(time_analysis1, j);
      }
      // Enregistrement de l'espace mémoire non-utilisé.
      analyzer_append(memory_analysis1,t->capacite_max-t->nb_element);
      mem = mem + (t->capacite_max-t->nb_element);
    }
  }
  /*
  clock_gettime(clk_id1, &before1);
  tmp = supprimer_tas(t);
  clock_gettime(clk_id1, &after1);
  analyzer_append(time_analysis1, after1.tv_nsec - before1.tv_nsec);
  analyzer_append(memory_analysis1,t->capacite_max-t->nb_element);
  mem = mem + (t->capacite_max-t->nb_element);
  */
  while(t->nb_element != 0){
    clock_gettime(clk_id1, &before1);
    tmp = supprimer_tas2(t);
    clock_gettime(clk_id1, &after1);

    ext = union_find2(tmp, tab);
    if(ext == 1){
      distance_total = distance_total + tmp.distance;
      g->tab_sommet[g->nb_sommet] = tmp.Ville_D;
      g->nb_sommet++;
      g->tab_sommet[g->nb_sommet] = tmp.Ville_A;
      g->nb_sommet++;
    }

    analyzer_append(time_analysis1, after1.tv_nsec - before1.tv_nsec);
    for (int j = 0; j < time_analysis1->size; ++j){
      cout_amortie = cout_amortie + get_amortized_cost(time_analysis1, j);
    }
    analyzer_append(memory_analysis1,t->capacite_max-t->nb_element);
    mem = mem + (t->capacite_max-t->nb_element);

  }
  free(t->tab);
  free(t);
  fprintf(stderr, "Average cost: %Lf\n", get_average_cost(time_analysis1));
  analyzer_append(memory_analysis1,t->capacite_max-t->nb_element);
  mem = mem + (t->capacite_max-t->nb_element);
  printf("mem == %d\n", mem);
  printf("total amortized = %Lf\n", cout_amortie);

  save_values(time_analysis1, "../plots/temps_amortie_kruskal_fixe+dynamique_250000.plot");
  save_values(memory_analysis1, "../plots/waste_memory_kruskal_fixe+dynamique_250000.plot");
  analyzer_destroy(time_analysis1);
  analyzer_destroy(memory_analysis1);
  return distance_total;
}
