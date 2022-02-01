# Base de donnée distribué 
Un base de donnée distribuer implementer en C
Restpectant:
- La disponibilité sous partition
- La consistence sans partion

## NB
``Anycast`` addressed packets are used to deliver a message to one recipient when you don't
care who that recipient is. This is useful if you have several servers that provide the same
functionality, and you simply want one of them (you don't care which) to handle your
request.

The ``fgets()`` function returns a pointer to the string buffer if successful.
A NULL return value indicates an error or an end-of-file condition. 
Use the ``feof()`` or ``ferror()`` functions to determine whether the NULL value indicates an error or the end of the file.
In either case, the value of the string is unchanged.