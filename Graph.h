#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "Shape.h"
#include <unordered_map>
#include <map>
#include <vector>
#include <climits>
#ifndef GRAPH_H
#define GRAPH_H
using namespace std;
typedef multimap<int,Shape*> ShapeTable;
class Edge;
class Component;
class Window;
class Graph {
public:
   Graph() {}
   int read(const char* filename);
   void DFScoloring();
   void colorBalance();
private:
   // Helper functions:
   unsigned connect(Shape* s);
   // Member variables:
   ShapeTable                       _leftBound;
   ShapeTable                       _rightBound;
   ShapeTable                       _lowerBound;
   ShapeTable                       _upperBound;
   unsigned static                  _globalref;
   size_t                           _numShape;
   unsigned                         _alpha;
   unsigned                         _beta;
   unsigned                         _omega;
   vector<Shape*>                   _shape;
   vector<Edge*>                    _edge;
   vector<Component*>               _component;
};

class Component {
public:
   Component(vector<Shape*> temp):_shape(temp){};
   void coloring(bool){};
private:
   int                              _colorDiff;
   vector<Shape*>                   _shape;
   vector<Edge*>                    _edge;
};

class Edge {
public:
   Edge(Shape*, Shape*);
   bool operator< (const Edge& e) const;
private:
   Shape*                           _s[2];
};
#endif
