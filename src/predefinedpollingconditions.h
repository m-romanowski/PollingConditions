#ifndef POLLINGCONDITIONS_PREDEFINEDPOLLINGCONDITIONS_H
#define POLLINGCONDITIONS_PREDEFINEDPOLLINGCONDITIONS_H

#include "pollingconditions.h"

namespace dev::marcinromanowski {

    class PredefinedPollingConditions {
    public:
        inline static const PollingConditions SHORT_WAIT = PollingConditions(std::chrono::milliseconds(1000), std::chrono::milliseconds(0), std::chrono::milliseconds(100), 1.0);
        inline static const PollingConditions WAIT = PollingConditions(std::chrono::milliseconds(10000), std::chrono::milliseconds(0), std::chrono::milliseconds(100), 1.0);
        inline static const PollingConditions LONG_WAIT = PollingConditions(std::chrono::milliseconds(30000), std::chrono::milliseconds(0), std::chrono::milliseconds(100), 1.0);
    };

}

#endif //POLLINGCONDITIONS_PREDEFINEDPOLLINGCONDITIONS_H
