#ifndef POLLINGCONDITIONS_POLLINGCONDITIONS_H
#define POLLINGCONDITIONS_POLLINGCONDITIONS_H

#include <iostream>
#include <functional>
#include <chrono>
#include <string>
#include <thread>
#include <utility>

namespace dev::marcinromanowski {

    class PollingConditionsException : public std::exception {
    public:
        explicit PollingConditionsException(std::string reason) : reason{std::move(reason)} {

        }

        [[nodiscard]] const char* what() const noexcept override {
            return reason.c_str();
        }

    private:
        const std::string reason;
    };

    template<typename Rep, typename Period>
    class ConditionNotSatisfied : public PollingConditionsException {
    public:
        explicit ConditionNotSatisfied(const std::chrono::duration<Rep, Period> timeout)
            : PollingConditionsException{"Condition not satisfied after " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(timeout).count()) + "s"} {

        }
    };

    template<typename Rep, typename Period>
    class ConditionNotMaintained : public PollingConditionsException {
    public:
        explicit ConditionNotMaintained(const std::chrono::duration<Rep, Period> timeout)
            : PollingConditionsException{"Condition not maintained within " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(timeout).count()) + "s"} {

        }
    };

    template<typename Rep, typename Period>
    class PollingConditions {
    public:
        explicit PollingConditions(
                const std::chrono::duration<Rep, Period>& timeout = std::chrono::milliseconds(10000),
                const std::chrono::duration<Rep, Period>& initialDelay = std::chrono::milliseconds(0),
                const std::chrono::duration<Rep, Period>& delay = std::chrono::milliseconds(100),
                const double& factor = 1.0)
                : timeout{timeout},
                  initialDelay{initialDelay},
                  delay{delay},
                  factor{factor} {

        }

        inline std::chrono::duration<Rep, Period> getTimeout() const {
            return timeout;
        }

        inline std::chrono::duration<Rep, Period> getInitialDelay() const {
            return initialDelay;
        }

        inline std::chrono::duration<Rep, Period> getDelay() const {
            return delay;
        }

        [[nodiscard]] inline double getFactor() const {
            return factor;
        }

        inline void eventually(const std::function<bool()>& closure) const {
            if (!isConditionReached(closure)) {
                throw ConditionNotSatisfied<Rep, Period>(timeout);
            }
        }

        inline void constantly(const std::function<bool()>& closure) const {
            if (isConditionReached([&closure]() -> bool { return !closure(); })) {
                throw ConditionNotMaintained<Rep, Period>(timeout);
            }
        }

    private:
        bool isConditionReached(const std::function<bool()>& closure) const {
            if (initialDelay.count() > 0) {
                std::this_thread::sleep_for(initialDelay);
            }

            std::chrono::milliseconds startTime = getCurrentTimeMs();
            std::chrono::milliseconds currentDelay = std::chrono::duration_cast<std::chrono::milliseconds>(delay);
            std::chrono::milliseconds elapsedTime = std::chrono::milliseconds(0);

            while (elapsedTime <= timeout) {
                try {
                    if (closure()) {
                        return true;
                    }
                } catch (const std::exception& ex) {
                    std::cout << "Got unexpected exception, reason: " << ex.what() << std::endl;
                }

                elapsedTime = getCurrentTimeMs() - startTime;
                std::chrono::milliseconds sleepMillis = durationMinMs(currentDelay, startTime + timeout - getCurrentTimeMs());

                if (sleepMillis.count() > 0) {
                    std::this_thread::sleep_for(sleepMillis);
                }

                currentDelay *= factor;
            }

            return false;
        }

        [[nodiscard]] inline std::chrono::milliseconds getCurrentTimeMs() const {
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        }

        template<typename U, typename V>
        inline std::chrono::milliseconds durationMinMs(const U& firstDuration, const V& secondDuration) const {
            using CommonType = typename std::common_type<U, V>::type;
            const auto firstDurationAsCommonType = std::chrono::duration_cast<CommonType>(firstDuration);
            const auto secondDurationAsCommonType = std::chrono::duration_cast<CommonType>(secondDuration);
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::min(firstDurationAsCommonType, secondDurationAsCommonType));
        }

        const std::chrono::duration<Rep, Period> timeout;
        const std::chrono::duration<Rep, Period> initialDelay;
        const std::chrono::duration<Rep, Period> delay;
        const double factor;
    };

}

#endif //POLLINGCONDITIONS_POLLINGCONDITIONS_H
