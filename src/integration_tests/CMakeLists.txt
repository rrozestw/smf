set(IT_ROOT ${PROJECT_SOURCE_DIR}/src/integration_tests)

smf_test(
  INTEGRATION_TEST
  BINARY_NAME histograms
  SOURCES ${IT_ROOT}/histograms/main.cc
  SOURCE_DIRECTORY ${IT_ROOT}/histograms
  LIBRARIES smf_filesystem
  )
smf_test(
  INTEGRATION_TEST
  BINARY_NAME rpc
  SOURCES ${IT_ROOT}/rpc/main.cc
  SOURCE_DIRECTORY ${IT_ROOT}/rpc
  LIBRARIES smf_rpc
  )
if(ENABLE_INTEGRATION_TESTS)
  add_dependencies(
    rpc_integration_test
    generated_demo_service
    )
endif()
smf_test(
  INTEGRATION_TEST
  BINARY_NAME rpc_recv_timeout
  SOURCES ${IT_ROOT}/rpc_recv_timeout/main.cc
  SOURCE_DIRECTORY ${IT_ROOT}/rpc_recv_timeout
  LIBRARIES smf_rpc
  )

if(ENABLE_INTEGRATION_TESTS)
  add_dependencies(
    rpc_recv_timeout_integration_test
    generated_demo_service
    )
endif()
smf_test(
  INTEGRATION_TEST
  BINARY_NAME wal
  SOURCES ${IT_ROOT}/wal/main.cc
  SOURCE_DIRECTORY ${IT_ROOT}/wal
  LIBRARIES smf_filesystem smf_rpc smf_cr
  )
smf_test(
  INTEGRATION_TEST
  BINARY_NAME clock_pro
  SOURCES ${IT_ROOT}/wal_clock_pro_cache/main.cc
  SOURCE_DIRECTORY ${IT_ROOT}/wal_clock_pro_cache
  LIBRARIES smf_filesystem
  )
smf_test(
  INTEGRATION_TEST
  BINARY_NAME wal_segment
  SOURCES ${IT_ROOT}/wal_segment/main.cc
  SOURCE_DIRECTORY ${IT_ROOT}/wal_segment
  LIBRARIES smf_filesystem flatbuffers
  )
