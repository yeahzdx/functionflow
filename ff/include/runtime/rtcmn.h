#ifndef FF_RUNTIME_RTCMN_H_
#define FF_RUNTIME_RTCMN_H_
#include <thread>
#include <atomic>
#include <mutex>
#include "common/log.h"


namespace ff {
namespace rt {

//Give other tasks opportunities to run!
void yield();

}//end namespace rt
}//end namespace ff
#endif
