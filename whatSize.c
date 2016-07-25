#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define ERR_DROITS_INSUFFISANTS 0
#define ERR_FICHIER_REPERTOIRE_INEXISTANT 1
#define ERR_AUTRE 2

typedef struct tailleEtNoms {
	int id;
	char name[200];
} taillesNoms;

void print_erreur(int err);
void print_element(unsigned long long taille, char * nom);
void trierAffichage(int n, taillesNoms *array);
static int parcoursDir(char *dirpath, int counter);


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


/*
La methode trier permet d'afficher les resultats
en ordre descendant des tailles.
Si les tailles sont les memes, alors les noms
des fichiers sont tries en ordre alphabetique.
*/
void trierAffichage(int num, taillesNoms *array)
{
	int i, j;
	taillesNoms temp;
		
	for (i = 0; i < num; i++)
	{
		// Pour trier les tailles en ordre decroissant.
		for (j = 0; j < (num - i - 1); j++)
		{
			if (array[j].id < array[j + 1].id)
			{
				temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
		}
		
		// Pour trier les noms des fichiers qui ont les meme tailles en ordre alphabetique.
		for (int j = i; j < num; j++)
		{
			if ((array[j].id == array[i].id) && (strcmp(array[i].name, array[j].name) > 0) )
			{	
				temp  = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
        }
	}
	
	// Affichage 	
	for (i = 0; i < num; ++i)
	{
		print_element(array[i].id, array[i].name);
	}
}


/*
parcoursDir cherche les tailles et les noms des fichiers et repertoires.
dirpath permet a la fonction d'entrer dans un sous-repertoire.
counter sert a reperer les contenus principaux et ceux des sous-repertoires.
fileSize represente la taille du fichier.
tailleRepTotal represente la taille totale du repertoire.
nbrElement sera le nombre d'element qui s'affichera.
arr fait appelle au structure taillesNoms.
*/
static int parcoursDir(char *dirpath, int counter)
{
	DIR *dir = opendir(dirpath);
	struct dirent *direntp;
	struct stat fileStat;
	int fileSize = 0;
	int tailleRepTotal = 0;
	char filePath[500];
	int nbrElement = 0;
	taillesNoms *arr = (taillesNoms *) malloc(sizeof(taillesNoms) * 200);


	while ((direntp = readdir(dir)) != NULL)
    {		
		// Exclure les repertoires ".." (parent) et "." (courant)
		if ( (strcmp(direntp->d_name, ".") == 0) || (strcmp(direntp->d_name, "..") == 0) ) continue;
		
		// Pour recuperer les noms des fichiers et repertoires correctement
		sprintf(filePath, "%s/%s", dirpath, direntp->d_name);
		if (lstat(filePath, &fileStat) != 0) continue;
				
		fileSize = fileStat.st_size;
		
		if(S_ISDIR(fileStat.st_mode))
		{	
			// Sort du programme si un repertoire est inacessible.	
			if (!(fileStat.st_mode & S_IREAD)){
				print_erreur(0);
				exit( 0 );
			}
			
			counter++; // A chaque fois on entre dans un sous-repertoire, counter augmente.
			int repSize = parcoursDir(filePath, counter) + fileSize;
			counter--;
			tailleRepTotal += fileSize;
			
			// On n'affiche pas les contenus des sous-repertoires
			if(counter == 0)
			{		
				// Affectation des valeurs obtenues vers variables des structures.
				arr[nbrElement].id = repSize;
				strcpy(arr[nbrElement].name, direntp->d_name);
			}
		}
		else
		{
			tailleRepTotal += fileSize;
			
			// On n'affiche pas les contenus des sous-repertoires
			if(counter == 0)
			{
				// Affectation des valeurs obtenues vers variables des structures.
				arr[nbrElement].id = fileSize;
				strcpy(arr[nbrElement].name, direntp->d_name);	
			}
		}
		nbrElement++;
	}
	
	
	if(counter == 0)
	{
		trierAffichage(nbrElement, arr);
	}
	
	free(arr); // On libere la memoire.
	return tailleRepTotal;
}


int main (int argc, char *argv[])
{
	DIR *dir;
	int compteur = 0;
	
	// Lance un message erreur si le repertoire ou le fichier est inexistant.
	if ( argc != 2 ) {
        print_erreur(1);
        return 1;
    }
	
	if ((dir = opendir(argv[1])) == NULL) {
        print_erreur(1);
        return 1;
    }
		
	parcoursDir(argv[1], compteur); 
		
	closedir(dir);
		
	return 0;
}
