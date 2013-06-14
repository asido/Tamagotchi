#ifndef __SCENENODE_H__
#define __SCENENODE_H__

#include <vector>
#include <memory>

//-----------------------------------------------------------------------------------------------------------
//  class SceneNode
//-----------------------------------------------------------------------------------------------------------

class SceneNode;
typedef std::vector< std::shared_ptr<SceneNode> > SceneNodeList;

class SceneNode
{
public:
	virtual void OnUpdate(float delta);
	virtual void OnRender();

private:
	SceneNodeList childNodes;
};


//-----------------------------------------------------------------------------------------------------------
//  class CameraSceneNode
//-----------------------------------------------------------------------------------------------------------

class CameraSceneNode : public SceneNode
{
private:
	std::shared_ptr<SceneNode> target;
};

#endif // __SCENENODE_H__