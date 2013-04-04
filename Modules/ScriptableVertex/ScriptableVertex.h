#ifndef SCRIPTABLE_VERTEX_H
#define SCRIPTABLE_VERTEX_H

#include <IsoRealms/Resources/Vertex/Vertex.h>

class ScriptableVertex:public IVertex {
  private:
  Vertex cVertex;
  
  public:
  IVertex* getVertex();
    
  double getX();
  double getY();
  double getZ();
  void setX(double);
  void setY(double);
  void setZ(double);
};

#endif
