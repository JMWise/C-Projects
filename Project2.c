/* Author: (Jordan Wise, UIN: 655963633, jwise6@uis.edu)
 Compile: input the following two commands 
 -> "gcc -std=c99 -o Project2.out Project2.c"
 -> "touch history.txt"
 Brief Description: This program immitates a shell and uses forking to execute commands as well as having some history functions.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define MAX_LINE 80 /* The maximum length command */
#define MAX_ARGS 10 /* The maximum number of arguments */
#define MAX_Storage 10

struct record {
    int count;
    char cmd[80];
} entry;

// this creates a list of char arrays I can check for the user input !1-!10
generatelistofcommands(char list[][30])
{
    for(int y=0; y <10; y++)
        {
            sprintf(list[y], "!%d", y);
            
        }
}



//This parses a char array into an array of char* so that it can be used by execvp
parsecmmd(char* cmmd, char ** args)
{
    char *newline;
    char *parsed = strtok (cmmd, " ,.");
    int x = 0;
    while (parsed != NULL)
    {
            
            newline = strchr(parsed, '\n');
            if (newline) {
                *newline = '\0';
            }
            args[x] = parsed;
            
            
            parsed = strtok (NULL, " ");

            
            x++;
    }
    args[x] = NULL;
}

main()
{
        char *args[MAX_ARGS] = {NULL}; /* command line arguments */
        char *history[MAX_Storage] = {NULL};
        int should_run = 1; /* flag to determine when to exit program */
        int cmmdposition = 0;
        int y = 0;
        char history2[100][80]; //this is to store char* of previous commands
            
        //this is where you should read in the history commands from the bin file (and increment y)--------------------------------------------------------------------------------
        
        FILE *fptr;
        int r;
        
        if((fptr = fopen("history.txt", "rb")) == NULL)
        {
            printf("fopen [rb] error!\n");
            exit(1);
        }
        
        for(r = 0; r < 10; r++)
        {
            fread(&entry, sizeof(struct record), 1, fptr);
            strcpy(history2[y], entry.cmd);
            y++;
        }
        
        while (should_run){
                printf("COMMAND-> ");
                fflush(stdout);
               
               //these six lines create char* to compare user inputs
                char exit_str[30];
                strcpy(exit_str, "exit");
                char mfu[30];
                strcpy(mfu, "mfu");
                char recent[30];
                strcpy(recent, "recent");
                char recentexc[30];
                strcpy(recentexc, "!!");
       
                char historycommands[11][30];

                //make a list of strings to compare to input for the history commands
                generatelistofcommands(historycommands);
                

                int stringcomparision;
                int selectedcmmd = 0;
                int pid;
                char cmmd[256];
                
                //takes user input, stores it in the history(will be overwritten if it isn't a execvp command), then parses it.
                fgets(cmmd, 256, stdin);
                strcpy(history2[y], cmmd);
                parsecmmd(cmmd, args);
                char ** cmmd_to_run = args;
                
                
                
                //looks for user input !1-!10 and executes the appropriate command 
                for(int z =0; z <10; z++)
                {
                    if(strcmp(historycommands[z], args[0])==0)
                    {
                        if(y<z)
                        {
                            printf("No such command in history.\n");
                            continue;
                        }
                        strcpy(cmmd, history2[y-z]);
                        strcpy(history2[y], cmmd);
                        parsecmmd(cmmd, args);
                        printf("\n");
                    }
                }
                
                
                //checks for user input "Exit"
                if ((strcmp(exit_str, args[0])) == 0)
                {
                        printf("Exiting");
                        //Write to the history.bin --------------------------------------------------------------------------------------------------------------------
                        
                        
                        int historycounts[10];
                        
                        for(int u = 0; u < y; u++)
                        {
                            for(int p = 0; p < y; p++)
                            {
                                if(strcmp(history2[u], history2[p]) == 0)
                                {
                                    historycounts[u] = historycounts[u] + 1;
                                }
                            }
                        }
                        
                        
                        FILE *fptr;
                        char *filename = "history.txt";
                        errno = 0;
                        int i;
                        if ((fptr = fopen(filename,"wb")) == NULL) {  // open a binary file for writing (overwrite contents)
                           printf("fopen [wb] error!\n");  // file open failed
                           printf("Error %d", errno);
                           perror(filename);
                           //exit(1);
                        }
                        
                        for (i = 0; i < y; i++)
                        {
                            entry.count = historycounts[i];
                            strcpy(entry.cmd, history2[i]);
                            fwrite(&entry, sizeof(struct record), 1, fptr);
                        }
                        fclose(fptr);
                    
                        
                        
                        return 0;
                }
                //prints out the top 5 most frequently used commands and their corresponding occurance count line by line in a non-increasing order-----------------------
                
                else if(strcmp(mfu, args[0]) == 0)
                {
                    int mfucounts[10];
                    char mfucommands[100][80];
                    int z = 1;
                    for(int l = 0; l < y; l++)
                    {
                        for(int h = 0; h < z; h++)
                        {
                            if(strcmp(history2[l], mfucommands[h]) == 0)
                            {
                                mfucounts[h] = mfucounts[h] + 1;
                                break;
                            } else if (strcmp(mfucommands[h], "") == 0)
                            {
                                strcpy(mfucommands[h], history2[l]);
                                mfucounts[h] =  1;
                                z = z + 1;
                                break;
                            }
                        }
                    }
                    
                    int currenthighest;
                    if (z > 6)
                    {
                        z = 6;
                    }
                    
                    
                    for(int l = 0; l < z - 1 ; l++)
                    {
                        for(int h = 0; h < 10; h++)
                        {
                            if(mfucounts[h] > mfucounts[currenthighest])
                            {
                                currenthighest = h;
                            }
                        }
                        printf("%s was used %d times \n", mfucommands[currenthighest], mfucounts[currenthighest]);
                        mfucounts[currenthighest] = 0;
                        
                    }
                    
                    continue;
                }


                //prints out the previous commands
                else if (strcmp(recent, args[0]) == 0)
                {
                        
                        int k = 1;
                        for(int q=y-1;q>=0;q-- )
                        {
                            printf("%d ", k);
                            printf(history2[q]);
			    printf("\n");
                            k++;
                        }
                        continue;
                }

                //executes the most recent command
                else if (strcmp(recentexc, args[0]) == 0)
                {
                        if(y==0)
                        {
                            printf("No commands in history \n");
                            continue;
                        }
                        printf("recentexc\n");
                        strcpy(cmmd, history2[y-1]);
                        strcpy(history2[y], cmmd);
                        parsecmmd(cmmd, args);

                }
                //changes the Char** that will be input into execvp in the event the original user input wasn't the execvp command, e.g. "!!""
                cmmd_to_run = args;
                
                
                
                // creates a child process
                pid = fork();
                
                if (pid == 0)
                {

                        //child command
                        char *cmdtst = args[0];
                        if(execvp(cmmd_to_run[0], cmmd_to_run) == -1)
                        {
                            exit(EXIT_FAILURE);
                        }


                } else {
                        
                        //parent process
                        pid = wait(NULL);
                }
                
            if(y > 8)
            {
                y = 0;
            } else {
                y++;
            }
        }
}


