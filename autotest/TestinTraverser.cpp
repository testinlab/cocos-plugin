#include "TestinTraverser.h"
#include "TestinUiautomator.h"
#include "cocos_inc.h"
#include <sys/types.h>
#include <unistd.h>


NS_CC_BEGIN



const std::string TestinTraverser::dump() {
	//get running scene and then to traverse all sub nodes
	TCDirector* director = TCDirector::funcDirectorGetInstance();
	TCScene* scene = director->getRunningScene();
	if (scene == NULL)
		return std::string("");

	//create a tinyxml2 document
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
    if (NULL==pDoc) {
        return std::string("");
    }
	tinyxml2::XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    if (NULL==pDel) {
		delete pDoc;
        return std::string("");
    }
	pDoc->LinkEndChild(pDel);

	//root element
	tinyxml2::XMLElement *rootElement = pDoc->NewElement("hierarchy");
	TCSize winSize = director->getWinSize();
	char scStr[20] = {0};
	sprintf(scStr, "%0.0fx%0.0f", winSize.width, winSize.height);
	rootElement->SetAttribute("screen", scStr);
	rootElement->SetAttribute("support", "cocos2d-x");
	rootElement->SetAttribute("version", TESTIN_CC_VERSION);
	memset(scStr, 0, 20);
	sprintf(scStr, "%08X", COCOS2D_VERSION);
	rootElement->SetAttribute("cocosVersionCode", scStr);
	rootElement->SetAttribute("cocosVersion", cocos2dVersion());

	pDoc->LinkEndChild(rootElement);
	int pid = (int)getpid();
	rootElement->SetAttribute("pid", pid);
	pDoc->LinkEndChild(rootElement);

	//traverse
	parseNodeToXML(pDoc, rootElement, scene);

	tinyxml2::XMLPrinter printer;
	pDoc->Accept(&printer);

	const char* prtStr = printer.CStr();
	char* cstr = (char*)malloc(strlen(prtStr) + 1);
	strcpy(cstr, prtStr);
	cstr[strlen(prtStr)] = '\0';
	delete pDoc;
	std::string ret = cstr;
	free(cstr);
	return ret;
}

void TestinTraverser::parseNodeToXML(tinyxml2::XMLDocument *doc, tinyxml2::XMLElement* parent, TCNode* node) {
	const char* baseClass = baseClassName(node);
	//only parse specified classes or their sub classes
	if (baseClass == NULL || strcmp(baseClass, "TCNode") == 0)
		return;

	node->retain();

	tinyxml2::XMLElement *element = doc->NewElement("node");
	parent->LinkEndChild(element);

	//node attributes
	parseNodeAttr(doc, element, node);

	//end travers for specified classes
	//if (strcmp(baseClass, "TCMenuItem") == 0 || strcmp(baseClass, "TCScale9Sprite") == 0) {
	//	node->release();
	//	return;
	//}

	//get child nodes
#ifdef COCOS2D_CPP_2X
	CCObject* child;
	CCARRAY_FOREACH(node->getChildren(), child)
    {
        CCNode* sub = (CCNode*) child;
		parseNodeToXML(doc, element, sub);
	}
#elif defined(COCOS2D_CPP_3X)
	//special for Label
	Label* label = dynamic_cast<Label*>(node);
	for (auto& child : node->getChildren()) {
		if (label != NULL) {
			TCSprite* sprite = dynamic_cast<TCSprite*>(child);
			if (sprite != NULL) {
				//not parse Sprite under Label
				continue;
			}
		}
		parseNodeToXML(doc, element, child);
	}
#endif
	node->release();
}


void TestinTraverser::parseNodeAttr(tinyxml2::XMLDocument *doc, tinyxml2::XMLElement* element, TCNode* node) {
	if (node == NULL)
		return;

	node->retain();

	element->SetAttribute("tagId", node->getTag());
	element->SetAttribute("zIndex", node->funcNodeGetZOder());
	element->SetAttribute("className", className(node));
	element->SetAttribute("baseClass", baseClassName(node));
	element->SetAttribute("visible", node->isVisible());

	TCSize winSize = TCDirector::funcDirectorGetInstance()->getWinSize();
	TCSprite* sprite = dynamic_cast<TCSprite*>(node);
	TCMenuItem* item = dynamic_cast<TCMenuItem*>(node);
	TCAtlasNode* atlas = dynamic_cast<TCAtlasNode*>(node);
	TCScene* scene = dynamic_cast<TCScene*>(node);
#if TESTIN_USING_CC_EXTENSION == 1
	TCScale9Sprite* sprite9 = dynamic_cast<TCScale9Sprite*>(node);
#endif
	TCSpriteBatchNode* batch = dynamic_cast<TCSpriteBatchNode*>(node);
#ifdef COCOS2D_CPP_3X
	Label* label = dynamic_cast<Label*>(node);
#endif
	bool boundsSet = false;
	if (scene != NULL) {
		const char* name = TestinUIAutomator::getInstance()->sceneName(scene);
		if (name != NULL)
			element->SetAttribute("activity", name);
	} else if (sprite != NULL) {
		//Sprite attributes
		TCSize size = sprite->funcGetBoundingBox().size;
		TCPoint offsetWorld = node->convertToWorldSpace(sprite->getOffsetPosition());
		char str[40] = {0};
		sprintf(str, "[%0.0f,%0.0f][%0.0f,%0.0f]", offsetWorld.x, winSize.height-offsetWorld.y-size.height, offsetWorld.x+size.width, winSize.height-offsetWorld.y);
		element->SetAttribute("bounds", str);
		boundsSet = true;

		TCLabelTTF* ttf = dynamic_cast<TCLabelTTF*>(node);
		if (ttf == NULL) {
			std::string desc = descSpriteFrame(sprite);
			element->SetAttribute("name", desc.c_str());
		} else {
			element->SetAttribute("text", ttf->funcGetString());
		}
#ifdef COCOS2D_CPP_3X
	} else if (label != NULL) {
		element->SetAttribute("text", label->funcGetString());
#endif
	} else if (item != NULL) {
		//MenuItem attributes
		element->SetAttribute("clickable", 1);

		TCMenuItemLabel* itemLable = dynamic_cast<TCMenuItemLabel*>(node);
		TCMenuItemSprite* itemSprite = dynamic_cast<TCMenuItemSprite*>(node);
		TCMenuItemToggle* itemToggle = dynamic_cast<TCMenuItemToggle*>(node);

		if (itemLable != NULL) {
			TCNode* labelNode = itemLable->getLabel();
			if (labelNode != NULL) {
				labelNode->retain();
				TCLabelTTF* ttf = dynamic_cast<TCLabelTTF*>(labelNode);
				if (ttf != NULL) {
					element->SetAttribute("text", ttf->funcGetString());

					TCSize size = ttf->funcGetBoundingBox().size;
					TCPoint offsetWorld = node->convertToWorldSpace(ttf->funcGetOffsetPosition());
					char str[40] = {0};
					sprintf(str, "[%0.0f,%0.0f][%0.0f,%0.0f]", offsetWorld.x, winSize.height-offsetWorld.y-size.height, offsetWorld.x+size.width, winSize.height-offsetWorld.y);
					element->SetAttribute("bounds", str);
					boundsSet = true;
				}
#ifdef COCOS2D_CPP_3X
				else {
					Label* label = dynamic_cast<Label*>(labelNode);
					if (label != NULL) {
						element->SetAttribute("text", label->funcGetString());

						Size size = label->getBoundingBox().size;
						Point offsetWorld = node->convertToWorldSpace(label->getBoundingBox().origin);
						char str[40] = {0};
						sprintf(str, "[%0.0f,%0.0f][%0.0f,%0.0f]", offsetWorld.x, winSize.height-offsetWorld.y-size.height, offsetWorld.x+size.width, winSize.height-offsetWorld.y);
						element->SetAttribute("bounds", str);
						boundsSet = true;
					}
				}
#endif
				labelNode->release();
			}
		} else if (itemSprite != NULL) {
			char* descStr = NULL;
#ifdef COCOS2D_CPP_2X
			CCObject* child;
			CCARRAY_FOREACH(node->getChildren(), child)
			{
				TCNode* sub = (TCNode*) child;
#elif defined(COCOS2D_CPP_3X)
			for (auto& sub : node->getChildren()) {
#endif
				sub->retain();
				std::string descSprite = descSpriteFrame(sub);
				const char* descSub = descSprite.c_str();
				if (descStr == NULL) {
					descStr = (char*)malloc(strlen(descSub) + 1);
					strcpy(descStr, descSub);
					descStr[strlen(descSub)] = '\0';
				} else {
					int oldLen = strlen(descStr);
					descStr = (char*)realloc(descStr, oldLen + strlen(descSub) + 2);
					strcat(descStr, "|\0");
					strcat(descStr, descSub);
					descStr[oldLen + strlen(descSub) + 1] = '\0';
				}
				sub->release();
			}
			if (descStr != NULL) {
				element->SetAttribute("name", descStr);
				free(descStr);
			}
		} else if (itemToggle != NULL) {
#ifdef COCOS2D_CPP_2X
			TCMenuItem* subItem = dynamic_cast<TCMenuItem*>(itemToggle->getSubItems()->objectAtIndex(itemToggle->getSelectedIndex()));
#elif defined(COCOS2D_CPP_3X)
			TCMenuItem* subItem = itemToggle->getSubItems().at(itemToggle->getSelectedIndex());
#endif

			tinyxml2::XMLElement *sub = doc->NewElement("node");
			element->LinkEndChild(sub);
			parseNodeAttr(doc, sub, subItem);
		}
	} else if (atlas != NULL) {
		element->SetAttribute("name", atlas->getTexture()->getName());

		TCLabelAtlas* labelAtlas = dynamic_cast<TCLabelAtlas*>(atlas);
		if (labelAtlas != NULL) {
			element->SetAttribute("text", labelAtlas->funcGetString());
		}
#if TESTIN_USING_CC_EXTENSION == 1
	} else if (sprite9 != NULL) {
		std::string sprite9Desc = descSprite9(sprite9);
		element->SetAttribute("name", sprite9Desc.c_str());
#endif
	} else if (batch != NULL) {
		TCTexture2D* texture = batch->getTexture();
		if (texture != NULL) {
			std::string descBatch = descTexture(texture);
			element->SetAttribute("name", descBatch.c_str());
		}
	}

	if (!boundsSet) {
		TCPoint anchorPoint = node->getAnchorPointInPoints();
		
		TCPoint pos = node->getPosition();
		TCSize size = node->getContentSize();
		TCPoint tl = funcPoint(pos.x - anchorPoint.x, pos.y - anchorPoint.y + size.height);
		if (node->getParent() != NULL)
			tl = node->getParent()->convertToWorldSpace(tl);
		TCPoint cpp = node->convertToWorldSpace(tl);
		char str[40] = {0};
		sprintf(str, "[%0.0f,%0.0f][%0.0f,%0.0f]", tl.x, winSize.height - tl.y, tl.x + size.width, winSize.height - tl.y + size.height);
		element->SetAttribute("bounds", str);
	} 

	node->release();
}

const char* TestinTraverser::className(TCNode* node) {
	if (node == NULL)
		return "";

	TCNode &b2 = *node;
	return typeid(b2).name();
}


const char* TestinTraverser::baseClassName(TCNode* node) {
	if (node == NULL)
		return NULL;

	TCLayer* layer = dynamic_cast<TCLayer*>(node);
	if (layer != NULL)
		return "TCLayer";
	TCSprite* sprite = dynamic_cast<TCSprite*>(node);
	if (sprite != NULL)
		return "TCSprite";
	TCMenuItem* item = dynamic_cast<TCMenuItem*>(node);
	if (item != NULL)
		return "TCMenuItem";
	TCScene* scene = dynamic_cast<TCScene*>(node);
	if (scene != NULL)
		return "TCScene";
	TCAtlasNode* atlas = dynamic_cast<TCAtlasNode*>(node);
	if (atlas != NULL)
		return "TCAtlasNode";
	TCLabelTTF* ttf = dynamic_cast<TCLabelTTF*>(node);
	if (ttf != NULL)
		return "TCLabelTTF";
	TCSpriteBatchNode* batch = dynamic_cast<TCSpriteBatchNode*>(node);
	if (batch != NULL)
		return "TCSpriteBatchNode";
#if TESTIN_USING_CC_EXTENSION == 1
	TCScale9Sprite* sprite9 = dynamic_cast<TCScale9Sprite*>(node);
	if (sprite9 != NULL)
		return "TCScale9Sprite";
#endif
#ifdef COCOS2D_CPP_3X
	TCLabel* label = dynamic_cast<TCLabel*>(node);
	if (label != NULL)
		return "TCLabel";
#endif

	return "TCNode";
}


const std::string TestinTraverser::descSpriteFrame(TCNode* node) {
	TCSprite* sprite = dynamic_cast<TCSprite*>(node);
	if (sprite != NULL) {
		sprite->retain();
		TCTexture2D* texture = sprite->getTexture();
		if (texture != NULL) {
			std::string textureKey = descTexture(texture);
			TCRect rect = sprite->displayFrame()->getRect();
			const char* format = "%s-%0.0f-%0.0f-%0.0f-%0.0f";
			int len = strlen(format) + textureKey.length() + 8*4;
			char* cstr = (char*)malloc(len);
			memset(cstr, 0, len);
			sprintf(cstr, format, textureKey.c_str(), rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
			sprite->release();
			std::string ret(cstr);
			free(cstr);
			return ret;
		}
		sprite->release();
	}

#if TESTIN_USING_CC_EXTENSION == 1
	TCScale9Sprite* sprite9 = dynamic_cast<TCScale9Sprite*>(node);
	if (sprite9 != NULL) {
		std::string sprite9Desc = descSprite9(sprite9);
		return sprite9Desc;
	}
#endif

	return std::string("");
}


#ifdef COCOS2D_CPP_2X
const std::string TestinTraverser::descTexture(TCTexture2D* texture) {
	if (texture != NULL) {
		CCDictionary* textures = CCTextureCache::sharedTextureCache()->snapshotTextures();
		CCDictElement* pElement = NULL;
		const char* key = "";
		CCDICT_FOREACH(textures, pElement)
		{
			if (pElement->getObject() == texture) {
				key = pElement->getStrKey();
				break;
			}
		}

		std::string textureKey(key);
		return textureKey;
	}
	return std::string("");
}
#elif defined(COCOS2D_CPP_3X)
const std::string TestinTraverser::descTexture(TCTexture2D* texture) {
	if (texture != NULL) {
		std::string textureKey("");
		TCTextureCache* cache = TCDirector::funcDirectorGetInstance()->getTextureCache();
		std::string descs = cache->getCachedTextureInfo();
		std::vector<std::string> array;
		std::string delim("\n");
		split(descs, delim, &array);
		//not to use reges since reges is PARTIAL supported in gcc
		//std::regex subReg("\\\"(.*?)\\\" rc=\\d+ id=(\\d+) .*", std::regex_constants::basic);
		for (auto& line : array) {
			/*
			std::match_results<std::string::const_iterator> result;
			if (std::regex_match(line, result, subReg)) {

				std::string key = result[1];
				std::string idstr = result[2];
				int id = std::atoi(idstr.c_str());

				TCLog("texture cache: key=%s id=%s", key.c_str(), idstr.c_str());

				if (texture->getName() == id) {
					textureKey = key;
					break;
				}
			}
			*/
			if (line[0] != '"')
				continue;
			size_t last = 1;
			size_t index = line.find_first_of('"', last);
			if (index == std::string::npos)
				continue;
			std::string key = line.substr(last, index - last);
			last = index + 1;
			index = line.find(" id=", last);
			if (index == std::string::npos)
				continue;
			last = index + 4;
			index = line.find_first_of(' ', last);
			if (index == std::string::npos)
				continue;
			std::string idstr = line.substr(last, index - last);
			int id = std::atoi(idstr.c_str());
			TCLog("texture cache: key=%s id=%s", key.c_str(), idstr.c_str());

			if (texture->getName() == id) {
				textureKey = key;
				break;
			}
		}
		return textureKey;
	}
	return std::string("");
}
#endif


const std::string TestinTraverser::descSprite9(TCNode* node) {
#if TESTIN_USING_CC_EXTENSION == 1
	TCScale9Sprite* sprite9 = dynamic_cast<TCScale9Sprite*>(node);
	if (sprite9 != NULL) {
		sprite9->retain();
		std::string desc;
		std::string textureKey;
#ifdef COCOS2D_CPP_2X
		CCObject* child;
		CCARRAY_FOREACH(node->getChildren(), child) {
			TCNode* sub = (TCNode*) child;
#elif defined(COCOS2D_CPP_3X)
		for (auto& sub : node->getChildren()) {
#endif
			TCSprite* sprite = dynamic_cast<TCSprite*>(sub);
			if (sprite != NULL) {
				desc += "|";
				TCRect rect = sprite->displayFrame()->getRect();
				const char* format = "%0.0f-%0.0f-%0.0f-%0.0f";
				int len = strlen(format) + 8*4;
				char* cstr = (char*)malloc(len);
				memset(cstr, 0, len);
				sprintf(cstr, format, rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
				desc += std::string(cstr);
				free(cstr);
			} else {
				TCSpriteBatchNode* batch = dynamic_cast<TCSpriteBatchNode*>(sub);
				if (batch != NULL) {
					TCTexture2D* texture = batch->getTexture();
					if (texture != NULL) {
						textureKey = descTexture(texture);
					}
				}
			}
		}
		sprite9->release();
		desc = textureKey + desc;
		return desc;
	}
#endif
	return std::string("");
}

void TestinTraverser::split(std::string& s, std::string& delim,std::vector<std::string>* ret) {  
    size_t last = 0;  
    size_t index = s.find_first_of(delim, last);
    while (index != std::string::npos) {
        ret->push_back(s.substr(last, index - last));  
        last = index + 1;  
        index = s.find_first_of(delim, last);  
    }  

    if (index - last > 0)  
    {
        ret->push_back(s.substr(last, index - last));  
    }
} 



NS_CC_END
