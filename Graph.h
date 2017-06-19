#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "Shape.h"
#include <unordered_map>
#include <map>
#include <vector>
#include <climits>
#include <cassert>
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
   ~Graph();
   int read(const char* filename);
   void DFScoloring();
   void colorBalance();
   void separate();
   void PrintOut(const char* filename);
private:
   // Helper functions:
   unsigned connect(Shape* s);
   // Member variables:
   ShapeTable                       _leftBound;
   ShapeTable                       _rightBound;
   ShapeTable                       _lowerBound;
   ShapeTable                       _upperBound;
   unsigned static                  _globalref;
   unsigned                         _alpha;
   unsigned                         _beta;
   unsigned                         _omega;
   vector<Shape*>                   _shape;
   vector<Edge*>                    _edge;
   vector<Component*>               _component;
   vector<Window*>                  _window;
};

class Component {
public:
   Component(vector<Shape*> temp);
   void coloring(bool);
   void inverse();
   bool colorable();
   void range(int& xl, int& yl, int& xr, int& yu);
   void pushWindow(Window* window);
   friend class Window;
   void printGroup(FILE* file);
private:
   vector<Shape*>                   _shape;
   vector<Window*>                  _window;
   int                              _xl;
   int                              _xr;
   int                              _yl;
   int                              _yu;
};

class Edge {
public:
   Edge(Shape*, Shape*);
   bool operator< (const Edge& e) const;
private:
   Shape*                           _s[2];
};

class Window {
public:
   Window(unsigned id, int xl, int yd, int xr, int yu);
   int colorDiff() const { return _colorDiff; }
   bool overlap(int xl, int yl, int xr, int yu);
   void pushComponent(Component* comp);
   int calculateDiff();
   void refreshShape();
   void adjust();
   bool adjusted() const { return _adjusted; }
   void getSides(int& a,int& b,int& c,int& d){ a=_xl;b=_xr;c=_yl;d=_yu; }
   int getArea(int index);
private:
   vector<Component*>               _component;
   vector<Shape*>                   _shape;
   vector<int>                      _area;
   unsigned                         _id;
   int                              _xl;
   int                              _xr;
   int                              _yl;
   int                              _yu;
   int                              _colorDiff;
   int                              _redarea;
   int                              _bluearea;
   bool                             _adjusted;
};
#endif
