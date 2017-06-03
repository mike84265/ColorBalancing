#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#ifndef SHAPE_H
#define SHAPE_H
using namespace std;
enum Color {
   UNCOLORED = 0,
   RED = 1,
   BLUE = 2,
   UNCOLORABLE = 3
};

class Graph;
class Shape {
public:
   Shape(unsigned id, int x1, int x2, int y1, int y2);
   int xleft() const { return _xl; }
   int xright() const { return _xr; }
   int ylower() const { return _yl; }
   int yupper() const { return _yu; }
   unsigned id() const { return _id; }
   bool operator< (const Shape& s) { return this->_id < s._id; }
   Color color() const { return _color; }
   size_t connect(Shape* s);
   friend class Graph;
private:
   unsigned          _id;
   int               _xl;
   int               _xr;
   int               _yl;
   int               _yu;
   unsigned          _ref;
   Color             _color;
   vector<Shape*>    _neighbors;
};

#endif
