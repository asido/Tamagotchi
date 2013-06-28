#ifndef __GAMEVIEW_H__
#define __GAMEVIEW_H__

#include <memory>
#include <list>

class IRenderer;
class ScreenElement;

typedef std::list< std::shared_ptr<ScreenElement> > ScreenElementList;

class GameView
{
public:
    virtual ~GameView();

    void AddElement(std::shared_ptr<ScreenElement> element);
    void RemoveElement(std::shared_ptr<ScreenElement> element);

    virtual void OnUpdate(float delta);
    virtual void OnRender();

private:
    ScreenElementList screenElements;
};

#endif // __GAMEVIEW_H__