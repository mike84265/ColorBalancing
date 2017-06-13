#include "Graph.h"

unsigned Graph::_globalref = 0;

int Graph::read(const char* filename)
{
   // Input Argument:
   //    filename: The name of the input file.
   // Return value:
   //    Number of shapes read in. Return negative if errors happen.
   // Specification:
   //    1. Parse the line to get four bounds of the shape
   //    2. Call connect() to connect the current read shape with the others in the graph
   //    3. Insert the shape to bound list.

   FILE* fin = fopen(filename, "r");
   if (fin == NULL)
      return -1;
   fscanf(fin,"ALPHA=%u\n",&_alpha);
   fscanf(fin,"BETA=%u\n",&_beta);
   fscanf(fin,"OMEGA=%u\n",&_omega);

   int x1,y1,x2,y2;
   unsigned id = 1;
   while (fscanf(fin,"%d,%d,%d,%d\n",&x1, &y1, &x2, &y2) == 4) {
      Shape* s = new Shape(id,x1,x2,y1,y2);
      ++id;
      connect(s);
      _shape.push_back(s);
      _leftBound.insert(pair<int,Shape*>(x1,s));
      _rightBound.insert(pair<int,Shape*>(x2,s));
      _upperBound.insert(pair<int,Shape*>(y1,s));
      _lowerBound.insert(pair<int,Shape*>(y2,s));
   }
   fclose(fin);
   return id-1;
}

void Graph::DFScoloring()
{
   // Arguments that should be used:
   //    _shape
   //    _globalref
   //    _leftBound, _rightBound, _lowerBound, _upperBound
   //    _component
   // Specification:
   //    1. Update _globalref
   //    2. Run DFS to rule out odd cycles and find connected components.
   vector<Shape*>::iterator it = _shape.begin();
   for(; it!=_shape.end(); it++){
      bool violate = false;
      _globalref = 1;
      vector<Shape*> dfslist;
      (*it)->dfstravel(_globalref, dfslist, violate);
      if (!dfslist.empty()){
         Component* temp = new Component(dfslist);
         temp->coloring(violate);
         _component.push_back(temp);
      }
   }
}

unsigned Graph::connect(Shape* s)
{
   // Input Argument:
   //    s: The shape that should be connected to the others in the graph
   // Return Value:
   //    Number of shape connected
   // Specification:
   //    1. Compare vertical overlay
   //    2. Compare horizontal overlay
   //    3. Add to _edge if connection is established
   if(_leftBound.empty()){return 0;}

   ShapeTable::iterator it_left = _leftBound.begin();
   ShapeTable::iterator it_right = _rightBound.begin();
   ShapeTable::iterator it_upper = _upperBound.begin();
   ShapeTable::iterator it_lower = _lowerBound.begin();

   unsigned int _numofconnects = 0;
   for(;it_left!=_leftBound.end();it_left++)
   {
     if(it_left->second!=s && s->yupper>it_left()->second.ylower() && it_left->second.yupper()>s->ylower() )
     {
      if(abs(s.xright()-it_left->first)<=_alpha)
      {
        s.connect(it_left->second);
        Edge* con(s,it_left->second);
        _edge.push_back(con);
        _numofconnects ++;
      }
     }
   }

   for(;it_right!=_rightBound.end();it_right++)
   {
     if(it_left->second!=s && s->yupper>it_right()->second.ylower() && it_right->second.yupper()>s->ylower())
     {
       if(abs(s.xleft()-it_right->first)<=_alpha)
       {
        s.connect(it_right->second);
        Edge* con(s,it_right->second);
        _edge.push_back(con);
        _numofconnects++;
        }
     }
   }

   for(;it_upper!=_upperBound.end();it_upper++)
   {
     if(it_upper->second!=s && s.xright()>it_upper()->second.xleft() && it_upper->second.xright()>s.xleft())
     {
       if(abs(s.ylower()-it_upper->first)<=_beta)
       {
        s.connect(it_upper->second);
        Edge* con(s,it_upper->second);
        _edge.push_back(con);
        _numofconnects++;
       }
     }
   }

   for(;it_lower!=_lowerBound.end();it_lower++)
   {
     if(it_lower->second!=s && s.xright()>it_lower()->second.xleft() && it_lower->second.xright()>s.xleft())
     {
       if((s.yupper()-it_lower->first)<=_beta)
       {
        s.connect(it_lower->second);
        Edge* con(s,it_lower->second);
        _edge.push_back(con);
        _numofconnects++;
        }
     }
   }
   return _numofconnects;
}

void Component::coloring(bool violate){
   if (violate){
      vector<Shape*>::iterator it = _shape.begin();
      for(; it!=_shape.end(); it++){
         *it->nocolor();
      }
   }
   else
      _shape[0]->docolor(1);
}

Edge::Edge(Shape* s1, Shape* s2)
{
   if (s1->id() <= s2->id()) {
      _s[0] = s1; _s[1] = s2;
   } else {
      _s[0] = s2; _s[1] = s1;
   }
}

bool Edge::operator< (const Edge& e) const
{
   if (this->_s[0]->id() != e._s[0]->id())
      return this->_s[0]->id() < e._s[0]->id();
   else
      return this->_s[1]->id() < e._s[1]->id();
}

