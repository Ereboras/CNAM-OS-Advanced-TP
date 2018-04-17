#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
	//ps eaux | grep "^root " > /dev/null && echo "root est connecté"
	// ps
	//		afficher les processus actifs ainsi que ces ressources utilisé à un instant t, par utilisateur, par PID
	// -aux
	// 		permet de visualiser affiche les utilisateurs associés à chaque processus
	// |
	// 		Execute le résultat de la première commande sur la deuxième
	// grep
	// 		filtre
	// "^root"
	//		Ce qui commence par root
	// >
	//		Rediriger dans un nouveau fichier
	// /dev/null
	//		Chemin d'accès du nouveau fichier

	// &&
	// 		Execute le code suivant si le précédent renvoie 0
	// echo
	//		Affiche du texte
	// "root est connecté"

	int statval1, statval2;
	int fd[2];

	// Création du pipe
	pipe(fd);

	if ( fork() == 0 ) {

		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);

		// Execute la commande ps eaux
		execlp ("ps", "ps", "eaux", (void*)0);

		// Récupère les erreurs
		perror("Erreur PS : ");
		exit(1);
	}

	if (fork() == 0 )
	{
		char buffer[1024];
		close(fd[1]);
		dup2(fd[0], 0);

		// Création du fichier dans /dev/null
		int fd = open("/dev/null", O_WRONLY);
		dup2(fd, 1);

		// Execute la commande grep root
		execlp ("grep", "grep", "^root", (void*)0);

		// Récupère les erreurs
		perror("Erreur GREP : ");
		exit(1);
	}

	close(fd[1]);
	close(fd[0]);

	// Attend la fin des processus
	wait(&statval1);
	wait(&statval2);

	// Vérifie si les processus se sont terminés normalement
	if(WIFEXITED(statval1) && WIFEXITED(statval2) && WEXITSTATUS(statval1) == 0 && WEXITSTATUS(statval2) == 0 )
		write(1, "root est connecté\n", 19);
	return 0;
}
