# Pounce C-CORE
(work in progress) "C" code version of Pounce (parser and interpreter) intended for micro controllers, or web assembly.

## build

`gcc -o ./dist/repl ./src/repl.c ./src/linenoise/linenoise.c`

`./dist/repl`

(possibly permissions need to be adjusted `chmod +x ./dist/repl`)

### Help
`./dist/repl -help`

## some tests
`gcc -o ./dist/tests ./src/tests.c`

`./dist/tests`

Running this may take a few seconds since it does a simple stress test.

## valgrind check
`valgrind --leak-check=yes ./dist/repl`