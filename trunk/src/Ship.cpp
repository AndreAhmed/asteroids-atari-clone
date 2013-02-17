#include "Ship.h"
#include "cinder/app/AppBasic.h"

extern bool toggleCircles;
using namespace ci;
Ship::Ship(void)
{
	m_Pos = Vec2f(0,0);
	m_Vel = Vec2f(0, 0);
	m_Angle = -90;
	m_Thrust = false;
	left=right=up=fire=false;
	m_fire = 0;
	m_lifeSpan = 100;
	m_saveField = false;
}


Ship::~Ship(void)
{
}

void Ship::Draw( )
{
	if(m_state == Ship_Dead)
		return;

	float ratio = (float)app::getWindowWidth()/app::getWindowHeight();
	float w = app::getWindowWidth();
	gl::pushMatrices();
	gl::translate(m_Pos);
 
	gl::rotate(Vec3f(0,0,m_Angle));
	gl::scale(0.2,0.2,0.2);
	gl::color(ci::Color(1,1,1));
	//   /
	static int x = -80;
	static int y = -23;
	gl::drawLine(Vec2f(-72,-43),Vec2f(69,0));

	//  - 
	gl::drawLine(Vec2f(-73,-40),Vec2f(-10,0));
	gl::drawLine(Vec2f(-10,0),Vec2f(-73,40));

	// \ 
	gl::drawLine(Vec2f(69,0),Vec2f(-72,43));
	if(m_Thrust)
	{
		gl::drawLine(Vec2f(-43, -3), Vec2f(-60,0));
		gl::drawLine(Vec2f(-43, 5), Vec2f(-60,0) );
		m_Thrust = !m_Thrust;
	}

	gl::popMatrices();

	for (list<Bullet>::iterator itr = m_Bullets.begin(); itr!=m_Bullets.end(); itr++)
	{
		itr->Draw(59,-8);
	}
	if(toggleCircles)
	{
		gl::pushMatrices();
		gl::drawStrokedCircle(getCenter(), getRadius());
		gl::popMatrices();
	}

}

void Ship::Init( Vec2f pos )
{
	this->m_Pos = pos;
	m_state = Ship_Alive;
}

void Ship::FireBullet()
{
	if(m_fire<1)
	{

		int d = 22;
		Bullet bullet;
		bullet.m_Pos.x = m_Pos.x + cos(m_Angle*M_PI/180)*d;
		bullet.m_Pos.y = m_Pos.y + sin(m_Angle*M_PI/180)*d;
		bullet.m_Vel.y =  7*sin(m_Angle*M_PI/180);
		bullet.m_Vel.x =  7*cos(m_Angle*M_PI/180);	
		m_Bullets.push_back(bullet);
		m_fire++;
	}

}


void Ship::Update()
{
	if(m_state == Ship_Alive)
	{


		float ang = 7;
		float factor = 0.1;

		if (m_Pos.x > app::getWindowWidth())
		{
			m_Pos.x =  0;
		}

		else if (m_Pos.x < 0)
		{
			m_Pos.x = app::getWindowWidth();
		}
		if (m_Pos.y < 0)
		{
			m_Pos.y = app::getWindowHeight();
		}
		else if (m_Pos.y > app::getWindowHeight())
		{
			m_Pos.y =  0;
		}

		if(right)
		{
			m_Angle = fmod(m_Angle + ang, 360);
		}
		if(left)
		{

			m_Angle = fmod(m_Angle - ang, 360);
		}
		if(up)
		{
			m_Thrust =!m_Thrust;
			m_Vel.x += factor*cos(m_Angle*M_PI/180);
			m_Vel.y += factor*sin(m_Angle*M_PI/180);
		}	


		int counter = 0;
		if(m_Bullets.size()>0)
		{
			for (list<Bullet>::iterator itr = m_Bullets.begin(); itr!=m_Bullets.end();)
			{
				itr->m_Pos.x += itr->m_Vel.x*1.2;
				itr->m_Pos.y += itr->m_Vel.y*1.2;
				itr->m_life  -= itr->m_Vel.length();


				if(itr->m_Pos.x > app::getWindowWidth())
				{
					itr->m_Pos.x = -21;
				}
				else if (itr->m_Pos.x < -21)
				{
					itr->m_Pos.x = app::getWindowWidth();
				}
				if (itr->m_Pos.y > app::getWindowHeight())
				{
					itr->m_Pos.y = -14;
				}
				else if (itr->m_Pos.y < -14)
				{
					itr->m_Pos.y = app::getWindowHeight();
				} 

				if(itr->m_life<0)
				{
					std::cout<<"erase "<<std::endl;
					itr = m_Bullets.erase(itr);

				}
				else
				{
					itr++;
				}

			}
		}

		m_Vel.limit(6);

		// always apply friction in direction opposite current trajectory
		float fx = -m_Vel.x;
		float fy = -m_Vel.y;
		float length_f = sqrt(fx*fx+fy*fy); // normally we would avoid square root at all costs!

		// compute the frictional resitance

		if (fabs(length_f) > 0.1)
		{ 
			fx = 0.0005*fx/length_f;
			fy = 0.0005*fy/length_f;
		} // end if
		else
		{
			fx=fy=0;
		}

		// now apply friction to forward velocity
		m_Vel.x+=fx;
		m_Vel.y+=fy;
		m_Pos.x +=m_Vel.x;
		m_Pos.y +=m_Vel.y;

		if(fire)
		{
			FireBullet();			
		}

	}
	
		 
	 
	if(m_state == Ship_Dead && m_saveField==true)
	 
	{
		m_state	= Ship_Alive;
		m_Vel   = Vec2f(0,0);
		m_Pos.x = app::getWindowWidth()/2;
		m_Pos.y = app::getWindowHeight()/2;
		 

	}
	

	
}

ci::Vec2f Ship::getCenter()
{
	ci::Vec2f center = m_Pos;
	float ratio = (float)app::getWindowAspectRatio();
 
	return center;
}

double Ship::getRadius()
{

	float ratio = (float)app::getWindowAspectRatio();

	ci::Vec2f  _vec = (getCenter()  - (getCenter()+Vec2f(40,40)));
	return _vec.length()*0.3; 


}
