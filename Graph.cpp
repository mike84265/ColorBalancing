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

   ShapeTable::iterator it_left_l = _leftBound.lower_bound(s->xright());
   ShapeTable::iterator it_left_u = _leftBound.upper_bound(s->xright()+_alpha);

   ShapeTable::iterator it_right_l = _rightBound.lower_bound(s->xleft()-_alpha);
   ShapeTable::iterator it_right_u = _rightBound.upper_bound(s->xleft());

   ShapeTable::iterator it_upper_l = _upperBound.lower_bound(s->ylower()-_beta);
   ShapeTable::iterator it_upper_u = _upperBound.upper_bound(s->ylower());

   ShapeTable::iterator it_lower_l = _lowerBound.lower_bound(s->yupper());
   ShapeTable::iterator it_lower_u = _lowerBound.upper_bound(s->yupper()+_beta);

   unsigned int _numofconnects = 0;
   for(ShapeTable::iterator it = it_left_l;it!=it_left_u;it++)
   {
     if(it->second!=s && s->yupper()>it->second->ylower() && it->second->yupper()>s->ylower())
     {
      if(abs(s->xright()-it->first)<=_alpha)
      {
        s->connect(it->second);
        Edge* con=new Edge(s,it->second);
        _edge.push_back(con);
        _numofconnects ++;
      }
     }
   }

   for(ShapeTable::iterator it = it_right_l;it!=it_right_u;it++)
   {
     if(it->second!=s && s->yupper()>it->second->ylower() && it->second->yupper()>s->ylower())
     {
       if(abs(s->xleft()-it->first)<=_alpha)
       {
        s->connect(it->second);
        Edge* con=new Edge(s,it->second);
        _edge.push_back(con);
        _numofconnects++;
        }
     }
   }

   for(ShapeTable::iterator it=it_upper_l;it!=it_upper_u;it++)
   {
     if(it->second!=s && s->xright()>it->second->xleft() && it->second->xright()>s->xleft())
     {
       if(abs(s->ylower()-it->first)<=_beta)
       {
        s->connect(it->second);
        Edge* con=new Edge(s,it->second);
        _edge.push_back(con);
        _numofconnects++;
       }
     }
   }

   for(ShapeTable::iterator it=it_lower_l;it!=it_lower_u;it++)
   {
     if(it->second!=s && s->xright()>it->second->xleft() && it->second->xright()>s->xleft())
     {
       if((s->yupper()-it->first)<=_beta)
       {
        s->connect(it->second);
        Edge* con=new Edge(s,it->second);
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
         (*it)->nocolor();
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

