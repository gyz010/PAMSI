/* -*- mode: C; comment-column: 40; c-indent-comments-syntactically-p: t -*- */

/****************************************************************************/
/*                                                                          */
/* Copyright (c) 2024 Witold Paluszynski                                    */
/*                                                                          */
/* I grant everyone the right to copy this program in whole or part, and    */
/* to use it for any purpose, provided the source is properly acknowledged. */
/*                                                                          */
/* Udzielam kazdemu prawa do kopiowania tego programu w calosci lub czesci, */
/* i wykorzystania go w dowolnym celu, pod warunkiem zacytowania zrodla.    */
/*                                                                          */
/****************************************************************************/


/*
 * Program demonstruje polaczenie z serwerem na podanym porcie.
 * Ma byc elementem warstwy komunikacyjnej programu gry w warcaby.
 *
 * Compile on Solaris:
 *   cc -Xc -o client checkers_client.c -lsocket -lnsl
 * Compile on Linux:
 *   gcc -Wall -pedantic -o client checkers_client.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFSPACE 1024
#define BLACK 1
#define WHITE 0

int errno;


int main(int argc, char *argv[])
{
  char buf[BUFSPACE];
  int player, serv_sock, n, nr_ruchu;
  struct sockaddr_in serv_addr;
  struct hostent *serv_hostent;
  /* taka sobie przykladowa gra */
  char *ruchy[]={"9-14", "23-18", "14x23", "27x18", "12-16", "24-19",
		 "16x23", "26x19", "11-15", "18x11", "8x15x24", "28x19",
		 "10-14", "19-15", "14-18", "15-10", "6x15", "32-28",
		 "18-23", "30-26", "23x30", "21-17", "30x21x14", "22-17",
		 "14x21", "29-25", "21x30", "31-26", "30x23", "28-24",
		 "7-11", "24-20", "2-7", "20-16", "11x20"};

  /* argv[1] okresla czy gramy lokalnie GUI czy zdalnie NET */

  /* argv[2] okresla ktora strona ma grac program */
  player = -1;
  if (strcasecmp(argv[2], "BLACK") == 0) player = BLACK;
  if (strcasecmp(argv[2], "WHITE") == 0) player = WHITE;
  if (player < 0) {
    fprintf(stderr, "%s: niepoprawne okreslenie gracza w argv[2]: %s\n",
	    argv[0], argv[2]);
    exit(-1);
  }
  
  /* argv[3] zadaje glebokosc przeszukiwania minimaksu */

  /* argv[4] zadaje ziarno generatora liczb losowych */

  /* argv[5] zadaje nazwe serwera gry */
  /* argv[6] zadaje numer portu serwera gry */
  serv_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (serv_sock < 0) {
    perror("socket");
    exit(errno);
  }
  serv_hostent = gethostbyname(argv[5]);
  if (serv_hostent == 0) {
    fprintf(stderr, "%s: nieznany adres IP %s\n", argv[0], argv[5]);
    exit(-1);
  }
  serv_addr.sin_family = AF_INET;
  memcpy(&serv_addr.sin_addr, serv_hostent->h_addr, serv_hostent->h_length);
  serv_addr.sin_port = htons(atoi(argv[6]));

  printf("Laczymy sie z serwerem ...\n");
  if (connect(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("connect");
    exit(-1);
  }

  printf("Polaczenie nawiazane, zaczynamy gre ...\n");

  nr_ruchu = 0;				/* czarny robi ruchy parzyste */
  if (player==WHITE) nr_ruchu = 0;	/* bialy nieparzyste */
  while(1) {
    int index_ruchu = nr_ruchu*2+player-1;

    if ((nr_ruchu>0) || (player==BLACK)) {
        //Tutaj wykonuje wszystko zwiazane z ruchem
      printf("Wysylam do serwera moj ruch: %s\n", ruchy[index_ruchu]);
      //Wysylam ruch
      if (write(serv_sock, ruchy[index_ruchu], strlen(ruchy[index_ruchu])) < 0){
	perror("write");
	exit(errno);
      }
    }
    printf("Czekam na ruch przeciwnika ...\n");
    n=read(serv_sock, buf, sizeof buf);
    if (n<0) {
      perror("read");
      exit(errno);
    }
    if (n==0) { /* pusty komunikat = zamkniete polaczenie */
      printf("Broker zamknal polaczenie, hmmmm...\n");
      exit(0);
    }
    buf[n] = 0;
    printf("Otrzymalem ruch przeciwnika: %s", buf);
    if (buf[n-1]!='\n') printf("\n");

    ++nr_ruchu;
  }
}
//ruch    0 1 2 3 4 5  6
//BLACK   0 2 4 6 8 10 12
//WHITE   - 1 3 5 7 9  11
