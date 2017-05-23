#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#ifndef SHAPE_H
#define SHAPE_H
using namespace std;
class Graph;
class Shape {
public:
   Shape(unsigned id, int x1, int x2, int y1, int y2) : 
      _id(id), _xl(x1), _xr(x2), _yl(y1), _yu(y2) {}
   int xleft() const { return _xl; }
   int xright() const { return _xr; }
   int ylower() const { return _yl; }
   int yupper() const { return _yu; }
   unsigned id() const { return _id; }
   bool operator< (const Shape& s) { return this->_id < s->_id; }
   bool colorable() const;
   bool color() const;
   size_t connect(const Shape*);
   friend class Graph;
private:
   int               _xl;
   int               _xr;
   int               _yl;
   int               _yu;
   unsigned          _id;
   unsigned          _ref;
   vector<Shape*>    _neighbors;
};

struct Compare_Left {
   bool operator() (const Shape*, const Shape*) const
};
struct Compare_Right {
   bool operator() (const Shape*, const Shape*) const;
};
struct Compare_Upper {
   bool operator() (const Shape*, const Shape*) const;
};
struct Compare_Lower {
   bool operator() (const Shape*, const Shape*) const;
};
#endif
