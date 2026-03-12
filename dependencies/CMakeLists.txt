include(FetchContent)

#==================
# MattCore
#==================
#FetchContent_Declare(mattcore
#    GIT_REPOSITORY https://github.com/matheoheo/MattCore
#    GIT_TAG 14705a7a62f39fe964ab67a23a178f3e87dc7bfc)

#mattcore is treated as git submodule, so doesn't require fetching.
add_subdirectory(mattcore)

#==================
# GoogleTests
#==================
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
add_subdirectory(googletest)
