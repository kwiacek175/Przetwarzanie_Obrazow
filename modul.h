#ifndef MODUL_H
#define MODUL_H

/* strukura do zapamietywania opcji podanych w wywolaniu programu */
typedef struct {
  FILE *plik_we, *plik_wy;     /* uchwyty do pliku wej. i wyj. */
  char *nazwa_pliku;           /*nazwa pliku wyj. do wyświetlania przetworzonego obrazu*/
  int negatyw,progowanie,konturowanie,wyswietlenie,korekcja_gamma,roz_histogramu;   /* opcje */
  int progowanie_czerni,progowanie_bieli,zmiana_poziomow, rozmywanie_poziome, rozmywanie_pionowe;/*dodatkowe opcje*/
  int przetwarzaj_kolor, konwersja_do_szarosci; /*opcje do obrazow ppm*/            
  int w_progu;            /* wartosc progu dla opcji progowanie*/ 
  float w_gamma;          /*wartosc wspołczynnika dla opcji korekcji gamma*/
  int w_progu_bieli;     /*wartosc progu dla opcji progowanie bieli*/
  int w_progu_czerni;    /*wartosc progu dla opcji progowanie czerni*/
  int biel,czern;        /*dwie wartosci dla opcji zmiany poziomów*/
  char kolor;            /*kolor do przetwarzania funckji przetwarzaj_kolor*/
  } t_opcje;             /*zmienna typu t_opcje*/

/*struktura do zapisywania danych o wczytanym obrazie pgm lub ppm*/
typedef struct {
  int format; /*przechowuje informacje o formacie wczytanego obrazu*/
  int wym_x, wym_y, odcieni;   /*szerokość obrazu,wysokość obrazu,odcieni->wartość odcieni szarosci lub składowych kolorowych obrazu*/
  void *piksele;              /*wskaznik piksele wskazuje na tablice typu "pustego"*/
} t_obraz;/*zmienna typu t_obraz*/       /*dynamiczna tablica dwuwymiarowa do przechowywania pikseli obrazu*/

/*nagłowki wszystkich funkcji wraz z parametrami w programie*/
void wyzeruj_opcje(t_opcje *);
void wyswietlanie(char *);

int przetwarzaj_opcje(int ,char **, t_opcje *);
int wykonaj_opcje(t_opcje *, t_obraz *,int ,char**);

int wczytaj(FILE *, t_obraz *);
int zapisz(FILE *, t_obraz *);

int negatyw(t_obraz *);
int konturowanie(t_obraz *);
int roz_histogramu(t_obraz *);
int rozmywanie_poziome(t_obraz *);
int rozmywanie_pionowe(t_obraz *);
int konwersja_do_szarosci(t_obraz *);
int zmiana_poziomow(t_obraz *, int , int );
int progowanie_czerni(t_obraz *, int );
int progowanie_bieli(t_obraz *, int );
int progowanie(t_obraz *, int );
int korekcja_gamma(t_obraz *, float );
int przetwarzaj_kolor(t_opcje *, t_obraz *, char, int, char**);


#endif
