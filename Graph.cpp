#include "Graph.h"
#include <fstream>

unsigned Graph::_globalref = 0;

Graph::~Graph()
{
   size_t i,n;
   for (i=0,n=_shape.size();i<n;++i)
      delete _shape[i];
   for (i=0,n=_component.size();i<n;++i)
      delete _component[i];
   for (i=0,n=_window.size();i<n;++i)
      delete _window[i];
}

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
      _shape.push_back(s);
      _leftBound.insert(pair<int,Shape*>(x1,s));
      _rightBound.insert(pair<int,Shape*>(x2,s));
      _upperBound.insert(pair<int,Shape*>(y2,s));
      _lowerBound.insert(pair<int,Shape*>(y1,s));
   }
   for(int i=0;i<_shape.size();i++)
      connect(_shape[i]);

   fclose(fin);

   return id-1;
}

void Graph::separate()
{
   int upmost=0;
   ShapeTable::iterator it1 = _upperBound.end();
   it1--;
   for(ShapeTable::iterator it=it1;it!=_upperBound.begin();it--)
   {
     if(it->second->color()!=3)
     {
      upmost = it->first;
      break;
     }
   }
   int lowmost=0;
   ShapeTable::iterator it2 = _lowerBound.begin();
   for(ShapeTable::iterator it=it2;it!=_lowerBound.end();it++)
   {
     if(it->second->color()!=3)
     {
      lowmost = it->first;
       break;
     }
   }
   int leftmost=0;
   ShapeTable::iterator it3 = _leftBound.begin();
   for(ShapeTable::iterator it=it3;it!=_leftBound.end();it++)
   {
     if(it->second->color()!=3)
     {
      leftmost = it->first;
       break;
     }
   }
   int rightmost=0;
   ShapeTable::iterator it4 = _rightBound.end();
   it4--;
   for(ShapeTable::iterator it=it4;it!=_rightBound.begin();it--)
   {
     if(it->second->color()!=3)
     {
      rightmost = it->first;
       break;
     }
   }

   int height = (upmost-lowmost)/_omega
                + bool((upmost-lowmost)%_omega);
   int width = (rightmost-leftmost)/_omega
                + bool((rightmost-leftmost)%_omega);


   for(int i =0; i<height; i++){
      for(int j=0; j<width; j++){
       Window* temp;
         if(i==height-1)
         {
            if (j==width-1)
             temp = new Window(i*width+j, rightmost-_omega, upmost-_omega, rightmost, upmost);
            else
             temp = new Window(i*width+j, leftmost+j*_omega, upmost-_omega, leftmost+(j+1)*_omega, upmost);
         }
         else if (j==width-1)
           temp = new Window(i*width+j, rightmost-_omega, lowmost+i*_omega, rightmost, lowmost+(i+1)*_omega);
         else
           temp = new Window(i*width+j, leftmost+j*_omega, lowmost+i*_omega, leftmost+(j+1)*_omega, lowmost+(i+1)*_omega);
         _window.push_back(temp);

      }
   }
   size_t numComp = _component.size();
   size_t numWindow = _window.size();
   for (size_t i=0;i<numComp;++i) {
      int xl,yl,xr,yu;
      _component[i]->range(xl,yl,xr,yu);
      for (size_t j=0;j<numWindow;++j) {
         if (_window[j]->overlap(xl,yl,xr,yu)) {
            _window[j]->pushComponent(_component[i]);
            _component[i]->pushWindow(_window[j]);
         }
      }
   }
   for (size_t i=0;i<numWindow;++i)
      _window[i]->refreshShape();
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
        s->connect(it->second);
        Edge* con=new Edge(s,it->second);
        _edge.push_back(con);
        _numofconnects ++;
     }
   }

   for(ShapeTable::iterator it = it_right_l;it!=it_right_u;it++)
   {
     if(it->second!=s && s->yupper()>it->second->ylower() && it->second->yupper()>s->ylower())
     {
        s->connect(it->second);
        Edge* con=new Edge(s,it->second);
        _edge.push_back(con);
        _numofconnects++;
     }
   }

   for(ShapeTable::iterator it=it_upper_l;it!=it_upper_u;it++)
   {
     if(it->second!=s && s->xright()>it->second->xleft() && it->second->xright()>s->xleft())
     {
        s->connect(it->second);
        Edge* con=new Edge(s,it->second);
        _edge.push_back(con);
        _numofconnects++;
     }
   }

   for(ShapeTable::iterator it=it_lower_l;it!=it_lower_u;it++)
   {
     if(it->second!=s && s->xright()>it->second->xleft() && it->second->xright()>s->xleft())
     {
        s->connect(it->second);
        Edge* con=new Edge(s,it->second);
        _edge.push_back(con);
        _numofconnects++;
     }
   }
   return _numofconnects;
}

void Graph::PrintOut(const char* filename)
{
   FILE* f = fopen(filename,"w");
   for(int j=0;j<_window.size();j++)
   {
      int left=-1,right=-1,lower=-1,upper=-1;
      _window[j]->getSides(left,right,lower,upper);
      fprintf(f,"WIN[%d]=%d,%d,%d,%d(1.11 1.11)",j+1,left,lower,right,upper);
   }
   for(int i=0;i<_component.size();i++)
      _component[i]->printGroup(f);
   fclose(f);
}

void Graph::colorBalance()
{
   size_t numWindow = _window.size();
   int maxWinId, maxWinDiff=0;
   for (size_t i=0;i<numWindow;++i) {
      if (abs(_window[i]->colorDiff()) > maxWinDiff && !_window[i]->adjusted()) {
         maxWinId = i;
         maxWinDiff = abs(_window[i]->colorDiff());
      }
   }
   _window[maxWinId]->adjust();
}

Component::Component(vector<Shape*> temp) :
   _shape(temp), _xl(INT_MAX), _xr(INT_MIN), _yl(INT_MAX), _yu(INT_MIN)
{
   size_t n = _shape.size();
   for (size_t i=0;i<n;++i) {
      _shape[i]->setComponent(this);
      if (_shape[i]->xleft() < _xl)
         _xl = _shape[i]->xleft();
      if (_shape[i]->xright() > _xr)
         _xr = _shape[i]->xright();
      if (_shape[i]->ylower() < _yl)
         _yl = _shape[i]->ylower();
      if (_shape[i]->yupper() > _yu)
         _yu = _shape[i]->yupper();
   }
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

void Component::inverse()
{
   size_t n = _shape.size();
   for (size_t i=0;i<n;++i)
      _shape[i]->inverse();
   n = _window.size();
   for (size_t i=0;i<n;++i)
      _window[i]->calculateDiff();
}
void Component::printGroup(FILE* file)
{
   fprintf(file,"GROUP\n");
   int a_order=1;
   int b_order=1;
   int c_order=1;
   for (size_t i=_shape.size()-1;i>-1;--i) {
      switch(_shape[i]->color()) {
       case UNCOLORABLE:
         fprintf(file, "NO[%d]=%d,%d,%d,%d\n", c_order++, _shape[i]->xleft(), _shape[i]->ylower(), 
            _shape[i]->xright(), _shape[i]->yupper());
         break;
       case RED:
         fprintf(file, "CA[%d]=%d,%d,%d,%d\n", a_order++, _shape[i]->xleft(), _shape[i]->ylower(), 
            _shape[i]->xright(), _shape[i]->yupper());
         break;
       case BLUE:
         fprintf(file, "CB[%d]=%d,%d,%d,%d\n", b_order++, _shape[i]->xleft(), _shape[i]->ylower(), 
            _shape[i]->xright(), _shape[i]->yupper());
         break;
       default:
         break;
      }
   }
}
bool Component::colorable()
{
   return (_shape[0]->color() != UNCOLORABLE);
}

void Component::range(int& xl, int& yl, int& xr, int& yu)
{
   xl = _xl;
   xr = _xr;
   yl = _yl;
   yu = _yu;
}

void Component::pushWindow(Window* window)
{
   _window.push_back(window);
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

Window::Window(unsigned id, int xl, int yl, int xr, int yu) :
   _id(id), _xl(xl), _xr(xr), _yl(yl), _yu(yu), _colorDiff(INT_MIN), _adjusted(false)
{}

bool Window::overlap(int xl, int yl, int xr, int yu)
{
   #define inRange(x,l,u)  \
   x>=l && x<=u
   if (inRange(this->_xl,xl,xr))
      return true;
   if (inRange(this->_xr,xl,xr))
      return true;
   if (inRange(this->_yl,yl,yu))
      return true;
   if (inRange(this->_yu,yl,yu))
      return true;
   return false;
}

void Window::pushComponent(Component* comp)
{
   _component.push_back(comp);
}

void Window::refreshShape()
{
   size_t numComp = _component.size();
   for (size_t i=0;i<numComp;++i) {
      size_t numShape = _component[i]->_shape.size();
      for (size_t j=0;j<numShape;++j) {
         Shape* p = _component[i]->_shape[j];
         int area = p->overlapArea(_xl,_yl,_xr,_yu);
         if (area > 0) {
            _shape.push_back(p);
            _area.push_back(area);
         }
      }
   }
}
int Window::calculateDiff()
{
   int RedArea=0, BlueArea=0;
   size_t n = _shape.size();
   for (size_t i=0;i<n;++i) {
      switch(_shape[i]->color()) {
       case RED:
         RedArea += _area[i];
         break;
       case BLUE:
         BlueArea += _area[i];
         break;
       default:
         fprintf(stderr, "At Window %u, an uncolorable shape %u found\n", _id, _shape[i]->id());
         assert(0);
      }
   }
   _colorDiff = BlueArea - RedArea;
   return _colorDiff;
}

void Window::adjust()
{
   size_t n = _shape.size();
   map<Component*,int> componentArea;
   for (size_t i=0;i<n;++i) {
      int sign = (_shape[i]->color() == RED)? -1 : 1;
      componentArea[_shape[i]->getComp()] += (sign * _area[i]);
   }
   map<Component*,int>::iterator it;
   Component* bestSol = NULL;
   int bestArea = 0;
   for (it=componentArea.begin(); it!=componentArea.end(); ++it) {
      if (abs(it->second + _colorDiff) < abs(bestArea + _colorDiff)) { 
         bestArea = it->second;
         bestSol = it->first;
      }
   }
   if (bestSol != NULL)
      bestSol->inverse();
   _adjusted = true;
}
