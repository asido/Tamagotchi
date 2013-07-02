#include "GameView.h"
#include "ScreenElement.h"
#include "TamagotchiEngine.h"
#include "Rendering/Renderer.h"
#include "Rendering/Scene.h"
#include "UI/ScreenElement.h"

const float SCREEN_REFRESH_RATE = 1000.0f / 60.0f;

//-----------------------------------------------------------------------------------------------------------
//  class GameView
//-----------------------------------------------------------------------------------------------------------

GameView::GameView()
    : scene(TG_NEW Scene)
{

}

GameView::~GameView()
{
    while (!this->screenElements.empty())
    {
        this->screenElements.pop_front();
    }
}

void GameView::AddElement(std::shared_ptr<ScreenElement> element)
{
    this->screenElements.push_front(element);
}

void GameView::RemoveElement(std::shared_ptr<ScreenElement> element)
{
    this->screenElements.remove(element);
}

void GameView::ResetScene()
{
    this->scene.reset(TG_NEW Scene);
}

void GameView::OnUpdate(float delta)
{
    for (ScreenElementList::iterator it = this->screenElements.begin(); it != this->screenElements.end(); ++it)
    {
        std::shared_ptr<ScreenElement> element = *it;
        element->OnUpdate(delta);
    }
}

void GameView::OnRender()
{
    if (!this->scene)
    {
        LogInfo("scene == NULL");
        return;
    }

    // TODO: return if we last rendering has happened earlier than SCREEN_REFRESH_RATE
    // as it is pointless to render to screen more often than it refreshes.

    g_engine->GetRenderer()->PreRender();

    this->scene->OnRender();

    // TODO: we should sort screen elements by Z value here or during AddSceneElement

    for (ScreenElementList::iterator it = this->screenElements.begin(); it != this->screenElements.end(); ++it)
    {
        std::shared_ptr<ScreenElement> element = *it;
        if (element->IsVisible())
        {
            element->OnRender();
        }
    }

    g_engine->GetRenderer()->PostRender();
}

void GameView::HandleScreenEvent(const ScreenEvent &event)
{
    for (ScreenElementList::iterator it = this->screenElements.begin(), end = this->screenElements.end(); it != end; ++it)
    {
        std::shared_ptr<ScreenElement> screenElement = *it;
        if (screenElement->HandleEvent(event))
        {
            // Break as soon as the event is handled by the first ScreenElement.
            // XXX: by sorting elements we should be able to apply this technique to force only the top window
            // handle the event and windows bellow it not receive anything.
            break;
        }
    }
}