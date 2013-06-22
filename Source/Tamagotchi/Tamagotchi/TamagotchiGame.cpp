#include "TamagotchiGame.h"

//-----------------------------------------------------------------------------------------------------------
//  class Tamagotchi
//-----------------------------------------------------------------------------------------------------------

std::shared_ptr<GameLogic> Tamagotchi::CreateGameLogic()
{
    return std::shared_ptr<GameLogic>(TG_NEW TamagotchiGameLogic);
}

std::shared_ptr<GameView> Tamagotchi::CreateFirstView()
{
    // TODO: this should return main menu or intro view.
    return std::shared_ptr<GameView>(TG_NEW GameView(this->GetRenderer()));
}