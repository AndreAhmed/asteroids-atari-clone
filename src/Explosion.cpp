#include "Explosion.h"
#include "cinder/app/AppBasic.h"


using namespace ci;

Explosion::Explosion(void)
{
	m_Pos = Vec2f(0,0);
	m_Vel = Vec2f(0,0);
}


Explosion::~Explosion(void)
{
}

void Explosion::Draw()
{
	float ratio = (float)app::getWindowWidth()/app::getWindowHeight();

	gl::pushMatrices();
	gl::translate(m_Pos*ratio);
	 
	gl::scale(0.2*ratio,0.2*ratio,0.2*ratio);
	gl::color(ci::Color(1,1,1));
	 
	/*gl::drawSolidCircle(Vec2f(0, 1), 10);
	gl::drawSolidCircle(Vec2f(10, 2 ), 10);
	gl::drawSolidCircle(Vec2f(30, 1), 10);
	gl::drawSolidCircle(Vec2f(50, 2), 10);
*/


	gl::popMatrices();
}
