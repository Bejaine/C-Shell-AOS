### Prompt Display

Invoked shell in testdir with a subdirectory testdir2 and another directory testdir3 along with testdir1 and used `chdir("../testdir3");` to obtain
```
user1@DESKTOP-6R849NP:~/aos/testdir$ gcc -Wall -Wextra ../src/main.c ../src/prompt.c -o shell && ./shell
<user1@DESKTOP-6R849NP:/home/user1/aos/testdir3>
```
and used `chdir("testdir2");` to obtain
```
user1@DESKTOP-6R849NP:~/aos/testdir$ gcc -Wall -Wextra ../src/main.c ../src/prompt.c -o shell && ./shell
<user1@DESKTOP-6R849NP:~/testdir2>
```
Output for the test obtained correctly

### PARSING INPUT

Test with input containing multiple delimiters in the string, test with an empty input and test with EOF through Ctrl+D
```
user1@DESKTOP-6R849NP:~/aos/src$ gcc -Wall -Wextra ../src/main.c ../src/prompt.c ../src/input.c -o shell && ./shell
<user1@DESKTOP-6R849NP:~> echo    hello      world
Debug: Parsed Tokens
args[0]: 'echo'
args[1]: 'hello'
args[2]: 'world'

<user1@DESKTOP-6R849NP:~> 
<user1@DESKTOP-6R849NP:~> 
Exiting shell...
user1@DESKTOP-6R849NP:~/aos/src$
```
Outputs for the tests obtained correctly

### ECHO

Test input with multiple spaces between words in echo
```
user1@DESKTOP-6R849NP:~/aos/C-Shell-AOS/src$ gcc -Wall -Wextra ../src/main.c ../src/prompt.c ../src/input.c ../src/builtins.c -o shell && ./shell
<user1@DESKTOP-6R849NP:~> echo      all       the      best
all the best
<user1@DESKTOP-6R849NP:~> echo all the best
all the best
<user1@DESKTOP-6R849NP:~>
```
Output for the test obtained correctly

### PWD

Testing current working directory with pwd command as well as multiple inputs
```
user1@DESKTOP-6R849NP:~/aos/C-Shell-AOS/src$ gcc -Wall -Wextra ../src/main.c ../src/prompt.c ../src/input.c ../src/builtins.c -o shell && ./shell
<user1@DESKTOP-6R849NP:~> pwd
/home/user1/aos/C-Shell-AOS/src
<user1@DESKTOP-6R849NP:~> pwd cd test
/home/user1/aos/C-Shell-AOS/src
```
Output for the test obtained correctly

### CD

Tested all commands across directories
```
user1@DESKTOP-6R849NP:~/aos/C-Shell-AOS/src$ gcc -Wall -Wextra ../src/main.c ../src/prompt.c ../src/input.c ../src/builtins.c -o shell && ./shell
<user1@DESKTOP-6R849NP:~> cd - 
cd: no previous directory
<user1@DESKTOP-6R849NP:~> pwd
/home/user1/aos/C-Shell-AOS/src
<user1@DESKTOP-6R849NP:~> cd ..
<user1@DESKTOP-6R849NP:/home/user1/aos/C-Shell-AOS> cd ~
<user1@DESKTOP-6R849NP:~> cd -
/home/user1/aos/C-Shell-AOS
<user1@DESKTOP-6R849NP:/home/user1/aos/C-Shell-AOS> cd 
<user1@DESKTOP-6R849NP:~>
```
Outputs for the test obtained correctly

### HISTORY

Invoking the shell in a directory without shell history and running tests such as repeated commands and invoking after exiting the shell
```
user1@DESKTOP-6R849NP:~/aos/C-Shell-AOS/src$ ls -a
.  ..  builtins.c  history.c  input.c  main.c  prompt.c
user1@DESKTOP-6R849NP:~/aos/C-Shell-AOS/src$ gcc -Wall -Wextra ../src/main.c ../src/prompt.c ../src/input.c ../src/builtins.c ../src/history.c -o shell && ./shell
<user1@DESKTOP-6R849NP:~> history
history
<user1@DESKTOP-6R849NP:~> pwd
/home/user1/aos/C-Shell-AOS/src
<user1@DESKTOP-6R849NP:~> echo hello
hello
<user1@DESKTOP-6R849NP:~> cd ..
<user1@DESKTOP-6R849NP:/home/user1/aos/C-Shell-AOS> cd -
/home/user1/aos/C-Shell-AOS/src
<user1@DESKTOP-6R849NP:~> history
history
pwd
echo hello
cd ..
cd -
history
<user1@DESKTOP-6R849NP:~> pwd
/home/user1/aos/C-Shell-AOS/src
<user1@DESKTOP-6R849NP:~> pwd
/home/user1/aos/C-Shell-AOS/src
<user1@DESKTOP-6R849NP:~> history
history
pwd
echo hello
cd ..
cd -
history
pwd
history
<user1@DESKTOP-6R849NP:~> 
Exiting shell...
user1@DESKTOP-6R849NP:~/aos/C-Shell-AOS/src$ ls -a
.  ..  .history  builtins.c  history.c  input.c  main.c  prompt.c  shell
user1@DESKTOP-6R849NP:~/aos/C-Shell-AOS/src$ gcc -Wall -Wextra ../src/main.c ../src/prompt.c ../src/input.c ../src/builtins.c ../src/history.c -o shell && ./shell
<user1@DESKTOP-6R849NP:~> history
history
pwd
echo hello
cd ..
cd -
history
pwd
history
<user1@DESKTOP-6R849NP:~> 
```
Outputs for the tests obtained correctly

### FOREGROUND PROCESS EXECUTION

Tested execvp execution by running external commands `ls` and `ls -a` and waitpid execution by running `sleep 3`
```
user1@DESKTOP-6R849NP:~/aos/C-Shell-AOS/src$ gcc -Wall -Wextra ../src/main.c ../src/prompt.c ../src/input.c ../src/builtins.c ../src/history.c ../src/execute.c -o shell && ./shell
<user1@DESKTOP-6R849NP:~> ls
builtins.c  execute.c  history.c  input.c  main.c  prompt.c  shell
<user1@DESKTOP-6R849NP:~> ls -a
.  ..  .history  builtins.c  execute.c  history.c  input.c  main.c  prompt.c  shell
<user1@DESKTOP-6R849NP:~> fakecommand
fakecommand: No such file or directory
<user1@DESKTOP-6R849NP:~> sleep 3
<user1@DESKTOP-6R849NP:~> 
```
Outputs for the external commands through execvp obtained correctly and parent shall waits 3 seconds as specified in the child process through waitpid