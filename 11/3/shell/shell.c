#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    int shell_to_app[2] = {0};
    int app_to_shell[2] = {0};

    pipe(shell_to_app);
    pipe(app_to_shell);

    if(fork() == 0)
    {
        close(shell_to_app[1]);
        close(app_to_shell[0]);

        dup2(shell_to_app[0], STDIN_FILENO);
        dup2(app_to_shell[1], STDOUT_FILENO);

        execl("./app.exe", "./app.exe", NULL);
    }
    else
    {
    	int i = 0;
    	FILE* in = fdopen(app_to_shell[0], "r");
    	FILE* out = fdopen(shell_to_app[1], "w");

        char message[256] = {0};
        close(shell_to_app[0]);
        close(app_to_shell[1]);
	
		fgets(message, sizeof(message), in);
        printf("1: %s\n", message);
        fprintf(out, "1234\n");
        fflush(out);
		fgets(message, sizeof(message), in);
        printf("2: %s\n",  message);
		fclose(in);
		fclose(out);
    }

    return 0;
}

