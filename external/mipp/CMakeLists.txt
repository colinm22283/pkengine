include(FetchContent)

cmake_policy(SET CMP0077 NEW)

set(MIPP_TESTS_EXE OFF)
set(MIPP_EXAMPLES_EXE OFF)

FetchContent_Declare(
        mipp_content
        GIT_REPOSITORY git@github.com:aff3ct/MIPP.git
        GIT_TAG f3cd53611cc3a9688e30632b8c1d13eccb701aa7
)
FetchContent_MakeAvailable(mipp_content)
