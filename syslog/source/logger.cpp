

#include <sstream>

#include "logger.h"

logger *logger::instance = nullptr;

logger::logger(const char *name) {
    instance = this;
    setlogmask(LOG_UPTO(LOG_ERR | LOG_WARNING | LOG_INFO | LOG_DEBUG | LOG_NOTICE));
    openlog(name, LOG_NDELAY, LOG_LOCAL7);
}

logger::~logger(void) {
    instance = nullptr;
    closelog();
}

duration::duration(const char *fname) {
    t_start = std::chrono::steady_clock::now();
    LogN("In : %s \n", fname);
    function = fname;
}

duration::~duration() {;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::stringstream ts;
    ts <<  function << "[" << (end - t_start).count() << "ns ]";
    LogN("Out : %s\n",ts.str().c_str());
}