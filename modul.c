#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>              /*biblioteka użyta do operacji matematycznych*/
#include "modul.h"             /*przydzielenie nagłówków funckji oraz struktur danych*/

#define DL_LINII 1024             /* Dlugosc buforow pomocniczych */
#define W_OK 0                   /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA -1    /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY   -2
#define B_BRAKWARTOSCI  -3
#define B_BRAKPLIKU   -4
#define B_BLEDNE_DANE -5
#define B_NIEPOPRAWNY_PLIK -6
#define B_BRAK_PAMIECI -7

/****************************************************************************************
 * Funkcja wyzeruj_opcje zeruje opcje na poczatku działania programu i ustawia          *
 * uchwyty od plików na wartość NULL                                                    *
 *\param[in] wybor wskaznik na strukture opcje zawierajaca wszystkie wczytane parametry *
oraz argumenty w momencie wywolania programau                                           *
*****************************************************************************************/

void wyzeruj_opcje(t_opcje * wybor) {
  /*przypisanie zera symbolizuje nie wybrania okreslonej opcji*/
    wybor->plik_we=NULL;
    wybor->plik_wy=NULL;
    wybor->negatyw=0;
    wybor->konturowanie=0;
    wybor->progowanie=0;
    wybor->wyswietlenie=0;
    wybor->korekcja_gamma=0;
    wybor->roz_histogramu=0;
    wybor->progowanie_bieli=0;
    wybor->progowanie_czerni=0;
    wybor->zmiana_poziomow=0;
    wybor->konwersja_do_szarosci=0;
    wybor->przetwarzaj_kolor=0;
    wybor->rozmywanie_poziome=0;
    wybor->rozmywanie_pionowe=0;
}
/***************************************************************************/
/* Funkcja przetwarzaj_opcje rozpoznaje opcje wywolania programu zapisane  */
/* w tablicy argv                                                          */
/* i zapisuje je w strukturze opcje                                        */
/* Skladnia opcji wywolania programu                                       */
/*         program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-r] [-d] }     */
/* Argumenty funkcji:                                                      */
/*         argc  -  liczba argumentow wywolania wraz z nazwa programu      */
/*         argv  -  tablica argumentow wywolania                           */
/*         wybor -  struktura z informacjami o wywolanych opcjach          */
/* PRE:                                                                    */
/*      brak                                                               */
/* POST:                                                                   */
/*      funkcja otwiera odpowiednie pliki, zwraca uchwyty do nich          */
/*      w strukturze wybór, do tego ustawia wartość pola równą 1           */
/*   dla opcji, ktore poprawnie wystapily w linii wywolania programu,      */
/*	pola opcji nie wystepujacych w wywolaniu ustawione sa na 0;            */
/*	zwraca wartosc W_OK (0), gdy wywolanie bylo poprawne                   */
/*	lub kod bledu zdefiniowany stalymi B_* (<0)                            */
/* UWAGA:                                                                  */
/*      funkcja nie sprawdza istnienia i praw dostepu do plikow            */
/*      w takich przypadkach zwracane uchwyty maja wartosc NULL.           */    
/***************************************************************************/   

int przetwarzaj_opcje(int argc, char **argv, t_opcje *wybor) {
    int i, prog, prog_cz, prog_b, bialy, czarny; /*zmienne pomocnicze do wczytywania argumentow*/
    float gamma;
    char *nazwa_pliku_we, *nazwa_pliku_wy;
    char kolorek;

    wyzeruj_opcje(wybor);
    wybor->plik_wy=stdout;        /* na wypadek gdy nie podano opcji "-o" */

    for (i=1; i<argc; i++) {
      if (argv[i][0] != '-'){  /* blad: to nie jest opcja - brak znaku "-" */
          fprintf(stderr,"\nBlad! Niepoprawna opcja!\n\n");
          return B_NIEPOPRAWNAOPCJA;} 
    switch (argv[i][1]) {
    case 'i': {                 /* opcja z nazwa pliku wejsciowego */
        if (++i<argc) {   /* wczytujemy kolejny argument jako nazwe pliku */
	          nazwa_pliku_we=argv[i];
        if (strcmp(nazwa_pliku_we,"-")==0) /* gdy nazwa jest "-"        */
            wybor->plik_we=stdin;            /* ustwiamy wejscie na stdin*/
    else                             /* otwieramy wskazany plik   */
	      wybor->plik_we=fopen(nazwa_pliku_we,"r");
    } else {
        fprintf(stderr,"\nBlad brak nazwy pliku\n\n");
	      return B_BRAKNAZWY;}                   /* blad: brak nazwy pliku */
        break;
    }
    case 'o': {                 /* opcja z nazwa pliku wyjsciowego */
      if (++i<argc) {   /* wczytujemy kolejny argument jako nazwe pliku */
	nazwa_pliku_wy=argv[i];
	if (strcmp(nazwa_pliku_wy,"-")==0)/* gdy nazwa jest "-"         */
	    wybor->plik_wy=stdout;          /* ustwiamy wyjscie na stdout */
  else  {                          /* otwieramy wskazany plik    */
      if(argv[i][0] != '-'){    /*Na wszelki wypadek*/    
	  wybor->plik_wy=fopen(nazwa_pliku_wy,"w"); /*zapisujemy nazwe pliku*/
      wybor->nazwa_pliku=nazwa_pliku_wy;}       /*aby go wyswietlic w razie potrzeby*/
    else{ 
      fprintf(stderr,"\nBlad, Brak nazwy pliku\n\n");
      return B_BRAKNAZWY;}}  
    }else{ 
  fprintf(stderr,"\nBlad, Brak nazwy pliku\n\n");
	return B_BRAKNAZWY;}                 /* blad: brak nazwy pliku */
      break;
    }
     
    case 'n': {                 /* mamy wykonac negatyw */
      wybor->negatyw=1;
      break;
    }
    case 'k': {                 /* mamy wykonac konturowanie */
      wybor->konturowanie=1;
      break;
    }
    case 'd': {                 /* mamy wyswietlic obraz */
      wybor->wyswietlenie=1;
      break;
    }
    case 'h' : {               /*mamy wykonac roz_histogramu*/
      wybor->roz_histogramu=1;
      break;
    }
    case 'g': {              /*mamy wykonac korekcje_gamma*/
      if((++i<argc)&&(argv[i][0]!='-')) {  /* wczytujemy kolejny argument jako wspolczynnik gamma*/
  if(sscanf(argv[i],"%f",&gamma)==1) { 
    wybor->korekcja_gamma=1;
    wybor->w_gamma=gamma;
  } else { /*blad bledna wartosc wspolczynnika gamma*/
    fprintf(stderr,"\nBlad, Podano nieprawidlowa wartosc wspolczynnika gamma\n\n");
    return B_BRAKWARTOSCI;}
      } else{ /*blad brak wspolczynnika gamma*/
  fprintf(stderr,"\nBlad, Nie podano wartosci wspolczynnika gamma\n\n");
  return B_BRAKWARTOSCI;}
      break;
    }               /*mamy wykonac zmiane_poziomow*/
    case 'z' : {
      if((++i<argc)&&(argv[i][0]!='-')) {  /*wczytujemy kolejny argument czerni*/
  if(sscanf(argv[i],"%d", &czarny)==1) {
    wybor->czern=czarny;
  } else { /*blad nieprawidlowy argument czerni*/
    fprintf(stderr,"\nBlad, Podano nieprawidlową wartośc czerni\n\n");
    return B_BRAKWARTOSCI;}
     } else { /*blad brak argumentow*/
  fprintf(stderr,"\nBlad, Nie Podano argumentu czerni oraz bieli\n\n");
  return B_BRAKWARTOSCI;}
     if((++i<argc)&&(argv[i][0]!='-')) {     /*Wczytujemy argument bieli*/
  if(sscanf(argv[i],"%d", &bialy)==1) {
    wybor->biel=bialy;
    wybor->zmiana_poziomow=1;
  } else { /*blad bledny parametr*/
    fprintf(stderr,"\nBlad, Podano nieprawidlową wartość bieli\n\n");
    return B_BRAKWARTOSCI;}
  } else { /*blad brak parametru*/
  fprintf(stderr,"\nBlad, Nie Podano drugiego argumentu bieli\n\n");
  return B_BRAKWARTOSCI;}
      break;
  }             /*mamy wykonac operacje konwersji_do_szarosci*/
    case 'm' : {   
      if((++i<argc)&&(argv[i][0]!='-')){ /* Wczytujemy kolejny arguemnt*/
  if(sscanf(argv[i],"%c", &kolorek)==1) {
        if((kolorek=='r') || (kolorek=='g') || (kolorek=='b')) {
            wybor->kolor=kolorek;
            wybor->przetwarzaj_kolor=1;
        } /*argument ma wartosc 's' wykonujemy konwersje_do_szarosci*/
        else if(kolorek=='s') wybor->konwersja_do_szarosci=1;
        else{ /*blad bledna opcja */
         fprintf(stderr,"\nBlad! Niepoprawny argument do opcji -m!\n\n");
         return B_NIEPOPRAWNAOPCJA;}
  } else {/*blad nieprawidlowy parametr*/
    fprintf(stderr,"\nBlad, Podano nieprawidlowy parametr\n\n");
    return B_BRAKWARTOSCI;}
  } else { /*blad brak parametru*/
  fprintf(stderr,"\nBlad,Nie podano koloru do przetworzenia badź parametru 's'!\n\n");
  return B_BRAKWARTOSCI;}
      break;
  }             
    case 'p': {
      switch(argv[i][2]) {     /*mamy wykonac progowanie_czerni*/
        case 'c' : {
            if((++i<argc)&&(argv[i][0]!='-')) {      /*wczytujemy kolejny argument progu czerni*/
    if(sscanf(argv[i],"%d", &prog_cz)==1) {
      wybor->progowanie_czerni=1;
      wybor->w_progu_czerni=prog_cz;
    } else { /*blad bledny argument*/
      fprintf(stderr,"\nBlad, Podano nieprawidlowa wartosc progu czerni\n\n");
      return B_BRAKWARTOSCI;}
      } else { /*blad brak argumentu*/
      fprintf(stderr,"\nBlad,Nie podano wartosci progu czerni\n\n");   
    return B_BRAKWARTOSCI;}
      break;
      }               /*mamy wykonac progowanie_bieli*/
      case 'b' : {
          if((++i<argc)&&(argv[i][0]!='-')) {    /*wczytujemy kolejny argument progu bieli*/
    if(sscanf(argv[i],"%d", &prog_b)==1) {
      wybor->progowanie_bieli=1;
      wybor->w_progu_bieli=prog_b;
    } else { /*blad bledny argument*/
    fprintf(stderr,"\nBlad, Podano nieprawidlowa wartosc progu bieli\n\n");
    return B_BRAKWARTOSCI;}
      } else { /*blad brak argumentu*/
    fprintf(stderr,"\nBlad,Nie podano wartosci progu bieli\n\n");
    return B_BRAKWARTOSCI;}
      break;
      }          /*mamy wykonac progowanie*/
      case '\0' : {
          if ((++i<argc)&&(argv[i][0]!='-')) { /* wczytujemy kolejny argument jako wartosc progu */
	  if (sscanf(argv[i],"%d",&prog)==1) {
	    wybor->progowanie=1;
      wybor->w_progu=prog;
	  } else {
    fprintf(stderr,"\nBlad, Podano nieprawidlowa wartosc progu\n\n");
	  return B_BRAKWARTOSCI;}     /* blad: niepoprawna wartosc progu */
     } else {
    fprintf(stderr,"\nBlad,Nie podano wartosci progu\n\n");
	  return B_BRAKWARTOSCI;}             /* blad: brak wartosci progu */
      break;
      }
      default: { /*blad niepoprawna opcja*/
        fprintf(stderr,"\nBlad! Niepoprawna opcja!\n\n");
        return B_NIEPOPRAWNAOPCJA;}
    }
    break;
  }              
    case 'r' : {
      switch(argv[i][2]) { /*mamy wykonac rozmywanie_poziome*/
        case 'x' : {
            wybor->rozmywanie_poziome=1;
            break;
        } 
        case 'y' : {         /*mamy wykonac rozmywanie_pinowe*/
            wybor->rozmywanie_pionowe=1;
            break;
        }
        default: {         /*blad niepoprawna opcja*/
          fprintf(stderr,"\nBlad! Niepoprawna opcja!\n\n");
          return B_NIEPOPRAWNAOPCJA;}
    }
    break;
  }
    default: { 
      fprintf(stderr,"\nBlad! Niepoprawna opcja!\n\n");      /* nierozpoznana opcja */
      return B_NIEPOPRAWNAOPCJA;}
    } /*koniec switch */
  } /* koniec for */

  if (wybor->plik_we!=NULL)     /* ok: wej. strumien danych zainicjowany */
    return W_OK;
  else {
    fprintf(stderr,"\nBlad, Nie otwarto pliku wejsciowego\n\n");
    return B_BRAKPLIKU;}         /* blad:  nie otwarto pliku wejsciowego  */
}
/********************************************************************************************************
 * Funkcja wykonaj_opcje wykonuje opcje na podstawie parametrow wywolania programu                      *
 * \param[in] opcje wskaznik na strukture opcje zawierajaca wszystkie wybrane opcje i parametry         *
 * \param[in] obraz wskaznik na strukture obraz zawierajaca dane o wczytanym obrazie                    *
 * \param[in] argc liczba argumentów wywołania wraz z nazwą programu                                    *
 * \param[in] argv tablica argumentów wywołania programu                                                *
 * \return W_OK -> zwraca 0 ->brak bledow                                                               *
 * W przypadku wystąpienia błędów kończymy natychmiastowo działanie programu (exit(-1))                 *
 * Funkcja wczytaj wykonuje się zawsze pierwsza, operacje przetarzaj_kolor oraz konwersja_do szarosci   *
 * gdy są wywołane, wykonują się na początku.                                                           *
 * FUnkcje przetwarzania obrazu wykonywują się zgodnie z kolejnością wywołania programu, możliwe jest   *
 * wielokrotne wywołanie jednej i tej samej opcji                                                       *          
 * FUnkcja zapisz wykonuje się zawsze na końcu, wyświetlanie również o ile zostanie wywołane na początku*
*********************************************************************************************************/

int wykonaj_opcje(t_opcje *opcje, t_obraz *obraz, int argc, char **argv) {
  /*Najpierw nastepuje konieczne wczytanie pliku*/
  if(wczytaj(opcje->plik_we,obraz)!=0) exit(-1);
  /*W zaleznosci od wybranej opcji wykona sie dana operacja*/
  /*Do kazdej funkcji dostarczany jest parametr wskaznik na strukture obrazu*/
  /*oraz parametry potrzebne do przetworzenia w okreslonych funkcjach*/
  if(opcje->przetwarzaj_kolor==1) if(przetwarzaj_kolor(opcje,obraz,opcje->kolor,argc,argv)!=0) exit(-1);
  if(opcje->konwersja_do_szarosci==1) if(konwersja_do_szarosci(obraz)!=0) exit(-1);
  if(opcje->przetwarzaj_kolor!=1){ /*na wypadek dwukrotnego powtorzenia tej samej operacji*/
  for(int i=0; i<argc; i++){ /*W tablicy argv zapisane są opcje w kolejności wywołania*/
      switch(argv[i][1]){ /*W zależności aktualnej opcji argv[i][1] wywołujemy określoną funkcje*/
          case 'n':{
               if(negatyw(obraz)!=0) exit(-1);
               break;}
          case 'p':{
              switch(argv[i][2]){
                  case '\0':{
                      if(progowanie(obraz,opcje->w_progu)!=0) exit(-1);
                      break;}
                  case 'c':{
                      if(progowanie_bieli(obraz,opcje->w_progu_czerni)!=0) exit(-1); 
                      break;}
                  case 'b':{
                      if(progowanie_czerni(obraz,opcje->w_progu_bieli)!=0) exit(-1);
                      break;}}
              break;}
          case 'g':{
              if(korekcja_gamma(obraz,opcje->w_gamma)!=0) exit(-1);
              break;}
          case 'z':{
              if(zmiana_poziomow(obraz,opcje->czern,opcje->biel)!=0) exit(-1);
              break;}
          case 'k':{
              if(konturowanie(obraz)!=0) exit(-1);
              break;}
          case 'r':{
              switch(argv[i][2]){
                  case 'x':{
                      if(rozmywanie_poziome(obraz)!=0) exit(-1);
                      break;}
                  case 'y':{
                      if(rozmywanie_pionowe(obraz)!=0) exit(-1);
                      break;} }
              break; }
          case 'h':{
              if(roz_histogramu(obraz)!=0) exit(-1);
              break;}/*pomijamy zapis, wczytywanie i ewentualnie wyświetlanie*/
          default: break;}}} 
  zapisz(opcje->plik_wy, obraz); /*Zapisanie zmian w pliku wyjsciowym*/
  if(opcje->wyswietlenie==1){ /*Nie wyswietlimy obraz ze standardowego wyjscia*/
      if(opcje->plik_wy==stdout) printf("\n\nNie mozna wyswietlic obrazu ze standardowego wyjscia!!\n\n");
      else wyswietlanie(opcje->nazwa_pliku);} /*Na samym koncu wyswietlamy dokonane zmiany*/
  return W_OK; /*Udalo sie wykonac wszytsko bez bledow? ->  zwroc 0*/
}
/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */
void wyswietlanie(char *n_pliku){
    char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

    strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
    strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
    strcat(polecenie," &");
    printf("%s\n",polecenie);     /* wydruk kontrolny polecenia */
    system(polecenie);           /* wykonanie polecenia        */
}
/************************************************************************************
 * Funkcja wczytaj wczytuje PGM lub PPM z pliku do struktury typu t_obraz obraz     *
 * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM lub PPM	            *
 * \param[out] obraz wskaznik na strukture obraz do której zostanie zapisany obraz  *
 * \return W_OK -> zwraca 0 ->brak bledow                                           *
************************************************************************************/

int wczytaj(FILE *plik_we, t_obraz *obraz) {
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */

    /* Sprawdzenie "numeru magicznego" - powinien być P2 lub P3 */
  if (fgets(buf,DL_LINII,plik_we)==NULL)  /* Wczytanie pierwszej linii pliku do bufora */
        koniec=1;                          /* Nie udalo sie? Koniec danych! */

  
  if ( (buf[0]!='P') || koniec) {     /*Brak litery "P"-> zły format pliku*/
    fprintf(stderr,"\n\t\tBlad: To nie jest plik PGM ani PPM!\n\n");
    return B_NIEPOPRAWNY_PLIK;
  }

  if (buf[1]!='2') {   /* Czy jest magiczne "P2" lub "P3? */
      if (buf[1]!='3') {
             fprintf(stderr,"\n\t\tBlad: To nie jest plik PGM ani PPM!\n\n");
             return B_NIEPOPRAWNY_PLIK;
      }
  } 
  /*Mamy "P2" ?->wczytujemy obraz PGM*/
  if(buf[1]=='2'){
      /* Pominiecie komentarzy */
      do {
          if ((znak=fgetc(plik_we))=='#') {     /* Czy linia rozpoczyna sie od znaku '#'? */
              if (fgets(buf,DL_LINII,plik_we)==NULL)     /* Przeczytaj ja do bufora*/
              koniec=1;                              /* Zapamietaj ewentualny koniec danych */
          }
          else{                                     /* Gdy przeczytany znak z poczatku linii */
              ungetc(znak,plik_we);                   /* nie jest '#' zwroc go*/
          }                                          /* Powtarzaj dopoki sa linie komentarza */
      }while (znak=='#' && !koniec);                /* i nie nastapil koniec danych         */
       /* Pobranie wymiarow obrazu i liczby odcieni szarosci */ 
      if(fscanf(plik_we,"%d %d %d", &(obraz->wym_y), &(obraz->wym_x), &(obraz->odcieni))!=3){
      fprintf(stderr,"\n\t\tBlad: Brak wymiarow obrazu lub liczby stopni szarosci\n\n");
      return B_NIEPOPRAWNY_PLIK;
      }
  /*Alokowanie pamięci do przechowywania pikseli obrazu w dynamicznej tablicy dwuwymiarowej*/
  obraz->piksele = malloc(obraz->wym_x*obraz->wym_y*sizeof(int));
  if(obraz->piksele==NULL){        /*Nie udało się przydzielić pamięci? -> zwroc bład*/
    free(obraz->piksele);
    fprintf(stderr,"\nBlad przydzialu pamieci\n\n");
    return B_BRAK_PAMIECI;
  } 
  int (*piksele)[obraz->wym_x];   /*inicjacia zmiennej pomocniczej (wskaznik typu int(*)[wym_x]*/
  piksele=(int(*)[obraz->wym_x]) obraz->piksele;    /*przypisanie wskaznikowi piksele to*/
                                                    /*na co pokazuje wskaznik piksele w strukturze t_obraz*/
  /* Pobranie obrazu i zapisanie w strukturze t_obraz obraz*/                                                              
  for(int i=0; i<obraz->wym_y; i++) {
    for(int j=0; j<obraz->wym_x; j++) {
        if(fscanf(plik_we,"%d", &(piksele[i][j]))!=1){   /*blad zle wymiary? ->zakomunikuj o bledzie i zakoncz*/
            fprintf(stderr,"\n\t\tBlad: Niewlasciwe wymiary obrazu\n\n");
	          return B_NIEPOPRAWNY_PLIK;
        }
    }
  } /*Zapisanie format=2 -> wczytano obraz PGM*/
  obraz->format=2;
}
/*Mamy "P3" ?->wczytujemy obraz PPM*/
else {
    do { /* Pominiecie komentarzy */
        if ((znak=fgetc(plik_we))=='#') {        /* Czy linia rozpoczyna sie od znaku '#'? */
            if (fgets(buf,DL_LINII,plik_we)==NULL)    /* Przeczytaj ja do bufora*/
            koniec=1;                                /* Zapamietaj ewentualny koniec danych */
        }
        else{                                    /* Gdy przeczytany znak z poczatku linii */
            ungetc(znak,plik_we);                    /* nie jest '#' zwroc go*/
        }                                            /* Powtarzaj dopoki sa linie komentarza */
    }while (znak=='#' && !koniec);                    /* i nie nastapil koniec danych         */
    /* Pobranie wymiarow obrazu i liczby składowych kolorowych obrazu*/ 
    if(fscanf(plik_we,"%d %d %d", &(obraz->wym_y), &(obraz->wym_x), &(obraz->odcieni))!=3){
    fprintf(stderr,"\n\t\tBlad: Brak wymiarow obrazu lub maksymalnej liczby składowych kolorowych obrazu\n\n");
    return B_NIEPOPRAWNY_PLIK;
    }
    /*Alokowanie pamięci do przechowywania pikseli obrazu w dynamicznej tablicy dwuwymiarowej*/
    obraz->piksele = malloc(obraz->wym_x*3*obraz->wym_y*sizeof(int));
    if(obraz->piksele==NULL){              /*Nie udało się przydzielić pamięci? -> zwroc bład*/
      fprintf(stderr,"\nBlad, Brak przydzialu pamieci\n\n");
      return B_BRAK_PAMIECI;
    }
    int (*piksele)[obraz->wym_x*3];  /*inicjacia zmiennej pomocniczej (wskaznik typu int(*)[wym_x*3]*/
    piksele=(int(*)[obraz->wym_x*3]) obraz->piksele;    /*przypisanie wskaznikowi piksele to*/
                                                        /*na co pokazuje wskaznik piksele w strukturze t_obraz*/
    /* Pobranie obrazu i zapisanie w strukturze t_obraz obraz*/                                                          
    for(int i=0; i<obraz->wym_y; i++) {
        for(int j=0; j<obraz->wym_x*3; j++) {
            if(fscanf(plik_we,"%d", &(piksele[i][j]))!=1){   /*blad zle wymiary? ->zakomunikuj o bledzie i zakoncz*/
                fprintf(stderr,"\n\t\tBlad: Niewlasciwe wymiary obrazu\n\n");
	              return B_NIEPOPRAWNY_PLIK;
            }
        }
    }
    /*Zapisanie format=3 -> wczytano obraz PPM*/
    obraz->format=3;
}   /*wszystko ok? -> brak bledow zwroc 0*/
    return W_OK;
}
/*********************************************************************************************
 * * Funkcja zapisz zapisuje obraz PGM lub PPM do pliku     	    									         *
 * \param[in] plik_wy uchwyt do pliku z obrazem w formacie PGM lub PPM			                 *
 * \param[in] obraz wskaznik na strukture obraz zawierajaca dane o obrazie ktory mamy zapisac*
 * \return W_OK -> zwraca 0 ->brak bledow                                                    *
 *********************************************************************************************/

int zapisz(FILE *plik_wy, t_obraz *obraz){
    if(obraz->format==2){ /*format=2 -> zapisujemy obraz PGM*/
        fprintf(plik_wy,"P2\n");  /*Magiczne "P2"*/  
        fprintf(plik_wy,"%d %d\n%d\n",obraz->wym_y,obraz->wym_x,obraz->odcieni); /*zapisanie wysokosci*/ 
                                                                                 /*szerokosci, odcieni szarosci*/
        /*pomocniczy wskaznik na tablice dynamiczna typu int(*)[obraz->wym_x]*/
        int (*piksele)[obraz->wym_x]=(int(*)[obraz->wym_x]) obraz->piksele;  
        /*zapisuje kazdy piksel oddzielony spacja w pliku*/
        for(int i=0; i<obraz->wym_y; i++){
            for(int j=0; j<obraz->wym_x; j++){
                fprintf(plik_wy,"%d ",piksele[i][j]);
            }
        }
    } /*format=3 -> zapisujemy obraz PPM*/
    if(obraz->format==3){
        fprintf(plik_wy,"P3\n");   /*Magiczne "P3"*/
        fprintf(plik_wy,"%d %d\n%d\n",obraz->wym_y,obraz->wym_x,obraz->odcieni); /*zapis wysokosci,szerokosci*/
                                                                                 /*wartosci skladowych kolorowych obrazu*/
        /*pomocniczy wskaznik na tablice dynamiczna typu int(*)[obraz->wym_x]*/
        int (*piksele)[obraz->wym_x*3]  =(int(*)[obraz->wym_x*3]) obraz->piksele;
         /*zapisuje kazdy piksel oddzielony spacja w pliku*/
        for(int i=0; i<obraz->wym_y; i++){
            for(int j=0; j<obraz->wym_x*3; j++){
                fprintf(plik_wy,"%d ",piksele[i][j]);
            }
        }
    } /*Zwalnianie zaalokowanej pamieci tuż przed zakończeniem programu(funckja zapisz wykonuje się przedostatnia)*/
        /*po zapisaniu pikseli w pliku wyjsciowym, nalezy zwolnic pamiec*/
        free(obraz->piksele);
        obraz->piksele=NULL;
    /*wszystko ok? -> brak bledow zwroc 0*/
    return W_OK;     
}
/*********************************************************************************************
 * Funkcja negatyw przetwarza obraz PGM lub wczesniej skonwetowany obraz PPM na obraz PGM    *
 * 		 badz przetwarza tylko jeden kolor                                                     *  								    
 * \param[out] obraz wskaznik na strukture obraz do ktorej zostana zapisane zmiany w obrazie *
 * \return W_OK -> brak bledow ->wartosc 0                                                   *
 ********************************************************************************************/

int negatyw(t_obraz *obraz) {
     if(obraz->format!=2){/*nie mozna przetworzyc calego obrazu PPM, nalezy najpierw wybraz kolor do przetworzenia badz go skonwertowac!!*/
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
    /*pomocniczy wskaznik na tablice dynamiczna typu int(*)[obraz->wym_x]*/
    int (*piksele)[obraz->wym_x]=(int(*)[obraz->wym_x]) obraz->piksele;
    /*operacja negatywu na każdym z pikseli*/
    for(int i=0; i<obraz->wym_y; i++){           
        for(int j=0; j<obraz->wym_x; j++){
            piksele[i][j]=obraz->odcieni-piksele[i][j];
        }
    }/*wszystko ok? -> brak bledow zwroc 0*/
    return W_OK;
}
/**********************************************************************************************
 * Funkcja progowanie przetwarza obraz PGM  lub wczesniej skonwertowany obraz PPM na obraz PGM*	
 * 	badz przetwarza tylko jeden kolor                                                         *  					    
* \param[in] w_progu wartosc podanego progu w momencie wywolania programu                     *
 * \param[out] obraz wskaznik na strukture obraz do ktorej zostana zapisane zmiany w obrazie  *
 * \return W_OK -> brak bledow zwroc 0                                                        *
 **********************************************************************************************/

int progowanie(t_obraz *obraz, int w_progu) {
   if(obraz->format!=2){/*nie mozna przetworzyc calego obrazu PPM, nalezy najpierw wybraz kolor do przetworzenia badz go skonwertowac!!*/
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
  /*pomocniczy wskaznik na tablice dynamiczna typu int(*)[obraz->wym_x]*/
    int (*piksele)[obraz->wym_x]=(int(*)[obraz->wym_x]) obraz->piksele;
    int prog=(w_progu*obraz->odcieni)/100;   /*przygotowanie progu do obliczen*/
    /*operacja progowanie na każdym z pikseli*/ 
    for(int i=0; i<obraz->wym_y; i++){         
        for(int j=0; j<obraz->wym_x; j++){     
            if(piksele[i][j]<=prog){  
                piksele[i][j]=0;                                         
            }
            else piksele[i][j]=obraz->odcieni;
        }
    }/*wszystko ok? -> brak bledow zwroc 0*/
    return W_OK; 
}      
/**************************************************************************************************************
 * Funkcja progowanie_bieli przetwarza obraz PGM  lub wczesniej skonwertowany obraz PPM na obraz PGM          *
 *  badz przetwarza tylko jeden kolor                                                                         *  
* \param[in] w_progu_bieli wartosc podanego progu dla funckji progowanie bieli  w momencie wywolania programu *
 * \param[out] obraz wskaznik na strukture obraz do ktorej zostana zapisane zmiany w obrazie                  *
 * \return W_OK -> brak bledow zwroc 0                                                                        *
 **************************************************************************************************************/

int progowanie_bieli(t_obraz *obraz, int w_progu_bieli) {
   if(obraz->format!=2){/*nie mozna przetworzyc calego obrazu PPM, nalezy najpierw wybraz kolor do przetworzenia badz go skonwertowac!!*/
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
  /*pomocniczy wskaznik na tablice dynamiczna typu int(*)[obraz->wym_x]*/
   int (*piksele)[obraz->wym_x]=(int(*)[obraz->wym_x]) obraz->piksele;
   int prog=(w_progu_bieli*obraz->odcieni)/100; /*przygotowanie prog bieli do obliczen*/
   /*Operacja progowania_bieli na kazdym pikselu*/
   for(int i=0; i<obraz->wym_y; i++){         
        for(int j=0; j<obraz->wym_x; j++){     
            if(piksele[i][j]>prog)
                piksele[i][j]=obraz->odcieni;                                         
        }
    }/*wszystko ok? -> brak bledow zwroc 0*/
    return W_OK; 
} 
/****************************************************************************************************************
 * Funkcja progowanie_czerni przetwarza obraz PGM  lub wczesniej skonwertowany obraz PPM na obraz PGM           *
 *  badz przetwarza tylko jeden kolor                                                                           *  
* \param[in] w_progu_czerni wartosc podanego progu dla funckji progowanie czerni  w momencie wywolania programu *
 *\param[out] obraz wskaznik na strukture obraz do ktorej zostana zapisane zmiany w obrazie                     *
 *\return W_OK -> brak bledow zwroc 0                                                                           *
 ****************************************************************************************************************/

int progowanie_czerni(t_obraz *obraz, int w_progu_czerni) {
    if(obraz->format!=2){/*nie mozna przetworzyc calego obrazu PPM, nalezy najpierw wybraz kolor do przetworzenia badz go skonwertowac!!*/
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
   /*pomocniczy wskaznik na tablice dynamiczna typu int(*)[obraz->wym_x]*/
   int (*piksele)[obraz->wym_x]=(int(*)[obraz->wym_x]) obraz->piksele;
   int prog=(w_progu_czerni*obraz->odcieni)/100; /*Przygotowanie progu czerni do obliczen*/
   for(int i=0; i<obraz->wym_y; i++){         
        for(int j=0; j<obraz->wym_x; j++){     
            if(piksele[i][j]<=prog)
                piksele[i][j]=0;                                         
        }
    }/*wszystko ok? -> brak bledow zwroc 0*/
    return W_OK; 
} 
/*************************************************************************************************
 * Funkcja korekcja_gamma przetwarza obraz PGM lub wczesniej skonwertowany obraz PPM na obraz PGM*
 *  badz przetwarza tylko jeden kolor                                                            *  
 *\param[in] w_gamma	wspolczynnik gamma podany w momencie wywolania programu                    *
 *\param[out] obraz wskaznik na strukture obraz  do ktorej zostana zapisane zmiany w obrazie     *
 * \return W_OK ->brak bledow zwroc 0                                                            *  
 *************************************************************************************************/

int korekcja_gamma(t_obraz *obraz, float w_gamma){
     if(obraz->format!=2){/*nie mozna przetworzyc calego obrazu PPM, nalezy najpierw wybraz kolor do przetworzenia badz go skonwertowac!!*/
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
   }
     /*pomocniczy wskaznik na tablice dynamiczna typu int(*)[obraz->wym_x]*/
    int (*piksele)[obraz->wym_x]=(int(*)[obraz->wym_x]) obraz->piksele;
    if(w_gamma==0){ /*wspolczynnik gamma nie moze sie zerowac -> sprawdz*/
        fprintf(stderr,"\nBlad! WSpolczynnik gamma nie moze sie zerowac! Operacja sie nie powiedzie!\n\n");
        return B_BLEDNE_DANE;
    }
    float w=1/ w_gamma;   /*zmienna pomocnicza wykladnik potegi */ /*do ulatwienia obliczen*/
    float w2=(1-w_gamma)/w_gamma; /*zmienna pomocnicza wykladnik drugiej potegi*/
    /*operacja korekcji_gamma na kazdym pikselu*/
    for(int i=0; i<obraz->wym_y; i++){      
        for(int j=0; j<obraz->wym_x; j++){
            piksele[i][j]=(pow(piksele[i][j],w))/(pow(obraz->odcieni,w2));
        }
    }/*wszystko ok? -> brak bledow zwroc 0*/
    return W_OK;
}
/****************************************************************************************************
 * Funkcja zmiana poziomow przetwarza obraz PGM lub wczesniej skonwertowany obraz PPM na obraz PGM  *
 *  badz przetwarza tylko jeden kolor                                                               *  
 *\param[in] bialy	wartosc bieli podany w momencie wywolania programu                              *
 *\param[in] czarny wartosc czerni podany w momencie wywolania programu                             *
 *\param[out] obraz wskaznik na strukture obraz  do ktorej zostana zapisane zmiany w obrazie        *
 * \return W_OK ->brak bledow zwroc 0                                                               *  
 ****************************************************************************************************/

int zmiana_poziomow(t_obraz *obraz, int czarny, int bialy){
   if(obraz->format!=2){/*nie mozna przetworzyc calego obrazu PPM, nalezy najpierw wybraz kolor do przetworzenia badz go skonwertowac!!*/
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
  /*pomocniczy wskaznik na tablice dynamiczna typu int(*)[obraz->wym_x]*/
  int (*piksele)[obraz->wym_x]=(int(*)[obraz->wym_x]) obraz->piksele;
  int biel=(bialy*obraz->odcieni)/100; /*Przygotwanie bieli i czerni do obliczen*/
  int czern=(czarny*obraz->odcieni)/100;
  if(biel==czern){  /*Biel nie moze sie rownac czerni (biel=czern -> sprawdz)*/
      fprintf(stderr,"\nBlad!! czern nie moze byc rowna bieli!, Przetwarzanie się nie wykona.\n\n");
      return B_BLEDNE_DANE;
  }
  /*Operacja zmiany_poziomow na każdym pikselu*/
  for(int i=0; i<obraz->wym_y; i++){         
        for(int j=0; j<obraz->wym_x; j++){  
          if(piksele[i][j]<=czern)
              piksele[i][j]=0;
          else if(piksele[i][j]>=biel)
              piksele[i][j]=obraz->odcieni;
          else piksele[i][j]=(piksele[i][j]-czern)*obraz->odcieni/(biel-czern);   
        }
    }/*wszystko ok? -> brak bledow zwroc 0*/
  return W_OK;
}
/**********************************************************************************************
 * Funkcja konturowanie przetwarza obraz PGM lub wczesniej skonwertowany plik PPM na obraz PGM*
 *  badz przetwarza tylko jeden kolor                                                         *  
 *\param[out] obraz wskaznik na strukture obraz  do ktorej zostana zapisane zmiany w obrazie  *
 * \return W_OK brak bledow zworc 0                                                           *
 **********************************************************************************************/

int konturowanie(t_obraz *obraz) {
     if(obraz->format!=2){/*nie mozna przetworzyc calego obrazu PPM, nalezy najpierw wybraz kolor do przetworzenia badz go skonwertowac!!*/
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  } /*pomocniczy wskaznik na tablice dynamiczna typu int(*)[obraz->wym_x]*/
    int (*piksele)[obraz->wym_x]=(int(*)[obraz->wym_x]) obraz->piksele;
    /*Operacja konturowania na każdym pikselu*/
   for(int i=0; i<obraz->wym_y; i++){ /*Wazne jest sprawdzenie przypadkow szczegolnych*/ 
       for(int j=0; j<obraz->wym_x; j++){  /*aby nie wyjsc poza zakres tablicy*/
            if((i==obraz->wym_y-1)&&(j==obraz->wym_x-1)) piksele[i][j]=piksele[i][j];
            else if(i==obraz->wym_y-1) piksele[i][j]=abs(piksele[i][j+1]-piksele[i][j]);
            else if(j==obraz->wym_x-1) piksele[i][j]=abs(piksele[i+1][j]-piksele[i][j]);
            else piksele[i][j]=abs(piksele[i+1][j]-piksele[i][j])+abs(piksele[i][j+1]-piksele[i][j]);
        }
          
    }
    /*wszystko ok? -> brak bledow zwroc 0*/
    return W_OK;
}
/*****************************************************************************************************************
 * Funkcja rozmywanie_poziome o promienu 1 przetwarza obraz PGM lub wczesniej skonwertowany plik PPM na obraz PGM*
 *  badz przetwarza tylko jeden kolor                                                                            *  
 *\param[out] obraz wskaznik na strukture obraz  do ktorej zostana zapisane zmiany w obrazie                     *
 * \return W_OK brak bledow zworc 0                                                                              *
 ****************************************************************************************************************/

int rozmywanie_poziome(t_obraz *obraz){

   if(obraz->format!=2){/*nie mozna przetworzyc calego obrazu PPM, nalezy najpierw wybraz kolor do przetworzenia badz go skonwertowac!!*/
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
  /*pomocniczy wskaznik na tablice dynamiczna typu int(*)[obraz->wym_x]*/
  int (*piksele)[obraz->wym_x]=(int(*)[obraz->wym_x]) obraz->piksele;
  /*kopia pierwotnej tablicy wymagana do obliczen pikseli*/
  /*(po przetworzeniu piksela w nastepnym obok pikselu w obliczeniach wymagany jest piksel poprzedni pierwotny)*/
  int (*pomocna_tablica)[obraz->wym_x]=(int (*)[obraz->wym_x]) malloc(obraz->wym_x*obraz->wym_y*sizeof(int));
  if(pomocna_tablica==NULL){ /*nie udalo sie przydzielic pamiec? -> zwroc blad*/
      free(pomocna_tablica);
      fprintf(stderr,"\nBlad, Brak przydzialu pamieci\n\n");
      return B_BRAK_PAMIECI;
  }
  for(int i=0; i<obraz->wym_y; i++){ 
      for(int j=0; j<obraz->wym_x; j++){ 
          pomocna_tablica[i][j]=piksele[i][j];
      }
  }
  /*Operacja rozmywanie_poziome na kazdym pikselu*/
  for(int i=0; i<obraz->wym_y; i++){ /*Wazne jest sprawdzenie przypadkow szczegolnych*/ 
      for(int j=0; j<obraz->wym_x; j++){ /*aby nie wyjsc poza zakres tablicy*/
          if(j==0) piksele[i][j]=(piksele[i][j]+piksele[i][j+1])/2;
          else if(j==obraz->wym_x-1) piksele[i][j]=(piksele[i][j]+pomocna_tablica[i][j-1])/2;
          else piksele[i][j]=(pomocna_tablica[i][j-1]+piksele[i][j]+piksele[i][j+1])/3;
      }
  }/*zwalnianie dodatkowej pamieci na potrzeby funckji*/
  free(pomocna_tablica);
  pomocna_tablica=NULL;
  /*wszystko ok? -> brak bledow zwroc 0*/
  return W_OK;
}
/*****************************************************************************************************************
 * Funkcja rozmywanie_pionowe o promienu 1 przetwarza obraz PGM lub wczesniej skonwertowany plik PPM na obraz PGM*
 *  badz przetwarza tylko jeden kolor                                                                            *  
 *\param[out] obraz wskaznik na strukture obraz do ktorej zostana zapisane zmiany w obrazie                      *
 * \return W_OK brak bledow zworc 0                                                                              *
 ****************************************************************************************************************/

int rozmywanie_pionowe(t_obraz *obraz){
   if(obraz->format!=2){/*nie mozna przetworzyc calego obrazu PPM, nalezy najpierw wybraz kolor do przetworzenia badz go skonwertowac!!*/
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
  /*pomocniczy wskaznik na tablice dynamiczna typu int(*)[obraz->wym_x]*/
  int (*piksele)[obraz->wym_x]=(int(*)[obraz->wym_x]) obraz->piksele;
  /*kopia pierwotnej tablicy wymagana do obliczen pikseli*/
  /*(po przetworzeniu piksela w nastepnym obok pikselu w obliczeniach wymagany jest piksel poprzedni pierwotny)*/
  int (*pomocna_tablica)[obraz->wym_x]=(int (*)[obraz->wym_x]) malloc(obraz->wym_x*obraz->wym_y*sizeof(int));
  if(pomocna_tablica==NULL){ /*nie udalo sie przydzielic pamiec? -> zwroc blad*/
      free(pomocna_tablica);
      fprintf(stderr,"\nBlad, Brak przydzialu pamieci\n\n");
      return B_BRAK_PAMIECI;
  }
  for(int i=0; i<obraz->wym_y; i++){ 
      for(int j=0; j<obraz->wym_x; j++){ 
          pomocna_tablica[i][j]=piksele[i][j];
      }
  }
  /*Operacja rozmywanie_pionowe na kazdym pikselu*/
  for(int i=0; i<obraz->wym_y; i++){ /*Wazne jest sprawdzenie przypadkow szczegolnych*/ 
      for(int j=0; j<obraz->wym_x; j++){ /*aby nie wyjsc poza zakres tablicy*/
          if(i==0) piksele[i][j]=(piksele[i][j]+piksele[i+1][j])/2;
          else if (i==obraz->wym_y-1) piksele[i][j]=(piksele[i][j]+pomocna_tablica[i-1][j])/2;
          else piksele[i][j]=(piksele[i+1][j]+piksele[i][j]+pomocna_tablica[i-1][j])/3;
      }
  } /*zwalnianie dodatkowej pamieci na potrzeby funckji*/
  free(pomocna_tablica);
  pomocna_tablica=NULL;
  /*wszystko ok? -> brak bledow zwroc 0*/
  return W_OK;
}
/*******************************************************************************************
 * Funkcja roz_histogramu przetwarza obraz PGM lub wczesniej skonwertowany obraz PPM na PGM* 
 * badz przetwarza tylko jeden kolor                                                       *     	       	       	       	    
*\param[out] obraz wskaznik na strukture obraz  do ktorej zostana zapisane zmiany w obrazie*							  
 * \return W_OK brak bledow zwroc 0                                                        *
*******************************************************************************************/

int roz_histogramu(t_obraz *obraz){
    if(obraz->format!=2){/*nie mozna przetworzyc calego obrazu PPM, nalezy najpierw wybraz kolor do przetworzenia badz go skonwertowac!!*/
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
    /*pomocniczy wskaznik na tablice dynamiczna typu int(*)[obraz->wym_x]*/
    int (*piksele)[obraz->wym_x]=(int(*)[obraz->wym_x]) obraz->piksele;
    int min=obraz->odcieni; /*pomocnicze parametry min and max*/ 
    int max=0;          /*do wyszukania najmniejszj wartosci odcieni szarosci*/ 
    /*Operacja wyszukiwania min and max w calej tablicy pikseli*/
    for(int i=0; i<obraz->wym_y; i++){     
        for(int j=0; j<obraz->wym_x; j++){
            if(piksele[i][j]>max)
                max=piksele[i][j];
            if(piksele[i][j]<min)
                min=piksele[i][j];
        }
    } /*max==min -> obraz sie nie zieni , wypisywanie komunikatu*/
    if((max==obraz->odcieni)&&(min==0)) printf("\nPiksele pokrywaja caly zakres jasnosci obrazu!,Obraz sie nie zmienil!\n\n");
    if(min!=max){ /*Operacja roz_histogramu wykona sie tylko gdy min!=max*/
        for(int i=0; i<obraz->wym_y; i++){   
            for(int j=0; j<obraz->wym_x; j++){
                piksele[i][j]=((piksele[i][j]-min)*obraz->odcieni)/(max-min);
            }
        }
    } /*min==max? -> zwroc blad*/
    else{
        printf("\nBlad: Najmniejsza jasnosc = maksymalnej jasnosci! Operacja sie nie powiedzie!\n\n");
        return B_BLEDNE_DANE;
    }/*wszystko ok? -> brak bledow zwroc 0*/
    return W_OK;
}
/*******************************************************************************************
 * Funkcja konwersja_do_szarosci zamienia obraz PPM NA PGM                                 *  	       	       	       	    
*\param[out] obraz wskaznik na strukture obraz do ktorej zostana zapisane zmiany w obrazie *							  
 *\return W_OK brak bledow zwroc 0                                                         *
*******************************************************************************************/

int konwersja_do_szarosci(t_obraz *obraz){
  /*Jezeli obraz jest formatu PGM -> nie mozna skonwertowac obrazu*/
  if(obraz->format!=3){ /*Mamy obraz PGM? ->zwroc blad*/
      fprintf(stderr,"\nBlad Nie wczytano kolorowego obrazu, Nie mozna wykonac operacji konwersji do szarosci!\n\n");
      return B_BLEDNE_DANE;
  }
  /*pomocniczy wskaznik na tablice dynamiczna typu int(*)[obraz->wym_x*3]*/
  int (*piksele)[obraz->wym_x*3]=(int(*)[obraz->wym_x*3]) obraz->piksele;
  int k=0; /*Zmienne pomocnicze do nadpisu tablicy*/
  int w=0;
  /*Operacja konwersji pikseli*/
  /*Nowy piksel formatu PGM jest srednia arytmetyczna 3 skladowych piksela formatu PPM*/
  for(int i=0; i<obraz->wym_y; i++){
      for(int j=0; j<obraz->wym_x*3; j+=3){
          piksele[k][w]=(piksele[i][j]+piksele[i][j+1]+piksele[i][j+2])/3;
          w++;
          if(w==obraz->wym_x*3){
            k++;
            w=0;
          }
      }
  }/*Po dokonaniu zmiany pliku z PPM na PGM format=2 -> plik PGM*/
  obraz->format=2;
  /*zwalnianie pamieci nadmiarowej-> funkcja realloc znajduje nowy obszar pamieci, nastepnie kolejno kopiuje dane oryginalne*/
  /*w nowe miejsce, zwalnia stary obszar pamieci i zwraca wskaznik do nowego obszaru lub NULL*/
  obraz->piksele=realloc(obraz->piksele,obraz->wym_x*obraz->wym_y*sizeof(int));
  if(obraz->piksele==NULL){        /*Nie udało się przydzielić pamięci? -> zwroc bład*/
    free(obraz->piksele);
    fprintf(stderr,"\nBlad przydzialu pamieci\n\n");
    return B_BRAK_PAMIECI;
  } 
  /*wszystko ok? -> brak bledow zwroc 0*/
  return W_OK;
}
/*************************************************************************************************************************
* Funkcja przetwarzaj_opcje wykonuje operacje przetwarzania na wybranym kolorze z 3 mozliwych                            *
* czerwony, zielony oraz niebieski                                                                                       *
* Należy podać dodatkowo przynajmniej jedną operację przetwarzającą kolor w momencie wywołania programu                  *	       	       	       	    
*\param[out] obraz wskaznik na strukture obraz do ktorej zostana zapisane zmiany w obrazie                               *	
*\param[in] opcje wskaznik na strukture opcje, ktora przechowuje informacje o opcjach wywolania programu                 *
*oraz argumentay do tych opcji                                                                                           *
*\param[in] kolor zmienna przechowuje znak 'r' lub 'g' lub 'b' dzieki ktoremu wiadomo jaki kolor ma zostac przetworzony  *
*\param[in] argc przechowuje liczbę argumentów wywołania programu                                                        *
*\param[in] argv tablica z opcjami wywołania funkcji 				                                                             *		  
 *\return W_OK brak bledow zwroc 0                                                                                       *
**************************************************************************************************************************/

int przetwarzaj_kolor(t_opcje *opcje,t_obraz *obraz, char kolor, int argc, char **argv){
  if(obraz->format!=3){  /*Obraz musi byc formatu PPM ->sprawdz*/
      fprintf(stderr,"\nBlad Nie wczytano kolorowego obrazu, Nie mozna wykonac operacji przetwarznia koloru!\n\n");
      return B_BLEDNE_DANE;}
  /*gdy nie wybrano zadnej operacji(wybrano tylko opcje przetwarzania koloru ale nie podano jakiej operacji dokonac-> zwroc blad)*/
  if((opcje->negatyw==0)&&(opcje->progowanie==0)&&(opcje->progowanie_czerni==0)&&(opcje->progowanie_bieli==0)&&(opcje->korekcja_gamma==0)&&(opcje->zmiana_poziomow==0)
  &&(opcje->konturowanie==0)&&(opcje->rozmywanie_poziome==0)&&(opcje->rozmywanie_pionowe==0)&&(opcje->roz_histogramu==0)){
  fprintf(stderr,"\nBlad! Nie podano operacji do przetwarzania wybranego koloru w obrazie PPM!\n\n");
  return B_BLEDNE_DANE; }
  /*na potrzebe tej operacji zainicjowano dodatkowa strukture "pojedynczy_kolor" typu t_obraz, ktora bedzie przechowywac 
  wysokosc, szerokosc, oraz maksymalna skladowa kolorowa obrazu oraz wskaznik na tablice dwuwymiarowa ale tylko dla skladowych piksela pojedynczego koloru*/
  /*po zakonczeniu funckji przetwarzaj_kolor -> struktura pojedynczy_kolor przetstaje istniec*/
  t_obraz pojedynczy_kolor;
  pojedynczy_kolor.wym_x=obraz->wym_x;
  pojedynczy_kolor.wym_y=obraz->wym_y;
  pojedynczy_kolor.odcieni=obraz->odcieni;
  pojedynczy_kolor.format=2;
  /*alokowanie pamieci do przechowywania tylko skladowych jedngo koloru kazdego piksela*/
  pojedynczy_kolor.piksele=malloc(obraz->wym_x*obraz->wym_y*sizeof(int));
  if(pojedynczy_kolor.piksele==NULL){ /*nie udalo sie przydzielic pamiec? -> zwroc blad*/
      free(pojedynczy_kolor.piksele);
      fprintf(stderr,"\nBlad, Brak przydzialu pamieci\n\n");
      return B_BRAK_PAMIECI;}
  /*zmienna pomocnicza piksele typu int(*)[obraz->wym_x*3]->wskaznik na tablice z pierwotnie wczytanym obrazem */
  int (*piksele)[obraz->wym_x*3]=(int(*)[obraz->wym_x*3]) obraz->piksele;
  /*zmienna pomocnicza pomocna_tablica typu int(*)[obraz->wym_x]->wskaznik na tablice ze skladowymi jedngo koloru */
  int (*pomocna_tablica)[obraz->wym_x]=(int(*)[obraz->wym_x]) pojedynczy_kolor.piksele; 
  int k=0,w=0; /*zmienne iteracyjne*/

  if(kolor=='r'){ /*wybrano kolor czerwony*/
      for(int i=0; i<obraz->wym_y; i++){ /*zapisywanie czerwonych skladowych pikseli obrazu do struktury pojedynczy.kolor*/
          for(int j=0; j<obraz->wym_x*3; j+=3){
              pomocna_tablica[k][w]=piksele[i][j];
              w++;
              if(w==obraz->wym_x){
                k++;
                w=0;}}} 
  /*wykonywanie operacji przetwarzania na tej jednej skladowej*/
     for(int i=0; i<argc; i++){ /*posługiwanie się tablicą a argumentami argv*/
      switch(argv[i][1]){       /*Pozwala to na wykonywanie operacji w zadanej kolejności*/
          case 'n':{
               negatyw(&pojedynczy_kolor);
               break;}
          case 'p':{
              switch(argv[i][2]){
                  case '\0':{
                      progowanie(&pojedynczy_kolor,opcje->w_progu);
                      break;}
                  case 'c':{
                      progowanie_bieli(&pojedynczy_kolor,opcje->w_progu_czerni);
                      break;}
                  case 'b':{
                      progowanie_czerni(&pojedynczy_kolor,opcje->w_progu_bieli);
                      break;}}
              break;}
          case 'g':{
              korekcja_gamma(&pojedynczy_kolor,opcje->w_gamma);
              break;}
          case 'z':{
              zmiana_poziomow(&pojedynczy_kolor,opcje->czern,opcje->biel);
              break;}
          case 'k':{
              konturowanie(&pojedynczy_kolor);
              break;}
          case 'r':{
              switch(argv[i][2]){
                  case 'x':{
                      rozmywanie_poziome(&pojedynczy_kolor);
                      break;}
                  case 'y':{
                      rozmywanie_pionowe(&pojedynczy_kolor);
                      break;} }
              break; }
          case 'h':{
              roz_histogramu(&pojedynczy_kolor);
              break;}
          default: break;}}
      w=0; k=0; /*na samym koncu zmiany zostaja zapisane do pierwotnej tablicy zawierajacej wszytskie skladowe pikseli*/
      for(int i=0; i<obraz->wym_y; i++){
          for(int j=0; j<obraz->wym_x*3; j+=3){
              piksele[i][j]=pomocna_tablica[k][w];
              w++;
              if(w==obraz->wym_x){
                k++;
                w=0;}}}}
  else if(kolor=='g'){  /*wybrano kolor zielony*/
      for(int i=0; i<obraz->wym_y; i++){  /*zapisywanie zielonych skladowych pikseli obrazu do struktury pojedynczy.kolor*/
          for(int j=1; j<obraz->wym_x*3; j+=3){
              pomocna_tablica[k][w]=piksele[i][j];
              w++;
              if(w==obraz->wym_x){
                k++;
                w=0;}}}
       /*wykonywanie operacji przetwarzania na tej jednej skladowej*/
      for(int i=0; i<argc; i++){ /*posługiwanie się tablicą a argumentami argv*/
          switch(argv[i][1]){ /*posługiwanie się tablicą a argumentami argv*/
          case 'n':{
               negatyw(&pojedynczy_kolor);
               break;}
          case 'p':{
              switch(argv[i][2]){
                  case '\0':{
                      progowanie(&pojedynczy_kolor,opcje->w_progu);
                      break;}
                  case 'c':{
                      progowanie_bieli(&pojedynczy_kolor,opcje->w_progu_czerni);
                      break;}
                  case 'b':{
                      progowanie_czerni(&pojedynczy_kolor,opcje->w_progu_bieli);
                      break;}}
              break;}
          case 'g':{
              korekcja_gamma(&pojedynczy_kolor,opcje->w_gamma);
              break;}
          case 'z':{
              zmiana_poziomow(&pojedynczy_kolor,opcje->czern,opcje->biel);
              break;}
          case 'k':{
              konturowanie(&pojedynczy_kolor);
              break;}
          case 'r':{
              switch(argv[i][2]){
                  case 'x':{
                      rozmywanie_poziome(&pojedynczy_kolor);
                      break;}
                  case 'y':{
                      rozmywanie_pionowe(&pojedynczy_kolor);
                      break;} }
              break; }
          case 'h':{
              roz_histogramu(&pojedynczy_kolor);
              break;}
          default: break;}}
      w=0; k=0; /*na samym koncu zmiany zostaja zapisane do pierwotnej tablicy zawierajacej wszytskie skladowe pikseli*/
      for(int i=0; i<obraz->wym_y; i++){
          for(int j=1; j<obraz->wym_x*3; j+=3){
              piksele[i][j]=pomocna_tablica[k][w];
              w++;
              if(w==obraz->wym_x){
                k++;
                w=0;}}}}
  else if(kolor=='b'){ /*wybrano kolor niebieski*/
      for(int i=0; i<obraz->wym_y; i++){   /*zapisywanie niebieskich skladowych pikseli obrazu do struktury pojedynczy.kolor*/
          for(int j=2; j<obraz->wym_x*3; j+=3){
              pomocna_tablica[k][w]=piksele[i][j];
              w++;
              if(w==obraz->wym_x){
                k++;
                w=0;}}}
      /*wykonywanie operacji przetwarzania na tej jednej skladowej*/
     for(int i=0; i<argc; i++){ /*posługiwanie się tablicą a argumentami argv*/
      switch(argv[i][1]){ /*posługiwanie się tablicą a argumentami argv*/
          case 'n':{
               negatyw(&pojedynczy_kolor);
               break;}
          case 'p':{
              switch(argv[i][2]){
                  case '\0':{
                      progowanie(&pojedynczy_kolor,opcje->w_progu);
                      break;}
                  case 'c':{
                      progowanie_bieli(&pojedynczy_kolor,opcje->w_progu_czerni);
                      break;}
                  case 'b':{
                      progowanie_czerni(&pojedynczy_kolor,opcje->w_progu_bieli);
                      break;}}
              break;}
          case 'g':{
              korekcja_gamma(&pojedynczy_kolor,opcje->w_gamma);
              break;}
          case 'z':{
              zmiana_poziomow(&pojedynczy_kolor,opcje->czern,opcje->biel);
              break;}
          case 'k':{
              konturowanie(&pojedynczy_kolor);
              break;}
          case 'r':{
              switch(argv[i][2]){
                  case 'x':{
                      rozmywanie_poziome(&pojedynczy_kolor);
                      break;}
                  case 'y':{
                      rozmywanie_pionowe(&pojedynczy_kolor);
                      break;} }
              break; }
          case 'h':{
              roz_histogramu(&pojedynczy_kolor);
              break;}
          default: break;}}
      w=0; k=0; /*na samym koncu zmiany zostaja zapisane do pierwotnej tablicy zawierajacej wszytskie skladowe pikseli*/
      for(int i=0; i<obraz->wym_y; i++){
          for(int j=2; j<obraz->wym_x*3; j+=3){
              piksele[i][j]=pomocna_tablica[k][w];
              w++;
              if(w==obraz->wym_x){
                k++;
                w=0;}}}} 
  /*zwalnianie dodatkowej pamieci na potrzeby tej funckji*/
  free(pojedynczy_kolor.piksele);
  pojedynczy_kolor.piksele=NULL;
  /*wszytsko sie udalo pomyslnie? -> zwroc wartosc 0 brak bledow*/
  return W_OK;
}