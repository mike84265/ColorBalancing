#include "Shape.h"
bool Compare_Left::operator() (const Shape* a, const Shape* b) const
{
   if (a->xleft() != b->xleft() )
      return a->xleft() < b->xleft;
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
