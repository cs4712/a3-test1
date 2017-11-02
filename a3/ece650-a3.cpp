#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <signal.h>

int main(int argc, char **argv) {
    
    int pipe_r1[2];
    pipe(pipe_r1);
    
    int pipe_12[2];
    pipe(pipe_12);
    
    
    pid_t pid1;
    std::vector<pid_t> kids;
    
    ////////////////////////////////// rgen ------> a1 ///////////////////////////////////
    if ((pid1 = fork()) <0)
    {
        std::cerr << "Error: Unable to create child 1 process" << std::endl;
        return 0;
    }
    
    else if (pid1 == 0) /* In Child Process (generate random numbers)*/
    {
        //redirect stdout
        close(pipe_r1[0]);
        dup2(pipe_r1[1],STDOUT_FILENO);
        close(pipe_r1[1]);
       
        char* argv_rg[argc+1];
        argv_rg[0] = (char*)"./rgen";
        for(int i=1;i<argc;i++){
            argv_rg[i] = argv[i];
        }
        argv_rg[argc] = NULL;
        execv("./rgen",argv_rg);
        
//        char* argv_rg[2];
//        argv_rg[0] = (char*)"./coor";
//        argv_rg[1] = NULL;
//        execv("./coor",argv_rg);
        
        
        
        return 0;
    }
    else  /* In Parent Process (input a1.py)*/
    {
        //        int status;
        //        waitpid(pid, &status, 0);
        kids.push_back(pid1);
        //redirect stdin
        dup2(pipe_r1[0], STDIN_FILENO);
        close(pipe_r1[0]);
        close(pipe_r1[1]);
        
        char* argv_a1[2];
        argv_a1[0] = (char*)"./ece650-a1.py";
        argv_a1[1] = NULL;
        execv("./ece650-a1.py", argv_a1);
        
    }
    
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    /////////////////////////////////////// a1 ------> a2 ///////////////////////////////////////////
    pid_t pid2;
    
    if ((pid2 = fork()) <0)
    {
        std::cerr << "Error: Unable to create child 2 process" << std::endl;
        return 0;
    }
    
    else if (pid2 == 0) /* In Child Process (get V E from a1.py)*/
    {
        //redirect stdout
        close(pipe_12[0]);
        dup2(pipe_12[1],STDOUT_FILENO);
        close(pipe_12[1]);
        
        
        char* argv_a11[2];
        argv_a11[0] = (char*)"./ece650-a1.py";
        argv_a11[1] = NULL;
        execv("./ece650-a1.py", argv_a11);
        
        
        
        return 0;
    }
    else  /* In Parent Process (put V E to a2.cpp)*/
    {
        //        int status;
        //        waitpid(pid, &status, 0);
        kids.push_back(pid2);
        //redirect stdin
        dup2(pipe_12[0], STDIN_FILENO);
        close(pipe_12[0]);
        close(pipe_12[1]);
        
        char* argv_a2[2];
        argv_a2[0] = (char*)"./ece650-a2";
        argv_a2[1] = NULL;
        execv("./ece650-a2",argv_a2);
        
    }
    
    
    
    for (pid_t k : kids) {
        int status;
        kill(k, SIGTERM);
        waitpid(k, &status, 0);
    }
    
    
    return 0;
    
}
