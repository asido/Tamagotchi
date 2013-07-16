#ifndef __GAMEVIEW_H__
#define __GAMEVIEW_H__

#include <memory>
#include <list>

//-----------------------------------------------------------------------------------------------------------
//  class GameView
//-----------------------------------------------------------------------------------------------------------

class IRenderer;
class ScreenElement;
class ScreenEvent;
class Scene;

typedef std::list< std::shared_ptr<ScreenElement> > ScreenElementList;

class GameView
{
public:
    GameView();
    virtual ~GameView();

    void AddElement(std::shared_ptr<ScreenElement> element);
    void RemoveElement(std::shared_ptr<ScreenElement> element);

    void ResetScene();

    virtual void OnUpdate(float delta);
    virtual void OnRender();

    virtual void HandleScreenEvent(const ScreenEvent &event);

private:
    std::shared_ptr<Scene>  scene;
    ScreenElementList       screenElements;
};

#endif // __GAMEVIEW_H__