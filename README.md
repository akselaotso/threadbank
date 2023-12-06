# A "bank" using threads and a wait queue.

An app which uses threads to run a server with a specified number of service desks. The server communicates with clients using sockets. The available commands are listed below:

l [account] - read account balance
w [account] [amount] - withdraw from account
t [sending account] [recieving account] [amount] - transfer money between accounts
d [account] [amount] - deposit to an account, only way to create new accounts

First to compile the client and server applications run 
´´´
make
´´´

Next you can launch the server (The command compiles the server if it is not yet compiled) with 
´´´
make server
´´´

And finally you can launch (and compile if not yet compiled) the client...
´´´
make client
´´´

Then the client can then interact with the server using the specified commands.

The makefile contains a number of more and less useful commands some of which were mainly useful during development.

To "clean" the environment run
´´´
make clean
´´´