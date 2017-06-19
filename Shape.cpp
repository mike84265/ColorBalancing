#include "Shape.h"
Shape::Shape(unsigned id, int x1, int x2, int y1, int y2)
   : _id(id), _xl(x1), _xr(x2), _yl(y1), _yu(y2), _ref(0), _color(UNCOLORED)
{}
size_t Shape::connect(Shape* s)
{
   _neighbors.push_back(s);
   return _neighbors.size();
}

void Shape::dfstravel(unsigned ref, vector<Shape*>& dfslist, bool& violate){
   if (!unvisited())
   	return;
   _ref = ref;
   vector<Shape*>::iterator it = _neighbors.begin();
   for (; it!=_neighbors.end(); it++){
      if((*it)->unvisited()){
         (*it)->dfstravel(_ref+1, dfslist, violate);
      }
      else if (!evenloop((*it)->ref())){
      	_color = UNCOLORABLE;
      	violate = true;
      }
   }
   dfslist.push_back(this);
}

void Shape::docolor(int i) {
   if (_color!=UNCOLORED)
      return;
   if (i==1){
      _color = RED;
      vector<Shape*>::iterator it = _neighbors.begin();
      for(; it!=_neighbors.end(); it++)
         (*it)->docolor(2);
   }
   else if (i==2){
      _color = BLUE;
      vector<Shape*>::iterator it = _neighbors.begin();
      for(; it!=_neighbors.end(); it++)
         (*it)->docolor(1);
   }
   else
      return;
}

Color Shape::inverse()
{
   if (_color == RED)
      _color = BLUE;
   else if (_color == BLUE)
      _color = RED;
   return _color;
}

int Shape::overlapArea(int xl, int yl, int xr, int yu)
{
   int xmin, xmax, ymin, ymax;
   xmin = (xl < this->_xl)? this->_xl : xl;
   xmax = (xr > this->_xr)? this->_xr : xr;
   ymin = (yl < this->_yl)? this->_yl : yl;
   ymax = (yu > this->_yu)? this->_yu : yu;
   if (xmin > xmax || ymin > ymax)
      return 0;
   else
      return (xmax-xmin) * (ymax-ymin);
}
