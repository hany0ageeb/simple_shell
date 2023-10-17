#!/usr/bin/bash
rm -r tests/build
mkdir tests/build
clear
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 tests/mem.test.c mem.c -o tests/build/mem.test.out && valgrind tests/build/mem.test.out
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 test/io.test.c io.c io2.c -o tests/build/io.test.out && valgrind test/build/io.test.out
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 tests/string.test.c string.c string2.c string3.c -o tests/build/string.test.out && valgrind tests/build/string.test.out ./ test.txt
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 tests/token.test.c token.c string.c -o tests/build/token.test.out && valgrind tests/build/token.test.out
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 tests/token_list.test.c string.c token.c token_list.c -o tests/build/token_list.test.out && valgrind tests/build/token_list.test.out
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 scan_tokens.c scan_tokens2.c string.c shell.c io.c io2.c token_list.c token.c string2.c tests/scan_tokens.test.c mem.c -o tests/build/scan_tokens.test.out && valgrind tests/build/scan_tokens.test.out
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 tests/simple_command.test.c simple_command.c token.c token_list.c string.c -o tests/build/simple_command.test.out && valgrind --track-origins=yes tests/build/simple_command.test.out
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 tests/parse_tokens.test.c simple_command.c parse_tokens.c scan_tokens.c scan_tokens2.c token_list.c token.c string.c -o env.c builtin.c string2.c string3.c io.c io2.c shell.c mem.c alias.c alias_list.c str_list.c env.c session.c -o tests/build/parse_tokens.test.out && valgrind tests/build/parse_tokens.test.out
