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

void saveGraph2(tas * cities){
  FILE* fileOut = NULL;
  fileOut = fopen("resuGraph.dat", "w");
  for(int i=0; i<cities->nb_element; i++){
    fprintf(fileOut, "%i %i\n", cities->tab[i].Ville_D, cities->tab[i].Ville_A);
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
  printf("%d\n", atoi(argv[1]));

  ListOfCities* cities;
  tas* t, *graphe;
  arete* a;
  int nb_arete;
  char fichier[100] = "graphe";
  char extension[100] = ".dat";
  cities = citiesReader(popMin);
  graphe = creer_tas((cities->number*(cities->number-1))/2);
  /*
  for(int i=0; i<cities->number; i++){
    printf("%s %i %f %f\n", cities->name[i], cities->pop[i], cities->lon[i], cities->lat[i]);
  }*/
  float distance_total = kruskal_algo(cities, strcat(strcat(fichier, argv[1]), extension), graphe);
  affichage(graphe);
  printf("distance total : %f\n",distance_total);


//-----------------------------------------------------------------
//--- COMPUTING complete graph
//-----------------------------------------------------------------


  /*
   Écriture du graphe (chaque ligne correspond à une arête)
   !!! Ci-dessous, on écrit le graphe complet pour l'exemple.
   Dans votre rendu, cette fonction prendra un graphe en argument,
   pas une liste de villes.
  */
  saveGraph2(graphe);


  freeListOfCities(cities);

  return 0;
}
