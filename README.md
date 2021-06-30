# Pounce C-CORE
(work in progress) "C" code version of Pounce (parser and interpreter) intended for micro controllers, or web assembly.

## build the REPL with gcc
`gcc -o ./dist/repl ./src/repl.c ./src/linenoise/linenoise.c`

`./dist/repl`

(possibly permissions need to be adjusted `chmod +x ./dist/repl`)

### Try the REPL Help
`./dist/repl -help`

### Feed input to the REPL
`./dist/repl -i tests/source.p`

### some tests
`gcc -o ./dist/tests ./src/tests.c`

`./dist/tests`

Running this may take a few seconds since it does a simple stress test.

### valgrind check for memory leaks
`valgrind --leak-check=yes ./dist/repl`


## rp-2040 pico (M0 micro controller) REPL 
`export PICO_SDK_PATH=<myRepoPath>/pico-sdk`

`cd build-pico`

`cmake ..`

`make pounce_repl`