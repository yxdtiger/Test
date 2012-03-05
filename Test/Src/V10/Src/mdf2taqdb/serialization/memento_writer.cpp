#include "memento_writer.h"

using namespace wmdf;


void XMLMementoWriter::ProcessMemento(Memento *pm, TiXmlElement *pNode, TiXmlDocument& doc)
{

	// 1.增加(根)结点
	TiXmlElement *NodeElement = new TiXmlElement(pm->GetName().c_str());
	
	if (pNode==NULL)//添加跟节点
	{
		doc.LinkEndChild(NodeElement);
	}
	else
	{
		pNode->LinkEndChild(NodeElement);
	}
	//
	//if (RootElement)
	//{
	//	RootElement->LinkEndChild(NodeElement);
	//}else
	//{
	//	doc.LinkEndChild(NodeElement);
	//}

	// 2.循环设置属性信息
	int i = 0;
	for(i = 0; i< pm->GetAttributeCount(); i++)
	{
		string str = pm->GetAttributeName(i);
		NodeElement->SetAttribute(pm->GetAttributeName(i).c_str(),pm->GetAttributeValue(i).c_str());
	}

	// 3.递归进行ChildMemento处理
	for(i = 0; i< pm->GetChildCount(); i++)
	{
		Memento *pChildMemento = pm->GetChild(i);
		ProcessMemento(pChildMemento,NodeElement,doc);
	}
}
void XMLMementoWriter::Write(Memento *pm, char *path)
{
	TiXmlDocument doc;
	Write(pm,doc);
	doc.SaveFile(path);
}

char* XMLMementoWriter::WriteToBuf(Memento *pm) 
{
	TiXmlDocument doc;
	Write(pm,doc);
	//char* newbuf = new char[40000];
	//memset(newbuf,0,sizeof(char) * 40000);
	//doc.Value();
	//char* charBuf = newbuf;
	//delete []newbuf;
	return (char*)doc.Value();
}

void XMLMementoWriter::Write(Memento *pm, TiXmlDocument& doc)
{
	ProcessMemento(pm,NULL,doc);
}
