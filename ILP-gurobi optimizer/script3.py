#!/usr/bin/python

import os
import string
import sys
freq=3
#with open("1GHz_Power", "r") as infile1, open("2GHz_Power","r"), open("Output.txt", "w") as outfile:
#    for line1 in infile1:
#			strcleaned = ''.join([x for x in line1 if (ord(x) < 128 and ord(x) >32) ])
#			print strcleaned	#,line2 in  infile2:
##			        outfile.write(line1 + "\n" + line2 + "\n")
P1 = open('1ghz-power', 'r')
P2 = open('1.6ghz-power', 'r')
P3 = open('2ghz-power','r')
D1 = open('1ghz-delay','r')
D2 = open('1.6ghz-delay','r')
D3 = open('2ghz-delay','r')

#
P = open('power.txt','w')
D = open('delay.txt','w')

#for i, j in zip(P1,P2):
#			print i #.encode('utf8')
 		
     #do something with i and j.
 		#	    write to an output file.
 		#			    no need to do anything with i and j after this.

linesp1 = [line.strip() for line in open('1ghz-power')]
linesp2 = [line.strip() for line in open('1.6ghz-power')]
linesp3 = [line.strip() for line in open('2ghz-power')]
linesd1 = [line.strip() for line in open('1ghz-delay')]
linesd2 = [line.strip() for line in open('1.6ghz-delay')]
linesd3 = [line.strip() for line in open('2ghz-delay')]

counter1 = len(linesp1)
#counter2 = len(lines2)
#print counter1
#print '\n'
#print counter2
#print '\n'
P.write(repr(counter1)+'\n')
P.write(repr(freq)+'\n')
D.write(repr(counter1)+'\n')
D.write(repr(freq)+'\n')
#counter1=counter1-2
#file.writelines(["%s\n%s\n" % (item for iiem in lines1, item for item in lines2)])
for i in range(0,counter1):
	strcleaned1 = ''.join([x for x in linesp1[i] if (ord(x) < 128 and ord(x) >32) ])
	strcleaned2 = ''.join([x for x in linesp2[i] if (ord(x) < 128 and ord(x) >32) ])
	strcleaned3 = ''.join([x for x in linesp3[i] if (ord(x) < 128 and ord(x) >32) ])
	P.write(strcleaned1+"\n"+ strcleaned2 + "\n"+ strcleaned3 + "\n")
for i in range(0,counter1):
	strcleaned1 = ''.join([x for x in linesd1[i] if (ord(x) < 128 and ord(x) >32) ])
	strcleaned2 = ''.join([x for x in linesd2[i] if (ord(x) < 128 and ord(x) >32) ])
	strcleaned3 = ''.join([x for x in linesd3[i] if (ord(x) < 128 and ord(x) >32) ])
	D.write(strcleaned1+"\n"+ strcleaned2 + "\n" + strcleaned3 + "\n")
	#P.write(lines1[i]+"\n"+ lines2[i] + "\n")
 #P.write(repr(lines1[i])+'\n'+ repr(lines2[i]) + '\n')
 #P.write(repr(lines2[i])+'\n')
#for i in range(2,counter1):
#	print lines1[i]
#	P.write((lines1[i])+'\n'+(lines2[i])+'\n')	
#
#for i in lines1:
#	for j in lines2:
#		P.write("%s\n%s\n" % (i,j))
#	#P.write("%s\n" % lines2[i])

P1.close()
P2.close()
P3.close()
D1.close()
D2.close()
D3.close()
P.close()
D.close()
