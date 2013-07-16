#ifndef __TAMAGOTCHIGAME_H__
#define __TAMAGOTCHIGAME_H__

#include "../Engine/TamagotchiEngine.h"

//-----------------------------------------------------------------------------------------------------------
//  class Tamagotchi
//-----------------------------------------------------------------------------------------------------------

class Tamagotchi : public TamagotchiEngine
{
protected:
    virtual std::shared_ptr<GameLogic>  CreateGameLogic() override;
    virtual std::shared_ptr<GameView>   CreateFirstView() override;
};


//-----------------------------------------------------------------------------------------------------------
//  class Tamagotchi
//-----------------------------------------------------------------------------------------------------------

class TamagotchiGameLogic : public GameLogic
{

};

#endif // __TAMAGOTCHIGAME_H__