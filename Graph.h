#include <iostream>
#include <cstdlib>
#include "Shape.h"
#include <unordered_map>
#include <vector>
#ifndef GRAPH_H
#define GRAPH_H
using namespace std;
typedef multimap<int,Shape*> ShapeTable;
class Edge;
class Component;
class Window;
class Graph {
public:
   Graph(unsigned a, unsigned b, unsigned o) ;
   unsigned addShape(const string&);
private:
   // Helper functions:
   unsigned connect(Shape*);
   size_t insert(Shape*);
   // Member variables:
   ShapeTable                       _leftBound;
   ShapeTable                       _rightBound;
   ShapeTable                       _lowerBound;
   ShapeTable                       _upperBound;
   unsigned                         _globalref;
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
private:
   vector<Shape*>                   _shape;
   vector<Edge*>                    _edge;
};

class Edge {
public:
   Edge(const Shape*, const Shape*);
private:
   Shape*                           _s1;
   Shape*                           _s2;
};
#define GRAPH_H
