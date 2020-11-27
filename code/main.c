#include "citiesReader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <math.h>

void saveGraph(ListOfCities * cities){
  FILE* fileOut = NULL;
  fileOut = fopen("resuGraph.dat", "w");
  for(int i=0; i<cities->number; i++){
    for(int j=0; j<i; j++){
      fprintf(fileOut, "%i %i\n", i, j);
    }
  }
  fclose(fileOut);
}


int main(int argc, char ** argv) {


  if( argc != 2 ){
    perror("Veuillez passer une population minimale en paramètre du programme");
    return EXIT_FAILURE;
  }
//-----------------------------------------------------------------
//--- READING cities
//-----------------------------------------------------------------
  int popMin = atoi(argv[1]);

  ListOfCities* cities;
  tas* t;
  arete* a;
  int nb_arete;
  cities = citiesReader(popMin);
  //nb_arete = (cities->number*(cities->number-1))/2;
  //t = creer_tas(nb_arete);
  // ... just to check! This line can be removed.

  for(int i=0; i<cities->number; i++){
    printf("%s %i %f %f\n", cities->name[i], cities->pop[i], cities->lon[i], cities->lat[i]);
  }

  kruskal_algo(cities);
  /*
  for(int i=0; i<cities->number; i++){
    for(int j=i+1; j<cities->number; j++){
      a = creer_arete(i,j,cities->lon[i],cities->lat[i], cities->lon[j],cities->lat[j]);
      //printf("\n==== %d %d %f\n", i,j, a->distance);
      inserer_tas(t,a);
      //kruskal_algo(t,a);
    }
    //break;
  }
  printf("\n\n");
  affichage(t);
  printf("\n");
  supprimer_tas(t);
  //affichage(t);
  supprimer_tas(t);
  //affichage(t);
  supprimer_tas(t);
  supprimer_tas(t);
  supprimer_tas(t);
  supprimer_tas(t);
  supprimer_tas(t);
  free_tas(t);
  */
//-----------------------------------------------------------------
//--- COMPUTING complete graph
//-----------------------------------------------------------------


  /*
   Écriture du graphe (chaque ligne correspond à une arête)
   !!! Ci-dessous, on écrit le graphe complet pour l'exemple.
   Dans votre rendu, cette fonction prendra un graphe en argument,
   pas une liste de villes.
  */
  saveGraph(cities);


  freeListOfCities(cities);

  return 0;
}
