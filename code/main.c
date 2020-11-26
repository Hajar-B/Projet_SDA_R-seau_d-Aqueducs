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
  float distance;
  /*
  if( argc != 2 ){
    perror("Veuillez passer une population minimale en paramètre du programme");
    return EXIT_FAILURE;
  }*/
//-----------------------------------------------------------------
//--- READING cities
//-----------------------------------------------------------------
  int popMin = atoi(argv[1]);

  ListOfCities* cities;
  cities = citiesReader(popMin);

  // ... just to check! This line can be removed.
  for(int i=0; i<cities->number; i++){
    printf("%s %i %f %f\n", cities->name[i], cities->pop[i], cities->lon[i], cities->lat[i]);
    //distance = 6371*acos( sin(cities->lat[i]) * sin(cities->lat[i+1]) + cos(cities->lat[i] - cities->lat[i+1])*cos(cities->lon[i])*cos(cities->lon[i+1]));
    distance = 6371*acos( sin(cities->lat[i]) * sin(cities->lat[i+1]) + cos(cities->lat[i] - cities->lat[i+1])*cos(cities->lat[i])*cos(cities->lat[i+1]));
    printf("distance entre %s et %s = %f\n\n", cities->name[i], cities->name[i+1],distance);
  }

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
