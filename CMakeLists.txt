include(ExternalProject)
# if no cdt root is given use default path
if(EOSIO_CDT_ROOT STREQUAL "" OR NOT EOSIO_CDT_ROOT)
   find_package(eosio.cdt)
endif()

set(ICON_BASE_URL          "https://cryptolions.io/assets/images/sa-icons-256")

set(ATTACH_ICON_URI        "attach.png#0b039adbbe1011a494959963917df4765dbae378004e00911464f865e58ef452")
set(ATTACHF_ICON_URI       "attachf.png#2d190203c6ae474e01cd70963c82a54cb542d95e29174e7572acb8c7653d8a8c")
set(AUTHORUPDATE_ICON_URI  "authorupdate.png#0b11c9c4e41b6ba1b00bd907c671f7ddc9e2f9caf26580f0b2e7c73e02f36ff3")
set(BURN_ICON_URI          "burn.png#76954820d84563187e968b461a706ab089a9c00f5332df96a4e5bb2e69ed5e84")
set(CANCELOFFER_ICON_URI   "canceloffer.png#83328bae49f18f0aeca6d04c2e3003e49637a462b44a492d3439a9c44724a654")
set(CANCELOFFERF_ICON_URI  "cancelofferf.png#9c35f488e2c53464f430e0e31e5a4b3bd2e914fe3b6db126722166a8a408fa83")
set(CLAIMF_ICON_URI        "claimf.png#80086207646470fa9cbaf4638ba0dedec96ea0e84d5059b567adc9e4b8543c30")
set(CREATE_ICON_URI        "create.png#db2bc53cef5dbbce5247543a91234373ec4bd1baf743301a0d92a35b053eeb70")
set(CREATEF_ICON_URI       "createf.png#5fb6788f19b8fb08ce46e765a9b0ae8758cba7e4da9895f840d34d143912fd3d")
set(DELEGATE_ICON_URI      "delegate.png#f8d91c1b9c0f376464e69aadf41142a17492af1e75d394601872b6a992b420bc")
set(DELEGATE_MORE_ICON_URI "delegate_more.png#6d1f7319569e3780e25cfb717d83f4ad58c91c0890ab234f106d1593956dfd10")
set(DETACH_ICON_URI        "detach.png#a075de69e2cf8eed78306834cc5688b64ce6ed660c0db762ae20c7c7a1d19a6b")
set(DETACHF_ICON_URI       "detachf.png#2476bc6b7c02a4ec7e0e307b60f44a02486b4bebb4d91714817e5816063198e8")
set(ISSUEF_ICON_URI        "issuef.png#a40d77b2162d646cd3ce0488dfeab1cdfae8801fcc1d0c6ef1f16f2547b16551")
set(OFFER_ICON_URI         "offer.png#4aea6612c87df6cde081689f861b55ddba2c277cc553f4faeb65a4cb26656c66")
set(OFFERF_ICON_URI        "offerf.png#a34462918e536372610a523225e5221cd2685eff3c329499ded7298907f8524b")
set(REGAUTHOR_ICON_URI     "regauthor.png#c6a539be8e7dfd1a4c466ba9cabfd13571cd77d5c988c652d2e8f87096f3548e")
set(TRANSFER_ICON_URI      "transfer.png#23227a4901414db5cd0973d1cc5df13f9b82375b21c6315fd3402cb4acd50cbf")
set(UPDATE_ICON_URI        "update.png#305b640fe614876c6e6f5fed5ac21f8157c80f7bee39f3db26228986c1fc5e0e")
set(UPDATEF_ICON_URI       "updatef.png#f5d5af2ed6af508777f9f0b3c3a8e24b61eeca2ea623cfec6f6d0e9ea2c4d1a7")
set(CLAIM_ICON_URI         "claim.png#8c3fdf140ea14a3cb9762a0f6ec0f0d73f8c2eeeec3ea2f109f394a9f35f472d")

configure_file( ${CMAKE_SOURCE_DIR}/ricardian/SimpleAssets.contracts.md.in ${CMAKE_SOURCE_DIR}/ricardian/SimpleAssets.contracts.md @ONLY )

ExternalProject_Add(
   SimpleAssets_project
   SOURCE_DIR ${CMAKE_SOURCE_DIR}/src
   BINARY_DIR ${CMAKE_BINARY_DIR}/SimpleAssets
   CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${EOSIO_CDT_ROOT}/lib/cmake/eosio.cdt/EosioWasmToolchain.cmake
   UPDATE_COMMAND ""
   PATCH_COMMAND ""
   TEST_COMMAND ""
   INSTALL_COMMAND ""
   BUILD_ALWAYS 1
)