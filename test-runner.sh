#! 

# gcc -o ./dist/repl ./src/repl/repl.c ./src/linenoise/linenoise.c

./dist/repl -i tests/crouch-test.p
./dist/repl -i tests/dip-test.p
./dist/repl -i tests/dip2-test.p
./dist/repl -i tests/drop-test.p
./dist/repl -i tests/dup-test.p
./dist/repl -i tests/dup-test-1.p
./dist/repl -i tests/dup-test-2.p
./dist/repl -i tests/dup-test-3.p
./dist/repl -i tests/if-else-test.p
./dist/repl -i tests/play-test.p
./dist/repl -i tests/reverse-list.p
./dist/repl -i tests/times-test.p
./dist/repl -i tests/concat-test.p
./dist/repl -i tests/uncons-test.p
./dist/repl -i tests/map-test.p

echo "valgrind "
valgrind --leak-check=yes -q ./dist/repl -i tests/crouch-test.p
valgrind --leak-check=yes -q ./dist/repl -i tests/dip-test.p
valgrind --leak-check=yes -q ./dist/repl -i tests/dip2-test.p
valgrind --leak-check=yes -q ./dist/repl -i tests/drop-test.p
valgrind --leak-check=yes -q ./dist/repl -i tests/dup-test.p
valgrind --leak-check=yes -q ./dist/repl -i tests/dup-test-1.p
valgrind --leak-check=yes -q ./dist/repl -i tests/dup-test-2.p
valgrind --leak-check=yes -q ./dist/repl -i tests/dup-test-3.p
valgrind --leak-check=yes -q ./dist/repl -i tests/if-else-test.p
valgrind --leak-check=yes -q ./dist/repl -i tests/play-test.p
valgrind --leak-check=yes -q ./dist/repl -i tests/reverse-list.p
valgrind --leak-check=yes -q ./dist/repl -i tests/times-test.p
valgrind --leak-check=yes -q ./dist/repl -i tests/concat-test.p
valgrind --leak-check=yes -q ./dist/repl -i tests/uncons-test.p
valgrind --leak-check=yes -q ./dist/repl -i tests/map-test.p
