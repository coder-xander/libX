﻿file(GLOB_RECURSE headerFiles "*.h")
file(GLOB_RECURSE sourceFiles "*.cpp")
add_library(xtask SHARED ${headerFiles} ${sourceFiles})
set_target_properties(xtask PROPERTIES LINKER_LANGUAGE CXX)
