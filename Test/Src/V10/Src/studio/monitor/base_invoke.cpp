#include "monitor_invoke.h"
#include <string>
#include <cstring>
#include <cstdio>

#include "../../include/wmdf_api.h"
#include "../../include/date_time.h"
#include "../common/studio_utility.h"
#include "server_manager.h"
using namespace wmdf;

BaseInvoke::BaseInvoke()
{
  wmdf_initialize();
}

BaseInvoke::~BaseInvoke(void)
{
  wmdf_finalize();
}




