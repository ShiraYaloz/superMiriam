#include "Physics.h"
#include <algorithm>

using namespace cv;
using namespace std;
/// //////////////////////////////////////////

// @2: Read this function and try to understand what it does. Use Image Watch !
bool checkPixelLevelCollision(IPhysicsComponent const* first, IPhysicsComponentPtr const& other)
{
	cv::Mat const& mask_First = first->getCollisionMask();
	cv::Mat const& mask_Other = other->getCollisionMask();
	if (mask_Other.empty() || mask_First.empty())
		return false;

	Point TL_first = first->getTL();
	Point TL_other = other->getTL();
	
	// ROI = Reagion of Interest
	Rect firstROI(TL_first, TL_first + Point(mask_First.size()));
	Rect othersROI(TL_other, TL_other + Point(mask_Other.size()));
	Rect intersection = firstROI & othersROI;
	if (intersection.empty())
		return false;

	Rect intersection_RelativeToMe(
		intersection.tl() - TL_first, intersection.br() - TL_first);

	Rect intersection_RelativeToOther(
		intersection.tl() - TL_other, intersection.br() - TL_other);

	Mat myMaskROI = mask_First(intersection_RelativeToMe);
	Mat othersMaskROI = mask_Other(intersection_RelativeToOther);
	Mat whereMeAndOtherIntersect = myMaskROI.mul(othersMaskROI);

	return countNonZero(whereMeAndOtherIntersect);
}

/// //////////////////////////////////////////

FixedWidgetPhysics::FixedWidgetPhysics()
	:_topLeft(0,0), _mask()
{
}

void FixedWidgetPhysics::reset(cv::Point const& tl)
{
	_topLeft = tl;
}

bool FixedWidgetPhysics::update(cv::Mat const& collisionMask)
{
	_mask = collisionMask;
	return false;
}

bool FixedWidgetPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

cv::Point const& FixedWidgetPhysics::getTL() const
{
	return _topLeft;
}

cv::Mat const& FixedWidgetPhysics::getCollisionMask() const
{
	return _mask;
}
////////////-----------------------------------------///////////////

ConstVelocityPhysics::ConstVelocityPhysics(cv::Point const& velocity)
	:_currTL(0, 0), _mask(), _velocity(velocity)
{
}

void ConstVelocityPhysics::reset(cv::Point const& tl)
{

	_currTL = tl;
}

bool ConstVelocityPhysics::update(cv::Mat const& collisionMask)
{
	_currTL = _currTL + _velocity;
	_mask = collisionMask;
	return false;
}

cv::Mat const& ConstVelocityPhysics::getCollisionMask() const
{
	return _mask;
}

bool ConstVelocityPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

cv::Point const& ConstVelocityPhysics::getTL() const
{
	return _currTL;
}
/////////////////////////////////////////////////////////
/**/

JumpPhysics::JumpPhysics(int horizontalVelocity, int initialVerticalVelocity, int gravity)
	:_currTL(0, 0), _mask(), _initialJumpVelocity(horizontalVelocity, -initialVerticalVelocity),
	_gravity(gravity),_currVelocity(horizontalVelocity, -initialVerticalVelocity),_jumpStartTL(0,0)
{}

void JumpPhysics::reset(cv::Point const& tl)
{
	_jumpStartTL = tl;
	_currTL = tl;
	_currVelocity = _initialJumpVelocity;
}

bool JumpPhysics::update(cv::Mat const& collisionMask)
{
	_mask = collisionMask;
	_currTL += _currVelocity;
	_currVelocity.y += _gravity;
	if ((_currTL.y + _currVelocity.y) > _jumpStartTL.y)
	{
		reset(Point(_currTL.x, _jumpStartTL.y));
		return true;
	}
	return false;
}

cv::Mat const& JumpPhysics::getCollisionMask() const
{
	return _mask;
}

bool JumpPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

cv::Point const& JumpPhysics::getTL() const
{
	return _currTL;
}

////////////////--------decorator---------///////////////////


BoundedPhysicsDecorator::BoundedPhysicsDecorator(IPhysicsComponentPtr base, cv::Rect bg)
	:_base(base),_bounds(bg)
{
}

void BoundedPhysicsDecorator::reset(cv::Point const& tl)
{
	_base->reset(tl);
}

bool BoundedPhysicsDecorator::update(cv::Mat const& collisionMask)
{
	bool b = _base->update(collisionMask);
	if (_base->getTL().x > (_bounds.x + _bounds.width) - _base->getCollisionMask().size().width)
		///--???????????----///
		_base->reset(Point(_bounds.tl().x,(_bounds.br().y - _base->getCollisionMask().size().height - 52)));
	else if (_base->getTL().x < _bounds.x)
	{	
		_base->reset(Point((_bounds.x + _bounds.width) - _base->getCollisionMask().size().width, 
			(_bounds.height - _base->getCollisionMask().size().height - 52)));
	}
	/*_base->reset(Point(_bounds.tl().x, _bounds.br().y));*/
	return b;
}

cv::Mat const& BoundedPhysicsDecorator::getCollisionMask() const
{
	return _base->getCollisionMask();
}

bool BoundedPhysicsDecorator::checkCollision(IPhysicsComponentPtr const& other) const
{
	return _base->checkCollision(other);
}

cv::Point const& BoundedPhysicsDecorator::getTL() const
{
	return _base->getTL();
}

/// /////////////////////////////////

NotCollisionPhysicsDecorator::NotCollisionPhysicsDecorator(IPhysicsComponentPtr p)
	:_base(p)
{
	
}

void NotCollisionPhysicsDecorator::reset(cv::Point const& tl)
{
	_base->reset(tl);
}

bool NotCollisionPhysicsDecorator::update(cv::Mat const& collisionMask)
{
	return _base->update(collisionMask);
}

cv::Mat const& NotCollisionPhysicsDecorator::getCollisionMask() const
{
	return Mat();
}

bool NotCollisionPhysicsDecorator::checkCollision(IPhysicsComponentPtr const& other) const
{
	return false;
}
IPhysicsComponentPtr NotCollisionPhysicsDecorator::getBase() {
	return _base;
}
cv::Point const& NotCollisionPhysicsDecorator::getTL() const
{
	return _base->getTL();
}
