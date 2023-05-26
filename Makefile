#Makefile
#Wywołanie komendą "make"
CFLAGS = -std=c99 -pedantic -Wall  #Inne flagi kompilatora
LIBS = -lm #DOdawanie bibliotek
#Z podanych wcześniej wygenerowanych plików całkowita kompilacja 
main: main.o modul.o
	gcc $(CFLAGS) main.o modul.o $(LIBS) -o prog
	
#Aby otrzymać plik main.o podajemy z czego: main.c modul.h oraz jak: gcc -c main.c
main.o: main.c modul.h
	gcc -c main.c 

#Aby otrzymać plik "modul.o" podajemy z czego: modul.c, modul.h oraz jak: gcc -c modul.c    
funkcje.o: modul.c modul.h
	gcc -c modul.c
# KOmenda "make clean" czyści pliki z rozszerzeniem .o oraz utworzony plik prog
clean:
	rm *.o prog

