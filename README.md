# echo-server
echo-server && simple-http-server

## versions
```
$ sw_vers
ProductName:	Mac OS X
ProductVersion:	10.15.7
BuildVersion:	19H1615

$ clang++ -v
Homebrew clang version 13.0.1
Target: x86_64-apple-darwin19.6.0
Thread model: posix

$ make -v
GNU Make 3.81
Copyright (C) 2006  Free Software Foundation, Inc.
This is free software; see the source for copying conditions.
There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.

This program built for i386-apple-darwin11.3.0
```

If you dont have `telnet` install this.
```bash
$ brew install telnet
```

## Dir tree

```bash
.
├── LICENSE
├── README.md
├── compile_commands.json
└── src
    ├── echo-server-and-client
    │   ├── Makefile
    │   ├── client.cpp
    │   └── server.cpp
    ├── http
    │   ├── Makefile
    │   ├── http.cpp
    │   └── index.html
    └── simple-echo
        ├── Makefile
        └── main.cpp
 ```
 
## USAGE

### ECHO-SERVER and CLIENT
In `src/echo-server-and-client`
```bash
$ make
$ ./server 7000

# another terminal
$ ./client localhost 7000
```

### SIMPLE-ECHO-SERVER with telnet
In `src/simple-echo`
```bash
$ make
$ ./simple_echo_server

# another terminal
$ telnet localhost 7000
```

### SIMPLE-HTTP-SERVER
In `src/http`
```bash
$ make
$ ./http-server
```
Access http://localhost:8080/
