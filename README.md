HTTP Server
===========

This is very simple implementation of HTTP server in C++. It carries both HTTP server and some very simple code to execute for testing

## Build

To build this project you have to have BOOST unit test (if you want to run unit tests).

Anyway, run:

`make all`

that runs build and tests or

`make server.o`

that compiles only server.

## Usage

Running the code is done via: `./server.o 8080` where 8080 is the port (actually default).

## Structure

The code is structured in very simple way:

* server.cpp - sample code with handlers
* server
** src
*** *.h - header files
*** *.cpp - C++ files
** test
*** *_test.cpp - all BOOST unit tests written for the server
* Makefile - make file
* log.config - an example of log config file
* README.md - this readme

## Logging

The server contains interesting logging configuration. Either:

1. provide SERVER_LOGGER=stdout or file that will use logging to file or to stdout only
2. do not provide anything and rely on HybridLoggerFactory that uses configuration (see hybrid logging section)

### Hybrid Logging

Consider a file `log.config` that contains such entries:

```
TcpConnection:stdout:debug
Tcp.*:file:debug
dispatcher:file:warn
```

Each of the entry is in the format: `Regex:(file|stdout):(debug|warn|error)`. Each of the logger (getLogger(someName)) can be logged
in different way, as in the above example:
* TcpConnection will be logged with debug level to stdout
* other TCP-related debug and higher events will be logged into file
* dispatcher will be logged into file only for warning and error (no debug)


