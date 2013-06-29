#ifndef __OBJECTFACTORY_H__
#define __OBJECTFACTORY_H__

#include <map>
#include "defines.h"

// TODO: We cannot use TG_NEW in this template or otherwise the following error will be generated.
// error C2661: 'TransformComponent::operator new' : no overloaded function takes 4 arguments	d:\code\tamagotchi\source\tamagotchi\engine\objectfactory.h	8
// The reason of this is because we use it for classes which expand EIGEN_MAKE_ALIGNED_OPERATOR_NEW. This expansion override ::operator new,
// but it does not declare new operator taking TG_NEW specified arguments. The solution is probably to try define the missing new
// overrides, but for now we just use plain simple 'new' here.
template <class BaseType, class SubType>
BaseType *GenericObjectCreationFunction() { return TG_NEW SubType; }

template <class BaseClass, class IdType>
class GenericObjectFactory
{
public:
    template <class SubClass>
    bool Register(IdType id)
    {
        auto it = this->creationFunctions.find(id);

        if (it == this->creationFunctions.end())
        {
            this->creationFunctions[id] = &GenericObjectCreationFunction<BaseClass, SubClass>;
            return true;
        }
        else
        {
            return false;
        }
    }

    BaseClass* Create(IdType id)
    {
        auto it = this->creationFunctions.find(id);

        if (it != this->creationFunctions.end())
        {
            ObjectCreationFunction f = it->second;
            return f();
        }
        else
        {
            return NULL;
        }
    }

private:
    typedef BaseClass* (*ObjectCreationFunction)();
    std::map<IdType, ObjectCreationFunction> creationFunctions;
};

#endif // __OBJECTFACTORY_H__