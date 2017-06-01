#include "Graph.h"
Graph::Graph(unsigned a, unsigned b, unsigned o)
{
   // 1. Initialize the given alpha, beta, omega
   // 2. Initialize the multimap. (functional object should be passed to the map)
}

unsigned Graph::addShape(const string& str)
{
   // Input Argument:
   //    str: A line containing the 4 coordinates of the shape.
   // Return value:
   //    Number of edges connected.
   // Specification:
   //    1. Parse the line to get four bounds of the shape
   //    2. Call connect() to connect the current read shape with the others in the graph
   //    3. Insert the shape to bound list.
}

void Graph::DFScoloring()
{
   // Arguments that should be used:
   //    _shape 
   //    _globalref
   //    _leftBound, _rightBound, _lowerBound, _upperBound
   //    _component
   // Specification:
   //    1. Update _globalref
   //    2. Run DFS to rule out odd cycles and find connected components.
}

unsigned Graph::connect(Shape* s)
{
   // Input Argument:
   //    s: The shape that should be connected to the others in the graph
   // Return value:
   //    Number of shape connected
   // Specification:
   //    1. Compare vertical overlay
   //    2. Compare horizontal overlay
   //    3. Add to _edge if connection is established
}

Edge::Edge(Shape* s1, Shape* s2)
{
   if (s1->id() <= s2->id()) {
      _s[0] = s1; _s[1] = s2;
   } else {
      _s[0] = s2; _s[1] = s1;
   }
}

bool Edge::operator< (const Edge& e) const
{
   if (this->_s[0]->id() != e._s[0]->id())
      return this->_s[0]->id() < e._s[0]->id();
   else
      return this->_s[1]->id() < e._s[1]->id();
}
