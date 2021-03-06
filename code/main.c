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
  analyzer_t * time_analysis1 = analyzer_create();
  analyzer_t * time_analysis2 = analyzer_create();
  analyzer_t * time_analysis3 = analyzer_create();
  analyzer_t * time_analysis4 = analyzer_create();

  struct timespec before1, after1;
  clockid_t clk_id1 = CLOCK_REALTIME;

  struct timespec before2, after2;
  clockid_t clk_id2 = CLOCK_REALTIME;

  struct timespec before3, after3;
  clockid_t clk_id3 = CLOCK_REALTIME;

  struct timespec before4, after4;
  clockid_t clk_id4 = CLOCK_REALTIME;

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
  graphe* g1 = creer_graphe((cities->number*(cities->number-1))/2);
  graphe* g2 = creer_graphe((cities->number*(cities->number-1))/2);
  graphe* g3 = creer_graphe((cities->number*(cities->number-1))/2);

  printf("\n*** FIXE UNION O(1) ***\n");
  clock_gettime(clk_id1, &before1);
  distance = kruskal_algo1(cities, g);
  clock_gettime(clk_id1, &after1);
  printf("\ndistance = %f\n", distance);
  analyzer_append(time_analysis1, after1.tv_nsec-before1.tv_nsec);
  fprintf(stderr,"Total cost: %Lf\n", get_total_cost(time_analysis1));

  printf("\n*** DYNAMIQUE UNION O(1) ***\n");
  clock_gettime(clk_id2, &before2);
  distance = kruskal_algo2(cities, g1);
  clock_gettime(clk_id2, &after2);
  printf("\ndistance = %f\n", distance);
  analyzer_append(time_analysis2, after2.tv_nsec-before2.tv_nsec);
  fprintf(stderr,"Total cost: %Lf\n", get_total_cost(time_analysis2));

  printf("\n*** FIXE DYNAMIQUE UNION O(1) ***\n");
  clock_gettime(clk_id3, &before3);
  distance = kruskal_algo3(cities, g2);
  clock_gettime(clk_id3, &after3);
  printf("\ndistance = %f\n", distance);
  analyzer_append(time_analysis3, after3.tv_nsec-before3.tv_nsec);
  fprintf(stderr,"Total cost: %Lf\n", get_total_cost(time_analysis3));

  printf("\n*** DYNAMIQUE FIXE UNION O(1) ***\n");
  clock_gettime(clk_id4, &before4);
  distance = kruskal_algo4(cities, g3);
  clock_gettime(clk_id4, &after4);
  printf("\ndistance = %f\n", distance);
  analyzer_append(time_analysis4, after4.tv_nsec-before4.tv_nsec);
  fprintf(stderr,"Total cost: %Lf\n", get_total_cost(time_analysis4));


//-----------------------------------------------------------------
//--- COMPUTING complete graph
//-----------------------------------------------------------------


  /*
   Écriture du graphe (chaque ligne correspond à une arête)
   !!! Ci-dessous, on écrit le graphe complet pour l'exemple.
   Dans votre rendu, cette fonction prendra un graphe en argument,
   pas une liste de villes.
  */
  saveGraph(g1);
  free(g->tab_sommet);
  free(g1->tab_sommet);
  free(g2->tab_sommet);
  free(g3->tab_sommet);
  free(g);
  free(g1);
  free(g2);
  free(g3);
  analyzer_destroy(time_analysis1);
  analyzer_destroy(time_analysis2);
  analyzer_destroy(time_analysis3);
  analyzer_destroy(time_analysis4);
  freeListOfCities(cities);
  return 0;
}
