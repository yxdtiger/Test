#include "memento_reader.h"
#include <iostream>

using namespace wmdf;

//#pragma comment(lib, "D:\\workspaces\\test\\feed\\tinyxml\\Debug\\tinyxmld.lib")

XMLMementoReader::XMLMementoReader()
{

}

XMLMementoReader::~XMLMementoReader()
{
}

void XMLMementoReader::ReadFromXml(Memento *pm, TiXmlDocument& doc)
{
	TiXmlElement *rootNode = doc.RootElement();
	if(rootNode == NULL)
	{
		assert(false);
		return;
	}

	ProcessMemento(pm,rootNode);
	//TiXmlElement *node = nodes->FirstChildElement();

	//while ( node ) {

	//	/*TiXmlAttribute* attributeOfStudent = node->FirstAttribute();*/  //获得student的name属性
	//	
	//	//while ( attributeOfStudent ) {
	//	//	std::cout << attributeOfStudent->Name() << " : " << attributeOfStudent->Value() << std::endl;
	//	//	attributeOfStudent = attributeOfStudent->Next();
	//	//}
	//	//TiXmlElement* phoneElement = studentElement->FirstChildElement();//获得student的phone元素
	//	//std::cout << "phone" << " : " << phoneElement->GetText() << std::endl;
	//	//TiXmlElement* addressElement = phoneElement->NextSiblingElement();
	//	//std::cout << "address" << " : " << phoneElement->GetText() << std::endl;
	//	ProcessMemento(pm,node);
	//	node = node->NextSiblingElement();
	//}

	//for(int i = 0; i< len; i++)
	//{
	//	DOMNode *node = nodes->item(i);
	//	TCHAR name[500] = {0};
	//	node->getNodeName(name);
	//	pm->SetName(name);
	//	ProcessMemento(pm,node);
	//	delete node;
	//}
	//delete nodes;
}

void XMLMementoReader::ProcessMemento(Memento *pm, TiXmlElement *pNode)
{
	TiXmlAttribute* attributeTemp= pNode->FirstAttribute();
	//遍历所有属性
	while ( attributeTemp ) {
		pm->SetString(attributeTemp->Name() ,attributeTemp->Value());
		//std::cout << attributeTemp->Name() << " : " << attributeTemp->Value() << std::endl;
		attributeTemp = attributeTemp->Next();
	}

	//递归搜索
	TiXmlElement* childElement = pNode->FirstChildElement();
	while (childElement)
	{
		Memento *pChild = pm->CreateChild(childElement->Value());
		//std::cout<<childElement->Value();
		ProcessMemento(pChild,childElement);
		//进入下一个数据
		childElement=childElement->NextSiblingElement();
	}


	//DOMNodeList *attributes = pNode->selectNodes(TEXT("./@*"));
	//if(attributes == NULL)
	//{
	//	assert(false);
	//	return;
	//}
	//// // 1.循环获取属性信息
	////long len = attributes->getLength();
	////int i = 0;
	////for(i = 0; i< len ; i++)
	////{
	////	TCHAR name[500] = {0};
	////	TCHAR value[1000] = {0};
	////	DOMNode *pNode = attributes->item(i);
	////	pNode->getNodeName(name);
	////	pNode->getNodeValue(value);
	////	pm->SetString(name,value);
	////	delete pNode;
	////}
	////delete attributes;

	////// 2.递归进行ChildMemento处理，需要创建ChildMemento对象
	////DOMNodeList *childs = pNode->getChildNodes();
	////if(childs == NULL)
	////{
	////	assert(false);
	////	return;
	////}
	////len = childs->getLength();
	////for(i = 0; i< childs->getLength(); i++)
	////{
	////	DOMNode *pNode = childs->item(i);
	////	DOMNodeType type;
	////	pNode->getNodeType(&type);
	////	if(type != NODE_COMMENT)//如果不是注释
	////	{
	////		TCHAR name[500] = {0};
	////		pNode->getNodeName(name);
	////		CMemento *pChild = pm->CreateChild(name);
	////		ProcessMemento(pChild,pNode);
	////	}
	////	delete pNode;
	////}
	////delete childs;
}

void XMLMementoReader::ReadFromFile(Memento *pm, char *path)
{
		TiXmlDocument doc;
		doc.LoadFile(path);
		ReadFromXml(pm,doc);
}

void XMLMementoReader::ReadFromString(Memento *pm, char *string)
{
	TiXmlDocument doc;
	doc.Parse(string);
	ReadFromXml(pm,doc);
}
