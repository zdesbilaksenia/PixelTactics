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
    LobbyPlayerDisconnected,
    LobbyLeave,
    GetHeroesStats,
    HeroesStats,

    GameDeck,
    GameHeroesStats,
    GameCanBeAttacked,
    GameLeaderChoice,
    GamePlayerOptionChoice,
    GameFullCoordinates,
    GameShortCoordinates,
    GameHelloToPlayer,
    GameCardFromHandChoice,
    GameStr,
    GameString,
    GameAttackRequest

};

