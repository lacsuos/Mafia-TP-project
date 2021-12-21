add_library(GAME SHARED
        Player/Player.cpp Player/Player.h
        Player/Citizen.cpp Player/Citizen.h
        Player/GameHost.cpp Player/GameHost.h
        Player/Mafia.cpp Player/Mafia.h
        PlayRoom/PlayRoom.cpp PlayRoom/PlayRoom.h
        )
target_include_directories(GAME PUBLIC Player
        PUBLIC PlayRoom)
