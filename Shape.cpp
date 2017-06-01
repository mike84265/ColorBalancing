#include "Shape.h"
Shape::Shape(unsigned id, int x1, int x2, int y1, int y2)
   : _id(id), _xl(x1), _xr(x2), _yl(y1), _yu(y2), _ref(0), _color(UNCOLORED)
{}
bool Compare_Left::operator() (const Shape* a, const Shape* b) const
{
   if (a->xleft() != b->xleft() )
      return a->xleft() < b->xleft();
   else
      return a->id() < b->id();
}

bool Compare_Right::operator() (const Shape* a, const Shape* b) const
{
   if (a->xright() != b->xright() )
      return a->xright() < b->xright();
   else
      return a->id() < b->id();
}

bool Compare_Lower::operator() (const Shape* a, const Shape* b) const
{
   if (a->ylower() != b->ylower() )
      return a->ylower() < b->ylower();
   else
      return a->id() < b->id();
}

bool Compare_Upper::operator() (const Shape* a, const Shape* b) const
{
   if (a->yupper() != b->yupper() )
      return a->yupper() < b->yupper();
   else
      return a->id() < b->id();
}

size_t Shape::connect(Shape* s)
{
   _neighbors.push_back(s);
   return _neighbors.size();
}
