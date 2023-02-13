//***************************************************************************
//
//  Darrius White
//  Z1900146
//  Assign 6
//  CSCI 463-1
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************/
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <unistd.h>

constexpr int rows = 1000;      ///< the number of rows in the work matrix
constexpr int cols = 100;       ///< the number of cols in the work matrix

std::mutex stdout_lock;         ///< for serializing access to stdout

std::mutex counter_lock;        ///< for dynamic balancing only
volatile int counter = rows;    ///< for dynamic balancing only

std::vector<int> tcount;        ///< count of rows summed for each thread
std::vector<uint64_t> sum;      ///< the calculated sum from each thread

int work[rows][cols];           ///< the matrix to be summed



/**
 * usage function shows user valid command line usage and arguments to use
 * within a standard error message.
 **********************************************************************************************/
static void usage()
{
  std::cerr << "Usage: reduce [-d] [-t num]" << std::endl;
  std::cerr << "    -d use dynamic load balancing" << std::endl;
  std::cerr << "    -t specifies the number of threads to use" << std::endl;
  exit(1);
}



/**
 * sum_static() function uses static load balancing to determine what rows each
 * thread will process.
 *
 * @param tid, specific thread id.
 * @param num_threads, number of threads.
 **********************************************************************************************/
void sum_static(int tid, int num_threads)
{
  //Stating which thread is beginning to process data
  stdout_lock.lock();
  std::cout << "Thread " << tid << " starting" << std::endl;
  stdout_lock.unlock();

  int row = 0;
  int ctr = 0;
  bool done = false;

  while (!done)
  {
    //setting next row to sum
    if(ctr != 0)
       row += num_threads;

    ctr++;

    //check if row is within the given range
    if ( row >= rows)
       done = true;

    if (!done)
    {
      uint64_t col_sum = 0;

      for (int i = 0; i < cols; i++)
         col_sum += work[row][i];//adding each column together in a specific row

      sum[tid] += col_sum; //calculated sum of each row a specific thread processes
      tcount[tid]++; //increment count of rows this thread has summed
    }
  }

  //Print sum of row processed by given thread
  stdout_lock.lock();
  std::cout << "Thread " << tid << " ending tcount=" << tcount[tid]
	    << " sum=" << sum[tid] << std::endl;
  stdout_lock.unlock();
}



/**
 * sum_dynamic() function uses dynamic load balancing to determine
 * what rows each thread will process.
 *
 * @param tid, given thread id.
 **********************************************************************************************/
void sum_dynamic(int tid)
{
  //Stating which thread is beginning to process data
  stdout_lock.lock();
  std::cout << "Thread " << tid << " starting" << std::endl;
  stdout_lock.unlock();

  bool done  = false;

  while (!done)
  {
    int ctr_copy;

    counter_lock.lock();
    {
      if (counter > 0)
         counter--;
      else
         done = true;

      ctr_copy = counter; //Copy the counter
    }
    counter_lock.unlock();

    if (!done)
    {
      uint64_t col_sum = 0;

      for (int i = 0; i < cols; i++)
         col_sum += work[ctr_copy][i]; //add all columns together in a row

      sum[tid] += col_sum; //calculated sum of each row a specific thread processes
      tcount[tid]++; //increment count of rows this thread has summed
    }
  }

  //Print sum of row processed by given thread
  stdout_lock.lock();
  std::cout << "Thread " << tid << " ending tcount=" << tcount[tid]
            << " sum=" << sum[tid] << std::endl;
  stdout_lock.unlock();
}



/**
 * main() function accepts command-line parameters to use for static or dynamic loading.
 *
 * @param argc, argument count.
 * @param argv, argument values.
 *
 **********************************************************************************************/
int main(int argc, char **argv)
{
  int opt;
  bool commnd_D = false;
  unsigned int num_threads = 2;
  std::vector<std::thread*> threads;

  while((opt = getopt(argc, argv, "dt:")) != -1)
  {
    switch(opt)
    {
      case 'd': //Dynamic load balancing
        commnd_D = true;

      break;

      case 't': //amount of threads to use
        num_threads = atoi(optarg);

        if (num_threads >= std::thread::hardware_concurrency()) //if num of threads entered by user is greater than amount allowed
           num_threads = std::thread::hardware_concurrency(); //default num of threads

      break;

      default:
        usage();
    }
  }

  //Initialize global work matrix
  srand(0x1234); //Seed the random number generator
  for (int i = 0; i < cols; i++)
  {
    for (int j = 0; j < rows; j++)
       work[j][i] = rand();
  }

  //Print num of cores in system
  std::cout << std::thread::hardware_concurrency() << " concurrent threads supported." << std::endl;

  //Resize global vectors to num of threads
  tcount.resize(num_threads, 0);
  sum.resize(num_threads, 0);

  //If dynamic load balance
  if (commnd_D)
  {
    //Add threads to vector and call dynamic load balancing
    for (unsigned int i = 0; i < num_threads; i++)
       threads.push_back(new std::thread(sum_dynamic, i));
  }
  else //Static load balancing
  {
    //Add threads to vector and call static load balancing
    for (unsigned int i = 0; i < num_threads; i++)
       threads.push_back(new std::thread(sum_static, i, num_threads));
  }

  int work_total = 0;
  //Calculate total work threads have done and join them
  for (unsigned int i = 0; i < num_threads; i++)
  {
    threads.at(i)->join();
    delete threads.at(i);
    work_total += tcount.at(i);
  }

  //Total sum of threads
  uint64_t gross_sum = 0;

  for (unsigned int i = 0; i < num_threads; i++)
    gross_sum += sum[i];

  //Print total work and total sum
  std::cout << "main() exiting, total_work=" << work_total
	    << " gross_sum=" << gross_sum << std::endl;
  return 0;
}
