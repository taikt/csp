// Exam tabling schedule problem using GA and hill climbing


#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <map>
#include <string>
#include <ctime>
#include <cstdio>

using namespace std;

#define POP_SIZE 20 // Population Size
#define MAX_GENERATIONS 30 // Number of total generations before stopping

#define PROB_MUTATION 0.05 // Probability of a mutation ocurring
#define PROB_CLIMB 0.75 // Probability of a HC ocurring

#define HC_ITERATIONS 100 // Maximum iterations in the HC algorithm
#define MAX_FEASIBLE_RETRIES 30 // Maximum retries for a feasible solution

#define TOURNAMENT_SIZE 3 // Size of the tournaments for the selection algorithn

int total_exams;
string dataset_name; // Saves the dataset name to read the files

// The conflict matrix saves in each cell (i,j) the number of students
// with a conflict between the exams i and j
vector< vector<int> > conflicts;

// Represents a timetable
class Solution
{
private:
  // Assigns the timeslot passed by parameter to the exam that is in the genotype's location
  // exam.
  // It makes sure that the count of the exams in a timeslot is correct
  // @param exam The id of the exam which will be re-assigned
  // @param timeslot The id of the new timeslot to be assigned to the exam
  void assign_timeslot(int exam, int timeslot)
  {
    exams_in_timeslot[genotype.at(exam)]--;
    genotype.at(exam) = timeslot;
    exams_in_timeslot[timeslot]++;
  }

public:
  int aptitude; // The result of the evaluation function for the solution

  // Each gene in the genotype saves the Timeslot to which the exam is assigned
  vector<int> genotype;
  vector<int> used_timeslots;
  map<int, int> exams_in_timeslot; // map<Timeslot id, # of exams assigned>

  // The default constructor generates a random solution
  // and calculates its aptitude
  Solution()
  {
    for (int i = 0; i < total_exams; ++i)
    {
      // Fill every exam with it's own timeslot to generate a feasible solution
      int random;
      do {
        random = (int)(((float) total_exams-1)*rand()/(RAND_MAX + 1.0));
        genotype.push_back(random);
        if (is_feasible_up_to(i)) // Only end the loop when there is a feasible solution
          break;
        else
          genotype.pop_back(); // Remove the assigned exam: it made the solution unfeasible
      } while(true);
      exams_in_timeslot[random]++; // Count the exams assigned to each timeslot
    }

    recalculate_used_timeslots();
    calculate_aptitude();
  }

  // Calculates the solution aptitude (and sets it)
  void calculate_aptitude()
  {
    aptitude = 0;

    // Count the total used timeslots
    aptitude = used_timeslots.size();
  }

  // Checks every timeslot and checks if it's being
  // used by an exam. If it is, it's added to the
  // used_timeslots vector.
  void recalculate_used_timeslots()
  {
    used_timeslots.clear();

    for (map<int, int>::iterator timeslot=exams_in_timeslot.begin(); timeslot!=exams_in_timeslot.end(); ++timeslot)
      if (timeslot->second > 0)
        used_timeslots.push_back(timeslot->first);
  }

  // Returns true if the solution is feasible.
  bool is_feasible()
  {
    for (int i = 0; i < total_exams; ++i)
      for (int j = i + 1; j < total_exams; ++j)
        if (conflicts.at(i).at(j) > 0 && genotype.at(i) == genotype.at(j)) // If there is a conflict
          return false; // Solution not feasible

    return true;
  }

  // Returns true if the solution is feasible, considering
  // only one exam compared to the others
  // @param exam The id of the exam to be checked
  bool is_feasible(int exam)
  {
    for (int i = 0; i < total_exams; ++i) // Loop through all the exams
      if (exam != i &&
        conflicts.at(exam).at(i) > 0 &&
        genotype.at(exam) == genotype.at(i)) // If there is a conflict
        return false; // Solution not feasible

    return true; // Solution is feasible
  }

  // Returns true if the solution is feasible considering
  // the exams up to the passed exam
  // @param exam The id of the last exam that will be checked
  bool is_feasible_up_to(int exam)
  {
    for (int i = 0; i < (int) genotype.size(); ++i)
      if (i != exam && conflicts.at(exam).at(i) > 0 && genotype.at(exam) == genotype.at(i)) // If there is a conflict
        return false; // Solution not feasible

    return true; // Solution is feasible
  }

  // Mutates the current solution. It iterates for each exam,
  // and based on a probability it may or may not re-assign
  // the exam to a new timeslot.
  // It always keeps the solution feasible.
  void mutate()
  {
    for (int exam = 0; exam < total_exams; ++exam)
    {
      if ( ((1.0)*rand()/(RAND_MAX + 1.0)) < PROB_MUTATION) // Only mutate the exam on a probability
      {
        // Mutate until we have a feasible solution or the maximum retries have been iterated
        for (int retries = 0; retries < MAX_FEASIBLE_RETRIES; ++retries)
        {
          int prev_timeslot = genotype.at(exam);

          assign_timeslot(exam, (int)(( (float) total_exams-1 )*rand()/(RAND_MAX + 1.0)));
          if(is_feasible(exam))
            break;
          else // Undo the changes
          {
            assign_timeslot(exam, prev_timeslot);
          }
        };
      }
    }

    // Recalculate the aptitude for the new solution
    recalculate_used_timeslots();
    calculate_aptitude();
  }

  // Make the solution pass through a Hill Climbing algorithm to
  // improve it
  void hill_climb()
  {
    for (int i = 0; i < HC_ITERATIONS; ++i)
    {
      Solution candidate = *this;

      int var = (int)(((float) total_exams-1 )*rand()/(RAND_MAX + 1.0)); // Select a random exam to change

      // Iterate through every currently used timeslot until we find
      // one that improves the solution's aptitude
      for(vector<int>::iterator timeslot = used_timeslots.begin(); timeslot != used_timeslots.end(); ++timeslot)
      {
        candidate.assign_timeslot(var, *timeslot);
        candidate.recalculate_used_timeslots();
        candidate.calculate_aptitude();

        // If the new genotype has a better aptitude than the current best,
        // we make the swap in the current solution. We do not copy the candidate
        // over the current solution for performance reasons
        if (candidate.is_feasible(var) && candidate.aptitude < aptitude)
        {
          assign_timeslot(var, *timeslot);
          used_timeslots = candidate.used_timeslots;
          aptitude = candidate.aptitude;

          break; // We pass to the next HC iteration
        }
      }

    }
  }

  // Prints the solution and it's aptitude to stdout
  void print()
  {
    for (vector<int>::iterator exam = genotype.begin() ; exam != genotype.end(); ++exam)
    {
      cout << " " << *exam;
    }
    cout << " | " << aptitude << "\n";
  }
};

vector< Solution > population; // The main population

int get_total_exams();
void fill_conflicts();
void print_pop();
void generate_population();
void destroy_population();
void selection();
bool compare_solutions(const Solution a, const Solution b);
Solution select_best_solution();

int main(int argc, char* argv[])
{
  srand (time(NULL)); // Sets a random seed

  // Read the program arguments
  vector<string> params(argv, argv+argc);
  if (argc < 2) { // Missing arguments
    cerr << "Usage: " << argv[0] << " DATASET_NAME" << endl;
    return 1;
  }

  dataset_name = params.at(1);

  total_exams = get_total_exams();
  printf("total number of exams: %d\n",total_exams);

  fill_conflicts();

  // Start the clock for benchmarks
  const clock_t begin_time = clock();

  generate_population();

  Solution best = select_best_solution();

  // Iterate through every generation
  for (int cur_generation = 0; cur_generation < MAX_GENERATIONS; ++cur_generation)
  {

    // For every generation, print the best aptitude yet and the accumulated time
    cout << "Generation " << cur_generation << ", Best: " << best.aptitude << "\n";
    cout << "Elapsed time: " << float( clock () - begin_time ) /  (CLOCKS_PER_SEC/1000) << "ms\n";

    // Select the best solution for Elitism
    best = select_best_solution();

    // Make a selection of the current population to generate the next one
    selection();

    // Create a new population iterating through the selected individuals,
    // and applying the operators to them
    vector< Solution > new_population;
    for (int i = 0; i < POP_SIZE - 1; ++i)
    {
      if ( ((1.0)*rand()/(RAND_MAX + 1.0)) < PROB_MUTATION)
        population.at(i).mutate();

      if ( ((1.0)*rand()/(RAND_MAX + 1.0)) < PROB_CLIMB)
        population.at(i).hill_climb();

      new_population.push_back(population.at(i));
    }

    // Elitism, keep the best solution from the last generation
    new_population.push_back(best);

    // The new population becomes the main population
    population = new_population;
  }

  best = select_best_solution(); // Select the final best solution

  cout << "Time used: " << float( clock () - begin_time ) /  (CLOCKS_PER_SEC/1000) << "ms\n";
  cout << "Best solution: ";
  best.print();

  return 0;
}

// Makes a Tournament-style selection on the current population,
// modifying it to contain the selected individuals
void selection()
{
  vector< Solution > new_population;
  vector< Solution > selected_solutions;

  // Generate POP_SIZE individuals
  for (int i = 0; i < POP_SIZE; ++i)
  {
    selected_solutions.clear();

    // Select TOURNAMENT_SIZE individuals to compete
    for (int j = 0; j < TOURNAMENT_SIZE; ++j)
    {
      // Select a random solution from the population
      selected_solutions.push_back(population.at((int)(((float) POP_SIZE - 1)*rand()/(RAND_MAX + 1.0))));
    }

    // Select the best solution from the selected ones
    sort(selected_solutions.begin(), selected_solutions.end(), compare_solutions);

    // And add it to the new population
    new_population.push_back(selected_solutions.front());
  }

  // The new population is now the main one
  population = new_population;
}

// Compares two solutions
// @param a, b The solutions to compare
// @return true if solution a is better (or equal) than b
bool compare_solutions(const Solution a, const Solution b)
{
  return a.aptitude <= b.aptitude;
}

// Prints the current population
void print_pop()
{
  for (vector< Solution >::iterator solution = population.begin() ; solution != population.end(); ++solution)
    cout << (*solution).aptitude << " ";
  cout << "\n";
}

// Selects the best solution from the current population
// @return The currently best solution
Solution select_best_solution()
{
  Solution *best = &(*population.begin());

  for (vector< Solution >::iterator solution = population.begin() + 1; solution != population.end(); ++solution)
    if ((*solution).aptitude < best->aptitude)
      best = &(*solution);

  return *best;
}

// Fills the conflict matrix, reading from the .stu file
void fill_conflicts()
{
  conflicts.resize(total_exams);

  // Initializes the matrix
  for (int i = 0; i < total_exams; ++i)
    conflicts[i].resize(total_exams);

  // Read the students file to fill the conflicts matrix
  string filename = dataset_name + ".stu";
  ifstream infile(filename.c_str());

  if(!infile.is_open()) {
    cout << "Error reading file" << endl;
    exit(0);
  }

  // We parse every line of the file, filling the conflict matrix
  string line;

  while (getline(infile, line))
  {

    // Tokenize the line and mark as conflicts every pair of exams
    // that appears in the same line
    stringstream strstr(line);
    istream_iterator<string> it(strstr);
    istream_iterator<string> end;
    vector<string> exams(it, end);

    // Iterate on all the remaining exams in the line, after the current one
    // and mark the couple (current, next) and (next, current) as conflicted
    for (vector<string>::iterator exam = exams.begin() ; exam != exams.end(); ++exam)
      for (vector<string>::iterator other_exam = exam + 1; other_exam != exams.end(); ++other_exam)
      {
        conflicts[atoi((*exam).c_str()) - 1][atoi((*other_exam).c_str()) - 1]++;
        conflicts[atoi((*other_exam).c_str()) - 1][atoi((*exam).c_str()) - 1]++;
      }
  }

  infile.close();
}

// Calculate the total number of exams by reading the .crs file
// @return The total number of exams
int get_total_exams()
{
  // Read the .crs file
  string filename = dataset_name + ".crs";
  ifstream exams_file(filename.c_str());
  if(!exams_file.is_open()) {
    cout << "Error reading file" << endl;
    exit(0);
  }

  // Count the newlines to get the total of exams
  exams_file.unsetf(ios_base::skipws); // Don't skip newlines
  unsigned total_exams = count(
                           istream_iterator<char>(exams_file),
                           istream_iterator<char>(),
                           '\n');
  exams_file.close();

  return total_exams;
}

// Generates a population of random (feasible) solutions
void generate_population()
{
  population.resize(POP_SIZE);

  for (vector< Solution >::iterator solution = population.begin() ; solution != population.end(); ++solution)
    *solution = Solution();
}