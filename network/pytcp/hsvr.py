#!/usr/bin/env python
from socket import *
from time import time, ctime
import ctypes
import sys
from struct import *

if len(sys.argv) != 3:
	print("usage: ./hsvr.py host port")
	sys.exit(-1)

HOST = sys.argv[1]
PORT = int(sys.argv[2])
osize = 1024*1024
ADDR = (HOST, PORT)

tcpSerSock = socket(AF_INET, SOCK_STREAM)
tcpSerSock.bind(ADDR)
tcpSerSock.listen(5)

isize = 88

odata = ctypes.create_string_buffer(osize)
for index in range(0, osize):
	pack_into('B', odata, index, index%255)

while 1:
        print 'waiting for connection...'
        tcpCliSock, addr = tcpSerSock.accept()
        print '...connected from:', addr

	cnt = 0
	while 1:
   	 	rsize = 0;
    		while rsize != isize:
    			data = tcpCliSock.recv(isize-rsize)
    			rsize += len(data)
			print('recv:%d'%rsize)
		cnt = cnt+1
    		print('%d:%d'%(cnt,rsize))
    	
		ssize = 0;
		while ssize != osize:
        		ssize += tcpCliSock.send(odata[ssize:])
			print('send:%d'%ssize)

