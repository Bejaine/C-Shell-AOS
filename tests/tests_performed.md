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