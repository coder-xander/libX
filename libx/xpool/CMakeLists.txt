﻿file(GLOB_RECURSE headerFiles "*.h")
file(GLOB_RECURSE sourceFiles "*.cpp")
add_library(xpool SHARED ${headerFiles} ${sourceFiles})