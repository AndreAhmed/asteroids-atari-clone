#include "Bullet.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"

using namespace ci;

extern bool toggleCircles;
Bullet::Bullet(void)
{
	 
	m_Vel = Vec2f(0,0);
	m_Pos.x = 59;
	m_Pos.y = -8;
	m_life  = 330;
	m_Hit	= false;
}


Bullet::~Bullet(void)
{
}

void Bullet::Draw(int x, int y)
{
	float ratio = (float)app::getWindowWidth()/app::getWindowHeight();
	gl::pushMatrices();
	gl::translate(m_Pos);
	gl::scale(0.3*ratio,0.3,0.3);
	gl::drawSolidRect(Rectf(0,0,8,8));
	gl::popMatrices();
	gl::pushMatrices();
	double radius = getRadius();
	if(toggleCircles)
	{
		gl::drawStrokedCircle(getCenter(), radius);
		gl::popMatrices();
	}
}

ci::Vec2f Bullet::getCenter()
{
	float ratio = (float)app::getWindowWidth()/app::getWindowHeight();
	float x = m_Pos.x;
	float y = m_Pos.y;
	return Vec2f(x,y);
}

double Bullet::getRadius()
{
	float ratio = (float)app::getWindowWidth()/app::getWindowHeight();
	ci::Vec2f  _vec = (getCenter() - (getCenter()+Vec2f(8,8)));
	 
	return _vec.length()*0.3; //making radius smaller -> looks silly if they bounce although they do not touch
}
