# gserv
A lean and simple UDP graphics server (~22kb compiled), written in C

## Requirements

* SDL2

## Build

make

(server currently is tested on OSX only, client works on OSX and Linux)

## Start

Start a graphics server
    ./gserv

Send some test commands to the server started on localhost
    ./gstest
    
To run ./gstest on from another machine, adjust server address UDP_ADDR in gstest.c

