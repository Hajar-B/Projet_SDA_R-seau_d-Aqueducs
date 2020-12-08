#include "citiesReader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "arraylist.h"
#include "analyzer.h"

/*void saveGraph(ListOfCities * cities){
  FILE* fileOut = NULL;
  fileOut = fopen("resuGraph.dat", "w");
  for(int i=0; i<cities->number; i++){
    for(int j=0; j<i; j++){
      fprintf(fileOut, "%i %i\n", i, j);
    }
  }
  fclose(fileOut);
}
*/
void saveGraph(graphe* g){
  FILE* fileOut = NULL;
  fileOut = fopen("resuGraph.dat", "w");
  for(int i=0; i<g->nb_sommet-1; i=i+2){
    fprintf(fileOut, "%i %i\n", g->tab_sommet[i], g->tab_sommet[i+1]);
  }
  fclose(fileOut);
}


int main(int argc, char ** argv) {
  analyzer_t * time_analysis = analyzer_create();
  struct timespec before, after;
  clockid_t clk_id = CLOCK_REALTIME;
  if( argc != 2 ){
    perror("Veuillez passer une population minimale en paramètre du programme");
    return EXIT_FAILURE;
  }
//-----------------------------------------------------------------
//--- READING cities
//-----------------------------------------------------------------

  int popMin = atoi(argv[1]);

  ListOfCities* cities;
  cities = citiesReader(popMin);
  //tas* t = creer_tas((cities->number*(cities->number-1))/2);
  //arete* a;
  float distance;
  graphe* g = creer_graphe((cities->number*(cities->number-1))/2);
  printf("\nnb arete = %d\n",(cities->number*(cities->number-1))/2);

  // ... just to check! This line can be removed.
  //for(int i=0; i<cities->number; i++){
    //printf("%s %i %f %f\n", cities->name[i], cities->pop[i], cities->lon[i], cities->lat[i]);
  //}
  clock_gettime(clk_id, &before);
  distance = kruskal_algo(cities, g);
  clock_gettime(clk_id, &after);
  printf("\ndistance = %f\n", distance);
  analyzer_append(time_analysis, after.tv_nsec-before.tv_nsec);
  fprintf(stderr,"Total cost: %Lf\n", get_total_cost(time_analysis));

//-----------------------------------------------------------------
//--- COMPUTING complete graph
//-----------------------------------------------------------------


  /*
   Écriture du graphe (chaque ligne correspond à une arête)
   !!! Ci-dessous, on écrit le graphe complet pour l'exemple.
   Dans votre rendu, cette fonction prendra un graphe en argument,
   pas une liste de villes.
  */
  saveGraph(g);
  analyzer_destroy(time_analysis);
  freeListOfCities(cities);
  return 0;
}
