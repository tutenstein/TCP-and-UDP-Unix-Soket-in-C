# TCP-and-UDP-Unix-Soket-in-C
A simple TCP and UDP unix socket program written in C language
# COMPILE
gcc server.c -o Server  
gcc client.c -o Client
# What are the parameters
./Server [protocol type] [Port no]  
protocol type ----> TCP = 1, UDP = 2  
./Client [protocol type] [Port no] [Server hostname]   
protocol type ----> TCP = 1, UDP = 2  
# Example 
$ ./Server 1 9898  
$ ./Client 1 9898  127.0.0.1  
