#ifndef __SCREENELEMENT_H__
#define __SCREENELEMENT_H__

class ScreenElement
{
public:
    virtual void OnUpdate(float delta) = 0;
    virtual void OnRender() = 0;

    bool IsVisible() const { return this->visible; }

protected:
    float   x, y;
    float   width, height;
    bool    visible;
};

#endif // __SCREENELEMENT_H__