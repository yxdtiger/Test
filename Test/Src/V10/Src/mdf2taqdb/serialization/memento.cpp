#include <stdio.h>
#include "memento.h"
#include <iostream>
#ifdef __linux__
#include <cstdlib>
#endif

using namespace wmdf;

/***********************************************************/
/*			CKeyValueCollector							   */
/***********************************************************/

KeyValueCollector::~KeyValueCollector()
{
	m_list.clear();
}

void KeyValueCollector::Add(string key, string value)
{
	KeyValue kv;
	kv.csKey = key;
	kv.csValue = value;
	m_list.push_back(kv);
}

bool KeyValueCollector::Remove(string key)
{
	vector<KeyValue>::iterator iter = m_list.begin();
	while (iter != m_list.end())
	{
		if (iter->csKey == key)
		{
			m_list.erase(iter);
			return true;
		}
		iter++;
	}
	return false;
}

bool KeyValueCollector::RemoveByValue(string value)
{
	vector<KeyValue>::iterator iter = m_list.begin();
	while (iter != m_list.end())
	{
		if (iter->csValue == value)
		{
			m_list.erase(iter);
			return true;
		}
		iter++;
	}
	return false;
}


void KeyValueCollector::Clear()
{
	m_list.clear();
}

int KeyValueCollector::GetCount()
{
	return m_list.size();
}

string KeyValueCollector::GetKey(int index)
{
	string rtn("");
	if((index < (int)m_list.size()) && (index >=0))
		rtn = m_list[index].csKey;
	return rtn;
}

string KeyValueCollector::GetValue(int index)
{
	string rtn("");
	if((index < (int)m_list.size()) && (index >=0))
		rtn = m_list[index].csValue;
	return rtn;
}

string KeyValueCollector::GetValue(string key)
{
	string rtn("");
	vector<KeyValue>::iterator iter = m_list.begin();
	while (iter != m_list.end())
	{
		//std::cout<<iter->csKey<<std::endl;
		if (iter->csKey == key)
		{
			rtn = iter->csValue;
			break;
		}
		iter++;
	}
	return rtn;
}

/***********************************************************/
/*			CKeyObjectCollector							   */
/***********************************************************/

KeyObjectCollector::~KeyObjectCollector()
{
	Clear();
}

string KeyObjectCollector::GetKey(int index)
{
	string rtn("");
	if((index >=0) && (index < (int)m_list.size() ) )
		rtn = m_list[index].csKey;
	return rtn;
}

Memento * KeyObjectCollector::GetObject(int index)
{
	Memento * rtn;
	if((index >=0) && (index < (int)m_list.size() ) )
		rtn = m_list[index].pMemento;
	return rtn;
}

Memento * KeyObjectCollector::GetObject(string key)
{
	vector<KeyObject>::iterator iter = m_list.begin();
	while (iter != m_list.end())
	{
		if (iter->csKey == key)
		{
			return iter->pMemento;
		}
		iter++;
	}
	return NULL;
}

int KeyObjectCollector::GetCount()
{
	return m_list.size();
}

void KeyObjectCollector::Clear()
{
	size_t i;
	for (i=0;i<m_list.size();i++)
	{
		delete m_list[i].pMemento;
	}
	m_list.clear();
	//vector<KeyObject>::iterator iter = m_list.end();
	//iter--;
	//while (iter > m_list.begin())
	//{
	//	delete iter->pMemento;
	//	iter--;
	//}
	//m_list.clear();
}

void KeyObjectCollector::Add(string key, Memento * object)
{
	KeyObject ko;
	ko.csKey = key;
	ko.pMemento = object;
	m_list.push_back(ko);
}

bool KeyObjectCollector::Remove(string key)
{
	vector<KeyObject>::iterator iter = m_list.begin();
	while (iter != m_list.end())
	{
		if (iter->csKey == key)
		{
			delete iter->pMemento;
			m_list.erase(iter);
			return true;
		}
		iter++;
	}
	return false;
}

bool KeyObjectCollector::Remove(Memento * object)
{
	vector<KeyObject>::iterator iter = m_list.begin();
	while (iter != m_list.end())
	{
		if (iter->pMemento == object)
		{
			delete iter->pMemento;
			m_list.erase(iter);
			return true;
		}
		iter++;
	}
	return false;
}


/***********************************************************/
/*			     CMementoArray					   		   */
/***********************************************************/
void MementoArray::Add(Memento *pm)
{
	m_ary.push_back(pm);
}

void MementoArray::Clear()
{
	vector<Memento *>::iterator iter = m_ary.begin();
	while (iter != m_ary.end())
	{

		delete *iter;
		iter++;
	}
	m_ary.clear();
}

Memento *MementoArray::GetAt(int index)
{
	if(index >= 0 && index < (int)m_ary.size())
		return m_ary[index];
	else
		return false;
}

int MementoArray::GetCount()
{
	return m_ary.size();
}

/***********************************************************/
/*			     CMemento					     		   */
/***********************************************************/
Memento::Memento()
{
	m_pArray = new MementoArray();
}

Memento::~Memento()
{
	if(m_pArray) delete m_pArray;
}

string Memento::GetName()
{
	return m_tcName;
}

void Memento::SetName(string name)
{
	m_tcName = name;
}

bool Memento::GetBoolean(string key)
{
  string value = m_Attributes.GetValue(key); 
  if(value == "1")
    return true;
  else
    return false;
}

void Memento::SetBoolean(string key, bool value)
{
	string str;
	if(value)
		str = "1";
	else
		str = "0";

	m_Attributes.Add(key,str);
}

int Memento::GetInteger(string key)
{
	return atoi(m_Attributes.GetValue(key).c_str());
}

void Memento::SetInteger(string key, int value)
{
	string str;
	char t[30];
	sprintf(t, "%d", value);
	str = t;
	m_Attributes.Add(key,str);
}

double Memento::GetFloat(string key)
{
	float f = 0;
	string str = m_Attributes.GetValue(key);
	return atof(str.c_str());
}

void Memento::SetFloat(string key, float value)
{
	string str;
	char t[30];
	sprintf(t, "%f", value);
	str = t;
	m_Attributes.Add(key,str);
}

string Memento::GetString(string key)
{
	return m_Attributes.GetValue(key);
}

void Memento::SetString(string key, string value)
{
	m_Attributes.Add(key,value);
}

int Memento::GetAttributeCount()
{
	return m_Attributes.GetCount();
}

string Memento::GetAttributeName(int index)
{
	return this->m_Attributes.GetKey(index);
}

string Memento::GetAttributeValue(int index)
{
	return this->m_Attributes.GetValue(index);
}

Memento *Memento::CreateChild(string type)
{
	Memento *sub = new Memento();
	sub->SetName(type);
	this->m_SubMementos.Add(type,sub);
	return sub;
}

int Memento::GetChildCount()
{
	return this->m_SubMementos.GetCount();
}

Memento *Memento::GetChild(int index)
{
	return m_SubMementos.GetObject(index);
}

Memento *Memento::GetChild(string type)
{
	return m_SubMementos.GetObject(type);
}

MementoArray *Memento::GetChildren(string type)
{
	m_pArray->Clear();

	for(int i = 0; i<m_SubMementos.GetCount(); i++ )
	{
		if(m_SubMementos.GetKey(i) == type)
			m_pArray->Add(m_SubMementos.GetObject(i));
	}
	return m_pArray;
}


