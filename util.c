#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "util.h"


/**
* � utiliser pour afficher la taille et le nom d'un �l�ment
*/
void print_element(unsigned long long taille, char * nom)
{
  printf("%-30llu\t%s\n",taille,nom); //long long unsigned int
}


void print_erreur(int err)
{
  switch (err)
  {
    case ERR_DROITS_INSUFFISANTS :
      printf("Erreur : droit insuffisant\n");
      break;
    case ERR_FICHIER_REPERTOIRE_INEXISTANT :
      printf("Erreur : fichier ou repertoire inexistant\n");
      break;
    case ERR_AUTRE :
      printf("Erreur : autre\n");
      break;
    default:
      break;
  }
}
