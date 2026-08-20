while (TRUE) { /* repeat forever */
type_prompt(void){
    printf('%%\n');
}

read_command (command, parameters) /* input from terminal */
if (fork() != 0) { /* fork off child process *//* Parent code */
waitpid( -1, &status, 0); /* wait for child to exit */} else {
/* Child code */
execve (command, parameters, 0); /* execute command */
}
} 