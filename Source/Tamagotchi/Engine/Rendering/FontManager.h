#ifndef __FONTMANAGER_H__
#define __FONTMANAGER_H__

#include <memory>
#include <string>
#include <map>

//-----------------------------------------------------------------------------------------------------------
//  class FontManager
//      Manages and prepares font resources to be rendered on the screen.
//-----------------------------------------------------------------------------------------------------------

class Font;

typedef std::map< std::string, std::shared_ptr<Font> >   FontMap;

class FontManager
{
public:
    bool                    Init();
    std::shared_ptr<Font>   GetFont(const std::string &fontName, unsigned int fontSize);

private:
    const std::string       FontNameSizeToIdentifier(const std::string &fontName, unsigned int fontSize) const;
    std::shared_ptr<Font>   LoadFont(const std::string &fontName, unsigned int fontSize, const std::string &fontIdentifier);
    bool                    RegisterFont(std::shared_ptr<Font> font, const std::string &fontIdentifier);

private:
    FontMap fonts;
};

#endif // __FONTMANAGER_H__