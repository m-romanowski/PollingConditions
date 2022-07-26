# PollingConditions
Repeatedly evaluates one or more conditions until they are satisfied or a timeout has elapsed. The timeout and delays
between evaluation attempts are configurable. It can be used for testing purposes where operation may  take a some
time (asynchronous). The [Spock framework](https://github.com/spockframework/spock) solution was the  inspiration for
this implementation.

## Requirements
- cmake _>= 3.22_
- libstdc++ _>= 4.6, c++11 support is required_

## Usage
There are provided some predefined polling conditions with default timeouts:
- __SHORT_WAIT__ (1 second)
- __WAIT__ (10 seconds)
- __LONG_WAIT__ (30 seconds)

```c++
#include "predefinedpollingconditions.h"

PredefinedPollingConditions::SHORT_WAIT.eventually([]() -> bool {
    ...
});
```

or you can create a custom one
```c++
#include "pollingconditions.h"

auto customPollingCondition = PollingConditions(std::chrono::milliseconds(5000), std::chrono::milliseconds(0), std::chrono::milliseconds(100), 1.0);
customPollingCondition.eventually([]() -> bool {
    ...
});
```

## Build

You can build project using cmake
```
cmake .
make -j <available processors>
```

## Built with
* [cmake](https://cmake.org)

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
