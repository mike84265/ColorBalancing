#include "Shape.h"
Shape::Shape(unsigned id, int x1, int x2, int y1, int y2)
   : _id(id), _xl(x1), _xr(x2), _yl(y1), _yu(y2), _ref(0), _color(UNCOLORED)
{}
size_t Shape::connect(Shape* s)
{
   _neighbors.push_back(s);
   return _neighbors.size();
}
