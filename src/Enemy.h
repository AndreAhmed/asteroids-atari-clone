#pragma  once
#include "cinder/Vector.h"

class Enemy
{
	enum {Enemy_Dead, Enemy_Alive};
public:
	Enemy();
	~Enemy();
	void Init();
	void Draw();
	void Update();
	ci::Vec2f  m_Pos;
	ci::Vec2f  m_Vel;
	int		   m_Age;
	int		   m_Dir;
	int		   m_State;
	int		   m_Counter;
	int		   m_LifeSpan;
	float	   m_SavedDy;
	int	       m_LastShot;
	int		   m_lastDirChange;
};