#include <stdio.h>
#include <stdlib.h>
#include "modul.h"

int main(int argc, char ** argv) {
  t_opcje opcje; /*iniciowanie zmiennej typu t_opcje pod nazwa opcje*/
  t_obraz obraz;  /*iniciowanie zmiennej typu t_obraz pod nazwa obraz*/

  /*Zapis oraz sprawdzenie opcji wywołania programu*/         
  if(przetwarzaj_opcje(argc,argv,&opcje)!=0) exit(-1); /*Zakończ program gdy funckja zwróci błąd*/
  wykonaj_opcje(&opcje,&obraz,argc,argv);      /*wykonujemy opcje programu*/                                       
  return 0; 
}