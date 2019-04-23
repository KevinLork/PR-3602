#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char **argv) {
	int fd0[2];
	int fd1[2];
	int fd2[2];
	int fd3[2];

	if (pipe(fd0) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd1) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd3) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

	pid_t pid;
	pid_t pid2;
	char buf;

	printf("%s\n", "je vais creer un fils terminal");
	pid = fork();

	if (pid == 0){	//le fils - terminal
		printf("%s\n", "je suis le fils terminal");
		char s_fd0[50], s_fd1[50];
		sprintf(s_fd0, "%d", fd0[0]);
		sprintf(s_fd1, "%d", fd1[0]);
		execl("./terminal","./terminal", s_fd0, s_fd1, NULL);
		printf("%s\n", "j'ai exec terminal.c");
	}
	else if(pid < 0){	//erreur
		perror("erreur fork1");
		exit(EXIT_FAILURE);
	}
	printf("%s\n", "je vais creer un fils autorisation");
	pid2 = fork();
	if(pid2 == 0){	//fils - autorisation
		printf("%s\n", "je suis le fils autorisation");
		close(fd2[0]);
		char s_fd2[50], s_fd3[50];
		sprintf(s_fd2, "%d", fd2[0]);
		sprintf(s_fd3, "%d", fd3[0]);

		execlp("./acquisition","./acquisition", s_fd2, s_fd3, NULL);
		printf("%s\n", "j'ai exec autorisation.c");
	}
	else if (pid2 < 0){
		perror("erreur fork2");
		exit(EXIT_FAILURE);
	}
	close(fd0[0]);
	close(fd1[0]);
	close(fd2[0]);
	close(fd3[0]);

	while(read(fd1[1], &buf, 255)){
		
		write(fd2[1], &buf, 255);
	}

	while(read(fd3[1], &buf, 255)){
			
		write(fd0[1], &buf, 255);
	}

	
return 0;
}