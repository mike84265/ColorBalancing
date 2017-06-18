#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include "Shape.h"
#include "Graph.h"
using namespace std;
#define ERR_EXIT  \
{              \
   fprintf(stderr, "usage: %s ($input_filename) ($input_filaname).out\n",argv[0]); \
   exit(2);    \
}     
int main(int argc, char** argv)
{
   // 1. Read the file from argv
   // 2. Read alpha, beta, omega.
   // 3. Construct the graph
   // 4. Call Graph::addShape() to read and add shapes to the graph
   // 5. Call Graph::DFScoloring() to rule out odd edges
   if (argc < 3)
      ERR_EXIT
   char* p = strstr(argv[2], argv[1]);
   if (p == NULL || p != argv[2])
      ERR_EXIT
   p = strstr(argv[2], ".out");
   if (p == NULL || strcmp(p,".out") != 0)
      ERR_EXIT
   Graph graph;
   graph.read(argv[1]);
   return 0;
}
