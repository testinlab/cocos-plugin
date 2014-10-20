#ifndef __CLASS_TESTIN_TRAVERSER__
#define __CLASS_TESTIN_TRAVERSER__

#include <list>
#include <string>
#include "cocos_inc.h"

NS_CC_BEGIN

class TestinTraverser
{
public:
	static const std::string dump();

private:
	static const char* className(TCNode* node);
	static const char* baseClassName(TCNode* node);
	static void parseNodeToXML(tinyxml2::XMLDocument *doc, tinyxml2::XMLElement* parent, TCNode* node);
	static void parseNodeAttr(tinyxml2::XMLDocument *doc, tinyxml2::XMLElement* element, TCNode* node);
	static const std::string descSpriteFrame(TCNode* node);
	static const std::string descTexture(TCTexture2D* texture);
	static const std::string descSprite9(TCNode* node);

	static void split(std::string& s, std::string& delim, std::vector<std::string>* ret);
};
NS_CC_END


#endif	//__CLASS_TESTIN_TRAVERSER__
