include(FetchContent)

# --------------------------------------------------------------------

# Offline mode (uncomment next line to enable)
# set(FETCHCONTENT_FULLY_DISCONNECTED ON)

# set(FETCHCONTENT_QUIET OFF)

# --------------------------------------------------------------------

# Libraries

# --------------------------------------------------------------------

# fmt with println

project_log("FetchContent: fmt")

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 87c066a35b7cc70bb7d438a543c8b49b577e61f4
)
FetchContent_MakeAvailable(fmt)

# --------------------------------------------------------------------

# Unique Function

project_log("FetchContent: function2")

FetchContent_Declare(
        function2
        GIT_REPOSITORY https://github.com/Naios/function2.git
        GIT_TAG 4.2.2
)
FetchContent_MakeAvailable(function2)

# --------------------------------------------------------------------

project_log("FetchContent: wheels")

FetchContent_Declare(
        wheels
        GIT_REPOSITORY https://gitlab.com/Lipovsky/wheels.git
        GIT_TAG 8a263b4b2ad16f7f93b099920b6f59bb5c5c98fe
)
FetchContent_MakeAvailable(wheels)

# --------------------------------------------------------------------

project_log("FetchContent: sure")

FetchContent_Declare(
        sure
        GIT_REPOSITORY https://gitlab.com/Lipovsky/sure.git
        GIT_TAG c99a4647f9a7297f5805f07f3c10f166ae86b8d6
)
FetchContent_MakeAvailable(sure)

# --------------------------------------------------------------------

project_log("FetchContent: twist")

FetchContent_Declare(
        twist
        GIT_REPOSITORY https://gitlab.com/Lipovsky/twist.git
        GIT_TAG 3295ef696c934f282f2a8256ceb7702a18856bd3
)
FetchContent_MakeAvailable(twist)

# --------------------------------------------------------------------

project_log("FetchContent: tinyfibers")

FetchContent_Declare(
        tinyfibers
        GIT_REPOSITORY https://gitlab.com/Lipovsky/tinyfibers.git
        GIT_TAG f51e497872b8577b32a163f96d1ff93e47dd745e
)
FetchContent_MakeAvailable(tinyfibers)

# --------------------------------------------------------------------

project_log("FetchContent: asio")

FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-22-1
)
FetchContent_MakeAvailable(asio)

add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
