#include "Asteroid.h"
#include "cinder/app/App.h"

using namespace ci;

bool toggleCircles = false;


void Asteroid::Draw()
{
	float ratio = (float)app::getWindowAspectRatio();
	gl::pushMatrices();
	 
	gl::translate(m_Pos);
 
	gl::scale(m_size,m_size);
	gl::color(ci::Color(1,1,1));
	if (m_iShapeType == 1)
	{
		gl::drawLine(Vec2f(20,0),Vec2f(80,0));
		gl::drawLine(Vec2f(80,0),Vec2f(100,20));
		gl::drawLine(Vec2f(100,20),Vec2f(100,50));
		gl::drawLine(Vec2f(100,50),Vec2f(60,100));
		gl::drawLine(Vec2f(60,100),Vec2f(40,100));
		gl::drawLine(Vec2f(40,100),Vec2f(50,70));
		gl::drawLine(Vec2f(50,70),Vec2f(25,90));
		gl::drawLine(Vec2f(25,90),Vec2f(0,70));
		gl::drawLine(Vec2f(0,70),Vec2f(20,40));
		gl::drawLine(Vec2f(20,40),Vec2f(0,20));
		gl::drawLine(Vec2f(0,20),Vec2f(20,0));
		//gl::drawLine(Vec2f(-15,0),Vec2f(-10,-5));
		//gl::drawLine(Vec2f(-10,-5),Vec2f(-5,-5));
		//gl::drawLine(Vec2f(-5,-5),Vec2f(-5,-8));
		//gl::drawLine(Vec2f(-5,-8),Vec2f(5,-8));
		//gl::drawLine(Vec2f(5,-8),Vec2f(5,-5));
		//gl::drawLine(Vec2f(5,-5),Vec2f(10,-5));
		//gl::drawLine(Vec2f(10,-5),Vec2f(15,0));
		//gl::drawLine(Vec2f(15,0),Vec2f(10,5));
		//gl::drawLine(Vec2f(10,5),Vec2f(-10,5));
		//gl::drawLine(Vec2f(-10,5),Vec2f(-10,5));
		//gl::drawLine(Vec2f(-15,0),Vec2f(-10,5));


	}
	else if (m_iShapeType == 2)
	{
		gl::drawLine(Vec2f(20,0),Vec2f(50,0));
		gl::drawLine(Vec2f(50,0),Vec2f(100,20));
		gl::drawLine(Vec2f(100,20),Vec2f(100,30));
		gl::drawLine(Vec2f(100,30),Vec2f(50,50));
		gl::drawLine(Vec2f(50,50),Vec2f(100,60));
		gl::drawLine(Vec2f(100,60),Vec2f(65,100));
		gl::drawLine(Vec2f(65,100),Vec2f(30,90));
		gl::drawLine(Vec2f(30,90),Vec2f(20,100));
		gl::drawLine(Vec2f(20,100),Vec2f(0,75));
		gl::drawLine(Vec2f(0,75),Vec2f(10,50));
		gl::drawLine(Vec2f(10,50),Vec2f(0,20));
		gl::drawLine(Vec2f(0,20),Vec2f(20,0));


	}

	else if (m_iShapeType == 3)
	{
		gl::drawLine(Vec2f(30,0),Vec2f(50,20));
		gl::drawLine(Vec2f(50,20),Vec2f(75,0));
		gl::drawLine(Vec2f(75,0),Vec2f(100,20));
		gl::drawLine(Vec2f(100,20),Vec2f(80,50));
		gl::drawLine(Vec2f(80,50),Vec2f(100,70));
		gl::drawLine(Vec2f(100,70),Vec2f(75,100));
		gl::drawLine(Vec2f(75,100),Vec2f(30,100));
		gl::drawLine(Vec2f(30,100),Vec2f(0,70));
		gl::drawLine(Vec2f(0,70),Vec2f(0,20));
		gl::drawLine(Vec2f(0,20),Vec2f(30,0));
		 

	}
	gl::popMatrices();

	gl::pushMatrices();

	m_Radius = getRadius();
	if(m_state == Asteroid_Medium)
	{
		m_Radius = 20;
	}
	else if ( m_state == Asteroid_Small)
	{
		m_Radius = 10;
	}
	if(toggleCircles)
	{
		 gl::drawStrokedCircle(getCenter(), m_Radius);
		 gl::popMatrices();
	}
}

void Asteroid::Init(int shapeType, float size)
{
	
	float r = (float)rand()/(float)RAND_MAX;
	if ((float)rand()/(float)RAND_MAX < 0.5) {
		m_Pos.x = -app::getWindowWidth() / 4;
		if ((float)rand()/(float)RAND_MAX < 0.5)
			m_Pos.x = app::getWindowWidth() / 4;
		    m_Pos.y = (int) ((float)rand()/(float)RAND_MAX * app::getWindowWidth());
	} else {
		m_Pos.x = (int) ((float)rand()/(float)RAND_MAX * app::getWindowWidth());
		m_Pos.y = -app::getWindowHeight() / 4;
		if (rand() < 0.5)
			m_Pos.y = app::getWindowHeight() / 4;
	}

	m_Vel.x = (float)rand()/(float)RAND_MAX * 2 - 1;
	m_Vel.y =(float)rand()/(float)RAND_MAX * 2  - 1;
	
	m_iShapeType = shapeType;
	m_size = size;
	 
}
 
Asteroid::Asteroid()
{
	m_bHit = false;
}

void Asteroid::Update()
{
	float ratio = (float)app::getWindowAspectRatio();
	if (m_Pos.x > app::getWindowWidth()+50)
	{
		m_Pos.x = 0;
	}

	else if (m_Pos.x < -50)
	{
	 
		m_Pos.x = app::getWindowWidth()+m_Pos.x;
	}
	if (m_Pos.y < -50)
	{
	 
		m_Pos.y = app::getWindowHeight()+m_Pos.y;
	}
	else if (m_Pos.y > app::getWindowHeight()+50)
	{
		 
		m_Pos.y =  0;
	}
		m_Pos.x +=m_Vel.x;
		m_Pos.y +=m_Vel.y;
}

cinder::Vec2f Asteroid::getCenter()
{ 
	ci::Vec2f center = m_Pos;
	float ratio = (float)app::getWindowAspectRatio();
	center.x = m_Pos.x + 25;
	center.y = m_Pos.y + 25;
	if(m_state == Asteroid_Small)
	{
		center.x = m_Pos.x+5;
		center.y = m_Pos.y+5;
	}
	else if(m_state == Asteroid_Medium)
	{
		center.x = m_Pos.x+15;
		center.y = m_Pos.y+15;
	}
	return center;
}

double Asteroid::getRadius()
{
	 
	if(m_state == Asteroid_Medium)
	{
		return  20;
	}
	else if ( m_state == Asteroid_Small)
	{
		return 10;
	}
	else 
	{
		ci::Vec2f  _vec = (getCenter()  - (getCenter()+Vec2f(100,100)));
		return _vec.length()*0.2; 
	}
}