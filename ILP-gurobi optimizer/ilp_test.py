#!/usr/bin/python

from gurobipy import *

import os
fx = open('outfile','w')
fp = open('power.txt','r')
fd = open('delay.txt','r')
linesp = [line.strip() for line in open('power.txt')]
linesd = [line.strip() for line in open('delay.txt')]
fp.close()
fd.close()
counter = 2

def gap(model):
    if model.solCount == 0 or abs(model.objVal) < 1e-6:
        return GRB.INFINITY
    return abs(model.objBound - model.objVal)/abs(model.objVal) * 100
    
def printsolution(model):
  # Print solution
  if model.status == GRB.status.OPTIMAL: 
    print "Solved to optimality"
    print "Objective value = " , model.ObjVal
    print "Run Time        = " , model.Runtime 
  elif model.status ==  GRB.status.TIME_LIMIT:
    print "Time limit reached"
    print "Lower Bound        = " , model.ObjBound 
    print "Upper Bound        = " , model.ObjVal 
    print "Node Count        = " , model.NodeCount 
    print 'MipGap = ' + str(gap(model))  


# Sets and data (just indexed by numbers)
I = int(linesp[0])
J = int(linesp[1])
#I = 2
#J = 2
Iset = range(I)  
Jset = range(J)  

#P = [[10,20],[20,30]]
P = []
for i in Iset:
	row = []
	P.append(row)
	for j in Jset:
		P[i].append(float(linesp[counter]))
		counter = counter + 1
counter = 2
PIJ = {}
for i in Iset:
    for j in Jset:
        PIJ[i,j]=P[i][j]
			

#D = [[10,20],[20,30]]
 
D = []
for i in Iset:
	row = []
	D.append(row)
	for j in Jset:
		D[i].append(float(linesd[counter]))
		counter = counter + 1
counter = counter -1
DIJ = {} 
for i in Iset:
    for j in Jset:
         DIJ[i,j]=D[i][j]        

#Define your P[I,J] here


m = Model('test')

### Build the model  below here

# Create variables
x = {}
for i in Iset: 
    for j in Jset:
       x[i,j] = m.addVar(vtype=GRB.BINARY,obj=P[i][j],name='x_%s_%s' % (i, j))

#integrate new variables
m.update()
             
# Constraints
m.addConstr(
          quicksum(DIJ[i,j] * x[i,j] for i in Iset for j in Jset) <= 73230.03,
                    'capacity' )


for i in Iset:                  
   m.addConstr(
          quicksum(x[i,j] for j in Jset) == 1,
                   'eqn2_%s_%s' %(i,j) )

#for i in Iset:
#	for j in Jset:
#		m.addConstr(
#					quicksum(x[i,j]) == I,
#									'eqn2_%s_%s' %(i,j) )

m.optimize()
print "Before using l-s inequalities"
printsolution(m)

for i in Iset:
	for j in Jset:
		#print x[i,j].x
		fx.write(repr(x[i,j].x) + '\n')
fx.close()
