include(CMakeParseArguments)
enable_testing()
option(ENABLE_INTEGRATION_TESTS "control if integrations are bulit and ran" ON)
option(ENABLE_UNIT_TESTS "control if unit tests are bulit and ran" ON)
option(ENABLE_BENCHMARK_TESTS "control if benchmarks are bulit and ran" OFF)
set(INTEGRATION_TESTS "")
set(UNIT_TESTS "")
set(BENCHMARK_TESTS "")
set(TEST_RUNNER ${PROJECT_SOURCE_DIR}/src/test_runner.py)

message(STATUS "ENABLE_INTEGRATION_TESTS=${ENABLE_INTEGRATION_TESTS}")
message(STATUS "ENABLE_UNIT_TESTS=${ENABLE_UNIT_TESTS}")
message(STATUS "ENABLE_BENCHMARK_TESTS=${ENABLE_BENCHMARK_TESTS}")

function (smf_test)
  set(options INTEGRATION_TEST UNIT_TEST BENCHMARK_TEST)
  set(oneValueArgs BINARY_NAME SOURCE_DIRECTORY)
  set(multiValueArgs SOURCES LIBRARIES)
  cmake_parse_arguments(SMF_TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(SMF_TEST_INTEGRATION_TEST AND ENABLE_INTEGRATION_TESTS)
    set(SMF_TEST_BINARY_NAME "${SMF_TEST_BINARY_NAME}_integration_test")
    set(INTEGRATION_TESTS "${INTEGRATION_TESTS} ${SMF_TEST_BINARY_NAME}")
    add_executable(
      ${SMF_TEST_BINARY_NAME} "${SMF_TEST_SOURCES}")
    target_link_libraries(
      ${SMF_TEST_BINARY_NAME}
      PUBLIC "${SMF_TEST_LIBRARIES}")
    add_test (
      NAME ${SMF_TEST_BINARY_NAME}
      COMMAND
      ${TEST_RUNNER}
      --type integration
      --binary $<TARGET_FILE:${SMF_TEST_BINARY_NAME}>
      --directory ${SMF_TEST_SOURCE_DIRECTORY}
      )
  endif()
  if(SMF_TEST_UNIT_TEST AND ENABLE_UNIT_TESTS)
    set(SMF_TEST_BINARY_NAME "${SMF_TEST_BINARY_NAME}_unit_test")
    set(UNIT_TESTS "${UNIT_TESTS} ${SMF_TEST_BINARY_NAME}")
    add_executable(
      ${SMF_TEST_BINARY_NAME} "${SMF_TEST_SOURCES}")
    target_link_libraries(
      ${SMF_TEST_BINARY_NAME} "${SMF_TEST_LIBRARIES}")
    add_test (
      NAME ${SMF_TEST_BINARY_NAME}
      COMMAND
      ${TEST_RUNNER}
      --type unit
      --binary $<TARGET_FILE:${SMF_TEST_BINARY_NAME}>
      --directory ${SMF_TEST_SOURCE_DIRECTORY}
      )
  endif()
  if(SMF_TEST_BENCHMARK_TEST AND ENABLE_BENCHMARK_TESTS)
    set(SMF_TEST_BINARY_NAME "${SMF_TEST_BINARY_NAME}_benchmark_test")
    set(BENCHMARK_TESTS "${BENCHMARK_TESTS} ${SMF_TEST_BINARY_NAME}")
    add_executable(
      ${SMF_TEST_BINARY_NAME} "${SMF_TEST_SOURCES}")
    target_link_libraries(
      ${SMF_TEST_BINARY_NAME} "${SMF_TEST_LIBRARIES}")
    add_test (
      NAME ${SMF_TEST_BINARY_NAME}
      COMMAND
      ${TEST_RUNNER}
      --type benchmark
      --binary $<TARGET_FILE:${SMF_TEST_BINARY_NAME}>
      --directory ${SMF_TEST_SOURCE_DIRECTORY}
      )
  endif()
endfunction ()


add_custom_target(check
  COMMAND ctest --output-on-failure
  DEPENDS "${UNIT_TESTS} ${INTEGRATION_TESTS} ${BENCHMARK_TESTS}")
