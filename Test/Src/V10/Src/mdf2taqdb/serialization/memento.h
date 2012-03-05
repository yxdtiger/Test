#if !defined MEMENTO_H
#define MEMENTO_H

#include <string>
#include <vector>

using namespace std;

namespace wmdf 
{

class Memento;

class KeyValue
{
public:
	string csKey;
	string csValue;
};

class KeyObject
{
	public:
	string csKey;
	Memento * pMemento;
};

class  KeyValueCollector
{
public:
	KeyValueCollector(){};
	virtual ~KeyValueCollector();
private:
	vector<KeyValue> m_list;
public:
	void Add(string key, string value);
	bool Remove(string key);
	bool Remove(int index);
	bool RemoveByValue(string value);
	void Clear();
	int GetCount();
	string GetKey(int index);
	string GetValue(int index);
	string GetValue(string key);
};

class  KeyObjectCollector
{
public:
	KeyObjectCollector(){};
	virtual ~KeyObjectCollector();
private:
	vector<KeyObject> m_list;
public:
	void Add(string key, Memento * object);
	bool Remove(string key);
	bool Remove(int index);
	bool Remove(Memento * object);
	void Clear();
	int GetCount();
	string GetKey(int index);
	Memento * GetObject(int index);
	Memento * GetObject(string key);
};

class MementoArray
{
public:
	MementoArray(){};
	virtual ~MementoArray(){
		Clear();
	};
private:
	vector<Memento*> m_ary;
public:
	Memento *GetAt(int index);
	int GetCount();
	void Clear();
	void Add(Memento * pm);
};

 class  Memento
{
public:
	Memento();
	virtual ~Memento();
private:
	string m_tcName;
	KeyValueCollector m_Attributes;
	KeyObjectCollector m_SubMementos;
	MementoArray *m_pArray;

public:
	string GetName();
	void SetName(string name);
	bool GetBoolean(string key);
	void SetBoolean(string key, bool value);
	int GetInteger(string key);
	void SetInteger(string key, int value);
	double GetFloat(string key);
	void SetFloat(string key, float value);
	string GetString(string key);
	void SetString(string key, string value);
	int GetAttributeCount();
	string GetAttributeName(int index);
	string GetAttributeValue(int index);
	Memento *CreateChild(string type);
	int GetChildCount();
	Memento *GetChild(int index);
	Memento *GetChild(string type);
	MementoArray * GetChildren(string type);
};
}
#endif
