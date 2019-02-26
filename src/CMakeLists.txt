project(SimpleAssets)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( SimpleAssets SimpleAssets SimpleAssets.cpp )
target_include_directories( SimpleAssets PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( SimpleAssets ${CMAKE_SOURCE_DIR}/../ricardian )