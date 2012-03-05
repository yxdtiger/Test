#ifndef REPLAY_ADAPTER_H
#define REPLAY_ADAPTER_H

#include "../session/feed_client.h"

namespace wmdf
{
  class ProtocolDescription;

  class ReplayAdapter:public IClientCallback
  {
  public:
    ReplayAdapter(ProtocolDescription* protocol_description);
    ~ReplayAdapter();
    virtual void OnRecvMsg(Message* msg);
    virtual void OnCloseCallback(EventMsg* args);
    virtual void OnEventCallback(EventMsg* args);
    virtual void ResetContext();
  private:
    ProtocolDescription* protocol_description_;
  };
}

#endif
