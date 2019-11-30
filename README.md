# VisBruf
A portable Brainfuck interpreter written in C
WORK IN PROGRESS

## Standard Brainfuck

Standard brainfuck commands are supported, and can be either entered at the prompt or read in through a file. 

## Input Args

- To load a brainfuck file run `$ visbuf -f filename`
- Or enter commands in the prompt `@ commands`

## Simple Mode

Simple mode will only respond to the eight brainfuck commands, and can load a file from the command line arguments with the `-f` flag. Only default settings can be used with simple mode. Simple mode will still show the data in memory, although the memory viewer cannot be controlled. To launch the program in simple mode use the `-s` or `--simple-mode` flags. Simple mode flag must be the first flag in the list if used.
To quit the executable in simple mode you must use Ctrl-C.

## Tests

This is still a work in progress. Some example brainfuck files online are working correctly, others are not

- [Bitwidth Test Works](https://github.com/rdebath/Brainfuck/blob/master/bitwidth.b) 
- [Mandelbrot Fractal Viewer not currently working](https://github.com/erikdubbelboer/brainfuck-jit/blob/master/mandelbrot.bf)

