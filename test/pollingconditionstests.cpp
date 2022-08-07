#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE LOGCPLUS_TESTS

#include <boost/test/unit_test.hpp>
#include <stdexcept>

#include "predefinedpollingconditions.h"

namespace dev::marcinromanowski {

    BOOST_AUTO_TEST_CASE(conditionsShouldBeEventuallyProcessed) {
        // expect
        BOOST_CHECK_NO_THROW(PredefinedPollingConditions::SHORT_WAIT.eventually([]() -> bool {
            return true;
        }));
    }

    BOOST_AUTO_TEST_CASE(timeoutExceptionShouldBeThrownIfConditionIsNotPassedWithTimeout) {
        // expect
        BOOST_CHECK_EXCEPTION(PredefinedPollingConditions::WAIT.eventually([]() -> bool {
            return false;
        }), PollingConditionsException, [](PollingConditionsException const& ex) -> bool {
            return std::string(ex.what()).find("Condition not satisfied after 10s") != std::string::npos;
        });
    }

    BOOST_AUTO_TEST_CASE(unexpectedExceptionShouldntStopPollingCondition) {
        // setup
        auto assertionAttempts = 0;

        // expect
        BOOST_CHECK_NO_THROW(PredefinedPollingConditions::WAIT.eventually([&assertionAttempts]() -> bool {
            if (assertionAttempts >= 10) {
                return true;
            }

            assertionAttempts++;
            throw std::invalid_argument("Mocked exception");
        }));
        BOOST_CHECK(assertionAttempts == 10);
    }

    BOOST_AUTO_TEST_CASE(conditionShouldBeMaintainedForCertainPeriod) {
        // expect
        BOOST_CHECK_NO_THROW(PredefinedPollingConditions::SHORT_WAIT.constantly([]() -> bool {
            return true;
        }));
    }

    BOOST_AUTO_TEST_CASE(timeoutExceptionShouldBeThrownIfConditionIsNotMaintainedWithTimeout) {
        // setup
        auto assertionAttempts = 0;

        // expect
        BOOST_CHECK_EXCEPTION(PredefinedPollingConditions::WAIT.constantly([&assertionAttempts]() -> bool {
            return ++assertionAttempts < 5;
        }), PollingConditionsException, [](PollingConditionsException const& ex) -> bool {
            return std::string(ex.what()).find("Condition not maintained within 10s") != std::string::npos;
        });
        BOOST_CHECK(assertionAttempts == 5);
    }

}
