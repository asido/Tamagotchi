#ifndef __OBJECTFACTORY_H__
#define __OBJECTFACTORY_H__

template <class BaseType, class SubType>
BaseType *GenericObjectCreationFunction() { return new SubType; }

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