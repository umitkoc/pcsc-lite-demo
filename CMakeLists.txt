cmake_minimum_required(VERSION 3.27)
project(program)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
        main.cpp
        winscard.c
        sys_unix.c
        debuglog.c
        readerfactory.c
        ifdwrapper.c
        atrhandler.c
        prothandler.c
        strlcpy.c
        strlcat.c
        eventhandler.c
        simclist.c
        dyn_unix.c
        configfile.c
        utils.c
        winscard_svc.c
        winscard_msg.c
        auth.c
        winscard_clnt.c
)
