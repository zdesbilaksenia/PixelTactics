#pragma once

enum class GameMsgTypes : uint32_t {
    ErrorMessage,
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
    MakeLobby,
    GetLobbies,
    JoinLobby,
    LobbyEmpty,
    LobbyWaitingForPlayer,
    LobbyFull,
    LobbyGameStart,
    LobbyGameOver,
    GameLeaderChoice,
    LobbyPlayerDisconnected,
    LobbyLeave,
    GetHeroesStats,
    HeroesStats,
    GameHelloToPlayer,
};

