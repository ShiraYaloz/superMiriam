#include "scoreEntity.h"
#include "Entities.h"
using namespace std;

EntityPtr createScore(int scale, int font) {

IGraphicsComponentPtr graphicsPtr(
	new ScoresGraphics(scale,0,font));
IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();
EntityPtr ePtr(new Entity(make_shared<EntityState>(graphicsPtr, physicsPtr)));
return ePtr;
}
