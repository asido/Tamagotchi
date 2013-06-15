#ifndef __GAMEVIEW_H__
#define __GAMEVIEW_H__

class GameView
{
public:
	virtual void OnUpdate(float delta);
	virtual void OnRender();
};

#endif // __GAMEVIEW_H__