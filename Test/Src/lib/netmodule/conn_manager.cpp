#include "conn_manager.h"

#include <vector>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

#include "connection.h"

wmdf::ConnManager* wmdf::ConnManager::instance_=NULL;

class wmdf::ConnManager::ConnGroupManagerImpl
{
public:
  ConnGroupManagerImpl(int max_connected,wmdf::Connection**connections)
    :current_start_id_(0),
    connections_(connections),
    size_(max_connected)
  {
  }
  int AddConnectionGroup();
  void AddConnection(int group_id,int socket_id);
  void RemoveConnection(int group_id,int socket_id);
	int GetConnectionCount(int group_id);
  void RemoveConnection(int socket_id);
  void GetConnection(int socket_id);
	void CloseConnection(int socket_id);
  //返回值为connection的socket_id
  int AddConnection(wmdf::Connection* connection);
  void Close();
  void SendMsg(int socket_id,Message* msg);
  void BroadcastMsg(int group_id,Message* msg);
private:
  typedef boost::shared_ptr<wmdf::Connection> ConnectionPtr;
  typedef boost::shared_ptr<Message> MessagePtr;
  typedef std::vector<int> ConnectionPos;
  std::vector<ConnectionPos> connection_group_;
  boost::mutex mutex_;
  int current_start_id_;
  wmdf::Connection** connections_;
  int size_;
};

int wmdf::ConnManager::ConnGroupManagerImpl::AddConnectionGroup()
{
  connection_group_.push_back(ConnectionPos());
  return connection_group_.size()-1;
}

void wmdf::ConnManager::ConnGroupManagerImpl::AddConnection( int group_id,int socket_id )
{
  if((size_t)group_id>=connection_group_.size())
    assert(0&&"group not exist");
  boost::mutex::scoped_lock lock(mutex_);
  connection_group_[group_id].push_back(socket_id);
}

void wmdf::ConnManager::ConnGroupManagerImpl::RemoveConnection( int group_id,int socket_id )
{
  if((size_t)group_id>=connection_group_.size())
    assert(0&&"group not exist");
  ConnectionPos::iterator iter = connection_group_[group_id].begin();
  boost::mutex::scoped_lock lock(mutex_);
  for (;iter!=connection_group_[group_id].end();++iter)
  {
    if(*iter==socket_id)
    {
      connection_group_[group_id].erase(iter);
      return;
    }
  }

}

void wmdf::ConnManager::ConnGroupManagerImpl::RemoveConnection( int socket_id )
{
  boost::mutex::scoped_lock lock(mutex_);
  for(std::vector<ConnectionPos>::iterator iter1=connection_group_.begin();
    iter1!=connection_group_.end();++iter1)
  {
    for (ConnectionPos::iterator iter2=iter1->begin();iter2!=iter1->end();++iter2)
    {
      if(*iter2==socket_id)
      {
        iter1->erase(iter2);
        break;
      }
    }
  }
  if(socket_id<size_){
    //connections_[size_]->Close();
    connections_[socket_id]=NULL;
  }
}

int wmdf::ConnManager::ConnGroupManagerImpl::AddConnection( wmdf::Connection* connection )
{
  boost::mutex::scoped_lock lock(mutex_);
  for(int i=current_start_id_;i<size_;i++)
  {
    if(connections_[i]==NULL)
    {
      connections_[i]=connection;
      current_start_id_=i;
      return i;
    }
  }
  for(int i=0;i<current_start_id_;i++)
  {
    if(connections_[i]==NULL)
    {
      connections_[i]=connection;
      current_start_id_=i;
      return i;
    }
  }
  return -1;
}


void wmdf::ConnManager::ConnGroupManagerImpl::Close()
{
	if (connection_group_.size() <= 0) return;
  ConnectionPos group=connection_group_[0];
  wmdf::Connection* connection=NULL;
  for(ConnectionPos::iterator iter=group.begin();iter!=group.end();++iter)
  {
    connection=connections_[*iter];
    if(connection!=NULL)
      connection->Close();
  }
  connection_group_.clear();
  delete[] connections_;
  connections_=NULL;
}


void wmdf::ConnManager::ConnGroupManagerImpl::SendMsg( int socket_id,Message* msg )
{
  boost::mutex::scoped_lock lock(mutex_);
  wmdf::Connection* connection=connections_[socket_id];
  if(connection!=NULL)
  {
    MessagePtr msg_ptr(msg);
    connection->SendMsg(msg_ptr);
  }
}

void wmdf::ConnManager::ConnGroupManagerImpl::BroadcastMsg( int group_id,Message* msg )
{
  boost::mutex::scoped_lock lock(mutex_);
  assert(connection_group_.size()>(uint32_t)group_id);
  ConnectionPos group=connection_group_[group_id];
  wmdf::Connection* connection=NULL;
  //ReleaseMessage
  
  if(group.size()==0){
	  ReleaseMessage(msg);
	  return;
  }
  MessagePtr msg_ptr(msg);
  for(ConnectionPos::iterator iter=group.begin();iter!=group.end();++iter)
  {
    connection=connections_[*iter];
    if(connection!=NULL)
      connection->SendMsg(msg_ptr);
  }
}

int wmdf::ConnManager::ConnGroupManagerImpl::GetConnectionCount( int group_id )
{
	boost::mutex::scoped_lock lock(mutex_);
	assert(connection_group_.size()>(uint32_t)group_id);
	return connection_group_[group_id].size();
}

void wmdf::ConnManager::ConnGroupManagerImpl::CloseConnection( int socket_id )
{
	
  if((socket_id<size_) && (NULL!=connections_[socket_id])) {
		connections_[socket_id]->Close();
		connections_[socket_id]=NULL;
	}
}

wmdf::ConnManager::ConnManager( int max_connection )
{
  connections_ = new Connection*[max_connection];
  for(int i=0;i<max_connection;i++)
    connections_[i]=NULL;
  impl_=new wmdf::ConnManager::ConnGroupManagerImpl(max_connection,connections_);
}

wmdf::ConnManager::~ConnManager( void )
{
  Close();
  delete impl_;
}

int wmdf::ConnManager::AddConnectionGroup()
{
  return impl_->AddConnectionGroup();
}

void wmdf::ConnManager::AddConnection( int group_id,int socket_id )
{
  impl_->AddConnection(group_id,socket_id);
}

int wmdf::ConnManager::AddConnection( Connection* connection )
{
  return impl_->AddConnection(connection);
}

void wmdf::ConnManager::RemoveConnection( int group_id,int socket_id )
{
  impl_->RemoveConnection(group_id,socket_id);
}

void wmdf::ConnManager::RemoveConnection( int socket_id )
{
  impl_->RemoveConnection(socket_id);
}

void wmdf::ConnManager::Close()
{
  impl_->Close();
}


void wmdf::ConnManager::SendMsg( int socket_id,Message* msg )
{
  impl_->SendMsg(socket_id,msg);
}

void wmdf::ConnManager::BroadcastMsg( int group_id,Message* msg )
{
  impl_->BroadcastMsg(group_id,msg);
}

NET_API int wmdf::ConnManager::GetConnectionCount( int group_id )
{
	return impl_->GetConnectionCount(group_id);
}

void wmdf::ConnManager::CloseConnection( int socket_id )
{
	impl_->CloseConnection(socket_id);
}




