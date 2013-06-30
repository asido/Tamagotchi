#include "GameView.h"
#include "ScreenElement.h"
#include "TamagotchiEngine.h"
#include "Rendering/Renderer.h"
#include "Rendering/Scene.h"

const float SCREEN_REFRESH_RATE = 1000.0f / 60.0f;

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