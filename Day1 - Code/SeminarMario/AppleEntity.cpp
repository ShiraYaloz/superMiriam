#include "AppleEntity.h"
using namespace cv;
using namespace std;
EntityPtr createApple(std::string const& PictureFolder)
{
	IGraphicsComponentPtr graphicsPtr(
		new AppleGraphics(PictureFolder));
	IPhysicsComponentPtr physicsPtr = make_shared<ConstVelocityPhysics>(Point(13, 9));
	EntityPtr apple(new Entity(make_shared<EntityState>(graphicsPtr, physicsPtr)));
	return apple;
}
