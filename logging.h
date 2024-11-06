#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>

#include "LogManager.h"


#ifndef PLUGIN_DEBUG

#ifdef ENABLE_DEBUG
#define PLUGIN_DEBUG(x) if (LogManager::get()->getLoglevel() >= LL_DEBUG) {std::cout << "[HyperX CC] " << x << std::endl;}
#else
#define PLUGIN_DEBUG(x)
#endif

#endif

#endif // LOGGING_H
