#!/usr/bin/python
from socket import *
import ctypes
import struct
import time
import sys

if len(sys.argv) != 3:
	print("usage: ./hclt.py host port")
	sys.exit(-1)
	
isize=88
idata = ctypes.create_string_buffer(isize)
for index in range(0, isize):
	struct.pack_into('B', idata, index, index%255)
	
HOST = sys.argv[1]
PORT = int(sys.argv[2])
t_len=128
osize = 1024*1024
ADDR = (HOST, PORT)
tcpCliSock = socket(AF_INET, SOCK_STREAM)
tcpCliSock.connect(ADDR)

cnt = 0;
while 1:
	ssize = 0
	while ssize != isize:
		ssize += tcpCliSock.send(idata[ssize:])
		print("sendsize:%d"%ssize)
    
	rsize=0
	while rsize != osize:
		data = ''
		data = tcpCliSock.recv(osize-rsize)
		if 0 == len(data):
			continue
		rsize += len(data)
		datastr = ' '.join( str(du) for du in struct.unpack_from('8B', data, 0) )
		print("rsize:%d,%d,%s"%(rsize, len(datastr), datastr ) )
		#time.sleep(10)
	cnt = cnt+1
	print('%d,%d'%(cnt,rsize))
    
tcpCliSock.close()

