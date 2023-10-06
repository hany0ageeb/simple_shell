#!/usr/bin/bash
rm -r tests/build
mkdir tests/build
clear
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 tests/mem.test.c mem.c -o tests/build/mem.test.out && valgrind tests/build/mem.test.out
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 tests/string.test.c string.c -o tests/build/string.test.out && valgrind tests/build/string.test.out
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 tests/token.test.c token.c string.c -o tests/build/token.test.out && valgrind tests/build/token.test.out
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 tests/token_list.test.c string.c token.c token_list.c -o tests/build/token_list.test.out && valgrind tests/build/token_list.test.out
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 tests/scan_tokens.test.c scan_tokens.c token.c token_list.c string.c -o tests/build/scan_tokens.test.out && valgrind tests/build/scan_tokens.test.out

