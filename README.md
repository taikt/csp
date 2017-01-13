
# CSP solvers in AI

pseudo code
## 1. Backtracking algorithm
## 2. Minimum conflict algorithm
 function Min-confict(csp,max_steps) return a solution or failure
 
 inputs: csp, a constraint satisfaction problem.
 
 max_steps - the number of steps allowed before giving up.
 
 current= an intial complete assignment for csp.
 
 for i=1 to max_steps do.
 
  	if current is a solution for csp then return current
	
	var= a randomly chosen conflicted variable from csp.Variables
	
	value= the value v for var that minimizes Conflicts(var,v,current,csp)
	
	set var=value in current
	
return failure.	

## Compile
g++ N_queen_minimum_conflict.cpp

./a.out

First assigment

1 0 0 0  row[0]=0

0 0 1 0  row[1]=2

0 0 0 1  row[2]=3

0 1 0 0  row[3]=1

Initial conflict value=1



Final assigment

0 0 1 0  row[0]=2

1 0 0 0  row[1]=0

0 0 0 1  row[2]=3

0 1 0 0  row[3]=1

Final conflict value=0, number of assigments=6
