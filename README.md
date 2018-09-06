# gserv
A lean and simple UDP graphics server (~22kb compiled on OSX), written in C

## Requirements

* SDL2

(Tested on Linux and OSX)

## Build

```
    make
```    


## Start

Start a graphics server
```
    ./gserv
```    

Send some test commands to the server started on `localhost`
```
    ./gstest
```    

To run `./gstest` on from another machine, adjust server address `UDP_ADDR` in `gstest.c`

