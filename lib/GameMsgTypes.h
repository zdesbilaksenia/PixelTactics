#pragma once

enum class GameMsgTypes : uint32_t {
    ErrorMessage,
    ServerAccept,
    ConnectToLobby,
    LobbyWaitingForPlayer,
    LobbyFull,
    LobbyGameStart,
    LobbyGameOver,
    LobbyLeave,

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
    GameAttackRequest,
    GameTakeCard,
    GamePowerRequest,
    GamePowerAnswer,
    GameBeforeHeroStats,
    GameRemoveBodyRequest,
    GameRemoveBody,
    GameCardReleased,
    GameReject,
    GameWon,
    GameLost
};

