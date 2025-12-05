# Standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_minimum_required(VERSION 3.20)

project(pricing_lib
    VERSION 0.1.0
    LANGUAGES CXX
)

enable_testing()

add_executable(pricing_tests
    tests/test_main.cpp
    tests/test_payoff.cpp
    tests/test_black_formula.cpp
    tests/test_european_option.cpp
    tests/test_rates.cpp
)

target_link_libraries(pricing_tests
    PRIVATE
        pricing_core
)

target_include_directories(pricing_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party   # pour doctest/doctest.h
)

if (MSVC)
    target_compile_options(pricing_tests PRIVATE /W4 /permissive-)
else()
    target_compile_options(pricing_tests PRIVATE -Wall -Wextra -pedantic)
endif()

set_target_properties(pricing_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)

add_test(
    NAME pricing_tests
    COMMAND pricing_tests
)




# Library: pricing_core
add_library(pricing_core
    src/core/Instrument.cpp
    src/core/PricingEngine.cpp
    src/core/Payoff.cpp
    src/core/InstrumentFactory.cpp      
    src/core/EngineFactory.cpp         
    src/market/MarketData.cpp
    src/models/BlackScholesModel.cpp
    src/models/BlackIRModel.cpp
    src/products/EuropeanOption.cpp
    src/engines/EuropeanOptionBSEngine.cpp
    src/products/CapFloor.cpp
    src/engines/CapFloorEngines.cpp
    src/products/Swap.cpp
    src/engines/SwapEngines.cpp
    src/products/DigitalOption.cpp
    src/engines/DigitalOptionBSEngine.cpp
    src/products/AsianOption.cpp
    src/engines/AsianOptionMCEngine.cpp
    src/products/BarrierOption.cpp
    src/engines/BarrierOptionMCEngine.cpp
    src/utils/BlackFormula.cpp
)



add_executable(pricing_factory_example
    examples/main_factory.cpp
)

target_link_libraries(pricing_factory_example
    PRIVATE
        pricing_core
)

if (MSVC)
    target_compile_options(pricing_factory_example PRIVATE /W4 /permissive-)
else()
    target_compile_options(pricing_factory_example PRIVATE -Wall -Wextra -pedantic)
endif()



target_include_directories(pricing_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (MSVC)
    target_compile_options(pricing_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(pricing_core PRIVATE -Wall -Wextra -pedantic)
endif()

add_executable(pricing_example
    examples/main_basic.cpp
)

target_link_libraries(pricing_example
    PRIVATE
        pricing_core
)

if (MSVC)
    target_compile_options(pricing_example PRIVATE /W4 /permissive-)
else()
    target_compile_options(pricing_example PRIVATE -Wall -Wextra -pedantic)
endif()
