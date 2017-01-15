 

# CSP (Contraint Satisfaction Problem) solvers in AI (Artificial Intelligence)

pseudo code
## 1. Backtracking algorithm
function BACKTRACKING-SEARCH(csp) returns a solution, or failure

    return BACKTRACK({ }, csp)
-------------------------------

function BACKTRACK(assignment, csp) returns a solution, or failure

    if assignment is complete then return assignment

    var ← SELECT-UNASSIGNED-VARIABLE(csp)

    for each value in ORDER-DOMAIN-VALUES(var, assignment, csp) do

       if value is consistent with assignment then

           add {var = value} to assignment

           inferences ← INFERENCE(csp, var, value)

           if inferences != failure then

               add inferences to assignment

               result ← BACKTRACK(assignment, csp)

               if result != failure then

                  return result

       remove {var = value} and inferences from assignment

    return failure

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
## 3. Genetic alogrithm
function GENETIC-ALGORITHM(population, FITNESS-FN) returns an individual

    inputs: population, a set of individuals
  
	FITNESS-FN, a function that measures the fitness of an individual

    repeat
     new population ← empty set
     
     for i = 1 to SIZE(population) do
     
         x ← RANDOM-SELECTION(population, FITNESS-FN)
     
         y ← RANDOM-SELECTION(population, FITNESS-FN)
     
         child ← REPRODUCE(x , y)
	 
         if (small random probability) then child ← MUTATE(child)
	 
         add child to new population
	 
     population ← new population
     
    until some individual is fit enough, or enough time has elapsed
  
    return the best individual in population, according to FITNESS-FN
  
--------------------

function REPRODUCE(x , y) returns an individual

    inputs: x , y, parent individuals
   
    n ← LENGTH(x ); c ← random number from 1 to n
   
    return APPEND(SUBSTRING(x , 1, c), SUBSTRING(y, c + 1, n))




