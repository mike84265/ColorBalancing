#include "Shape.h"
Shape::Shape(unsigned id, int x1, int x2, int y1, int y2)
   : _id(id), _xl(x1), _xr(x2), _yl(y1), _yu(y2), _ref(0), _color(UNCOLORED)
{}
size_t Shape::connect(Shape* s)
{
   _neighbors.push_back(s);
   return _neighbors.size();
}

void dfstravel(unsigned ref, vector<Shape*>& dfslist, bool& violate){
   if (!unvisited())
   	return;
   _ref = ref;
   vector<Shape*>::iterator it = _neighbors.begin();
   for (; it!=_neighbors.end(); it++){
      if(*it->unvisited()){
         *it->dfstravel(_ref+1, dfslist);
      }
      else if (!evenloop(*it->ref())){
      	_color = UNCOLORABLE;
      	violate = true;
      }
   }
   dfslist.push_back(this);
}

void docolor(int i) {
   if (_color!=UNCOLORED)
      return;
   if (i==1){
      _color = RED;
      vector<Shape*>::iterator it = _neighbors.begin();
      for(; it!=_neighbors.end(); it++)
         *it->docolor(2);
   }
   else if (i==2){
      _color = BLUE;
      vector<Shape*>::iterator it = _neighbors.begin();
      for(; it!=_neighbors.end(); it++)
         *it->docolor(1);
   }
   else
      return;
}