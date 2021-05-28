#include "Tile.h"
#include "configurations.cpp"

#include <iostream>
using namespace std;

Tile::Tile(RenderWindow &_window, Mouse &_mouse) : Clickable(_window, _mouse)
{
    unit = nullptr;
    coordX = 0;
    coordY = 0;
}

//Tile::Tile(RenderWindow &_window) : DrawableBox(_window) {};

void Tile::setUnit(Unit &_unit)
{
    this->unit = &_unit;
    if (unit->getHealth() == 0)
    {
        this->status = TileStatus::statusHasDeadBody;
    }
    else
    {
        this->status = TileStatus::statusHasUnit;
    }
    unit->setPosition(this->rect.getPosition().x + 20, this->rect.getPosition().y - 40);
}

void Tile::setStatus(TileStatus _status)
{
    this->status = _status;
}

Unit *Tile::getUnit()
{
    return unit;
}

int &Tile::getCoordX()
{
    return coordX;
}

int &Tile::getCoordY()
{
    return coordY;
}

void Tile::setCoordinates(const int &_x, const int &_y)
{
    coordX = _x;
    coordY = _y;
}

TileStatus Tile::getStatus()
{
    return status;
}

void Tile::draw()
{
    window.draw(rect);
    if (this->unit != nullptr)
    {
        unit->draw(mouse);
    }
}

Tile::~Tile()
{
    unit = nullptr;
}

//===========================================
//===============TilesManager================
//===========================================

auto initializeTilesLayer{
    [](RenderWindow &window, Mouse &mouse, vector<unique_ptr<Tile>> &tilesVector, vector<Tile *> &tiles, int round, Side side) {
        for (int i = 0; i < 3; ++i)
        {
            tilesVector.push_back(make_unique<Tile>(window, mouse));
            tilesVector[i]->setSize(tileWidth, tileHeight);
            tilesVector[i]->setStatus(TileStatus::statusIsEmpty);
            tilesVector[i]->setCoordinates(round, i);
            if (side == Side::sidePlayer)
            {
                tilesVector[i]->setPosition(150 * (3 - round) - 80, 100 * i + 200);
            }
            else
            {
                tilesVector[i]->setPosition(150 * (round + 1) + windowWidth - 520 - tileWidth, 100 * i + 200);
            }
            tiles.push_back(tilesVector[i].get());
        }
    }};

TilesManager::TilesManager(RenderWindow &_window, Mouse &_mouse, const Side &side, GameTcpClient &_client) : client(_client)
{
    status = TilesManagerStatus::statusNothingHappens;
    round = RoundType::roundAvangard;
    unitBuffer = nullptr;

    if (side == Side::sidePlayer)
    {
        initializeTilesLayer(_window, _mouse, tilesAvangard, tiles, 0, Side::sidePlayer);
        initializeTilesLayer(_window, _mouse, tilesFlank, tiles, 1, Side::sidePlayer);
        initializeTilesLayer(_window, _mouse, tilesRear, tiles, 2, Side::sidePlayer);
    }
    else if (side == Side::sideOpponent)
    {
        //Тайлы авангарда
        initializeTilesLayer(_window, _mouse, tilesAvangard, tiles, 0, Side::sideOpponent);
        initializeTilesLayer(_window, _mouse, tilesFlank, tiles, 1, Side::sideOpponent);
        initializeTilesLayer(_window, _mouse, tilesRear, tiles, 2, Side::sideOpponent);
    }
}

void TilesManager::setStatus(const TilesManagerStatus &_status)
{
    this->status = _status;
    switch (status)
    {
    case TilesManagerStatus::statusReleasingCard:
    {
        vector<bool> activeTiles;
        for (auto tile : tiles)
        {
            if (tile->getStatus() == TileStatus::statusIsEmpty)
            {
                tile->setFillColor(colorForReleasingUnit);
                activeTiles.push_back(true);
            }
            else
            {
                tile->setFillColor(colorWhenCantReleaseUnit);
                activeTiles.push_back(false);
            }
        }
        this->setActiveTiles(activeTiles);
    }
    default:
    {
        break;
    }
    }
}

void TilesManager::setTileBuffer(Tile *_tile)
{
    tileBuffer = _tile;
}

Tile *TilesManager::getTileBuffer()
{
    return tileBuffer;
}

void TilesManager::enable()
{
    for (auto tile : tiles)
    {
        tile->setFillColor(colorBasic);
        tile->enable();
    }
    this->updateFocus();
}

void TilesManager::disable()
{
    for (auto tile : tiles)
    {
        tile->setFillColor(colorDisabled);
        tile->disable();
    }
    this->updateFocus();
}

bool TilesManager::getPressed()
{
    return isPressed;
}

void TilesManager::setActiveTiles(const vector<bool> &_activeTiles)
{
    for (int i = 0; i < 3; ++i)
    {
        activeTiles[0][i] = _activeTiles[0 + i];
    }
    for (int i = 0; i < 3; ++i)
    {
        activeTiles[1][i] = _activeTiles[3 + i];
    }
    for (int i = 0; i < 3; ++i)
    {
        activeTiles[2][i] = _activeTiles[6 + i];
    }
}

void TilesManager::resetActiveTiles()
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            activeTiles[i][j] = 0;
        }
    }
}

bool TilesManager::hasBodies()
{
    for (auto tile : tiles)
    {
        if (tile->getStatus() == TileStatus::statusHasDeadBody)
        {
            return true;
        }
    }
    return false;
}

void TilesManager::setUnitBuffer(Unit &_unit)
{
    this->unitBuffer = &_unit;
}

auto pressTileOnStage{
    [](vector<unique_ptr<Tile>> *_tiles, TilesManager &_tileManager) {
        for (size_t i = 0; i < 3; ++i)
        {
            if ((*_tiles)[i]->hasFocus() && (*_tiles)[i]->getStatus() == TileStatus::statusHasUnit)
            {
                BOOST_LOG_TRIVIAL(info) << "TilesManager::mouseIsPressed(): tile was clicked!";
                _tileManager.setTileBuffer((*_tiles)[i].get());
                return true;
            }
        }
        return false;
    }};

auto focusTileOnStage{
    [](vector<unique_ptr<Tile>> &_tiles, const Color &_colorWhenCanAttack, const Color &_colorInFocus) {
        for (size_t i = 0; i < 3; ++i)
        {
            if ((_tiles)[i]->getStatus() == TileStatus::statusHasUnit)
            {
                (_tiles)[i]->setFillColor(_colorWhenCanAttack);
                if ((_tiles)[i]->hasFocus())
                {
                    (_tiles)[i]->setFillColor(_colorInFocus);
                }
            }
        }
        return;
    }};

bool TilesManager::mouseIsPressed()
{
    isPressed = false;
    switch (status)
    {
    case TilesManagerStatus::statusNothingHappens:
    {
        return false;
    }
    break;

    case TilesManagerStatus::statusGameStarting:
    {
        return false;
    }
    break;

    case TilesManagerStatus::statusGameStartingReleasingCard:
    {
        if (unitBuffer == nullptr)
        {
            BOOST_LOG_TRIVIAL(error) << "TilesManager::mouseIsPressed():: ERROR! statusReleasingCard : unitBuffer is nullptr!";
            return false;
        }
        for (auto tile : tiles)
        {
            tile->setFillColor(colorDisabled);
        }
        if (tilesFlank[1]->hasFocus())
        {
            BOOST_LOG_TRIVIAL(info) << "TilesManager::mouseIsPressed()::GameStarts releasing first card!";
            //Так как у лидера хп на 10 больше
            unitBuffer->setTextHP(unitBuffer->getHealth() + 10);
            tilesFlank[1]->setUnit(*unitBuffer);
            this->setNormalColors();
            BOOST_LOG_TRIVIAL(info) << "TilesManager::mouseIsPressed()::GameStarts ID = " << unitBuffer->getId();
            client.sendCardReleased(unitBuffer->getId(), tilesFlank[1]->getCoordX(), tilesFlank[1]->getCoordY());
            unitBuffer = nullptr;
            return true;
        }
        break;
    }

    case TilesManagerStatus::statusWaitingForAttack:
        for (auto tile : tiles)
        {
            if (tile->hasFocus() && activeTiles[tile->getCoordX()][tile->getCoordY()] == true)
            {
                BOOST_LOG_TRIVIAL(info) << "TilesManager::mouseIsPressed(): Tile was attacked, sending attacked pos!";
                BOOST_LOG_TRIVIAL(info) << "TilesManager::mouseIsPressed(): Attacker: " << this->tileBuffer->getCoordX() << " " << this->tileBuffer->getCoordY();
                BOOST_LOG_TRIVIAL(info) << "TilesManager::mouseIsPressed(): Attacked: " << tile->getCoordX() << " " << tile->getCoordY();

                client.sendAttackedPos(tile->getCoordX(), tile->getCoordY());

                isPressed = true;
                return true;
            }
        }
        break;

    case TilesManagerStatus::statusReleasingCard:
    {
        if (unitBuffer == nullptr)
        {
            BOOST_LOG_TRIVIAL(error) << "TilesManager::mouseIsPressed()::statusReleasingCard : ERROR! unitBuffer is nullptr!";
            return false;
        }
        for (auto tile : tiles)
        {
            if (tile->hasFocus() && tile->getStatus() == TileStatus::statusIsEmpty)
            {
                BOOST_LOG_TRIVIAL(info) << "TilesManager::mouseIsPressed()::statusReleasingCard : releasing card!";

                tile->setUnit(*unitBuffer);
                this->setNormalColors();

                BOOST_LOG_TRIVIAL(info) << "TilesManager::mouseIsPressed()::ReleasingCard ID = " << unitBuffer->getId();
                client.sendCardReleased(unitBuffer->getId(), tile->getCoordX(), tile->getCoordY());

                unitBuffer = nullptr;
                return true;
            }
        }
    }
    break;

    case TilesManagerStatus::statusAttackingUnit:
    {
        switch (round)
        {
        case RoundType::roundAvangard:
        {
            isPressed = pressTileOnStage(&tilesAvangard, *this);
            return false;
        }
        break;

        case RoundType::roundFlank:
        {
            isPressed = pressTileOnStage(&tilesFlank, *this);
            return false;
        }
        break;

        case RoundType::roundRear:
        {
            isPressed = pressTileOnStage(&tilesRear, *this);
            return false;
        }
        break;

        default:
            break;
        }
    }

    case TilesManagerStatus::statusWaitingForPower:
    {
        for (auto tile : tiles)
        {
            if (tile->hasFocus() && activeTiles[tile->getCoordX()][tile->getCoordY()] == true)
            {
                BOOST_LOG_TRIVIAL(info) << "TilesManager::mouseIsPressed(): Used power on tile, sending coordinates!";
                BOOST_LOG_TRIVIAL(info) << "TilesManager::mouseIsPressed(): Power target: " << tile->getCoordX() << " " << tile->getCoordY();

                client.sendPowerTargetPos(tile->getCoordX(), tile->getCoordY());

                this->setStatus(TilesManagerStatus::statusPowerWasUsed);

                isPressed = true;
                return false;
            }
        }
        break;
    }

    case TilesManagerStatus::statusWhenThePowerWhichChangesStatsImmidiatelyWasActivated:
    {
        return true;
        break;
    }

    case TilesManagerStatus::statusWaitingForRemovingBody:
    {
        for (auto tile : tiles)
        {
            if (tile->hasFocus() && activeTiles[tile->getCoordX()][tile->getCoordY()] == true)
            {
                BOOST_LOG_TRIVIAL(info) << "TilesManager::mouseIsPressed(): Removing Body!";
                BOOST_LOG_TRIVIAL(info) << "TilesManager::mouseIsPressed(): Body: " << tile->getCoordX() << " " << tile->getCoordY();

                client.sendRemovedBody(tile->getCoordX(), tile->getCoordY());
                //Эту инфа будет отлавливаться в PlayersTurn
                this->setStatus(TilesManagerStatus::statusBodyRemoved);

                isPressed = true;
                return false;
            }
        }
        break;
    }

    default:
        //return false;
        break;
    }
    return false;
}

void TilesManager::setNormalColors()
{
    for (auto tile : tiles)
    {
        tile->setFillColor(colorBasic);
    }
}

auto updateFocusOnLine{
    [](vector<unique_ptr<Tile>> &_tiles, const bool _activeTiles[3][3], const Color &_colorInFocus, const Color &_colorWhenActive, const Color &_colorWhenNotActive, int posX) {
        for (int j = 0; j < 3; ++j)
        {
            if (_activeTiles[posX][j] == true)
            {
                if (_tiles[j]->hasFocus())
                {
                    _tiles[j]->setFillColor(_colorInFocus);
                }
                else
                {
                    _tiles[j]->setFillColor(_colorWhenActive);
                }
            }
            else
            {
                _tiles[j]->setFillColor(_colorWhenNotActive);
            }
        }
        return;
    }};

void TilesManager::updateFocus()
{
    switch (status)
    {
    case TilesManagerStatus::statusNothingHappens:
    {
        for (auto tile : tiles)
        {
            if (tile->hasFocus())
            {
                tile->setFillColor(colorInFocus);
            }
            else if (tile->getStatus() == TileStatus::statusHasDeadBody)
            {
                tile->setFillColor(colorForDeadBody);
            }
            else
            {
                tile->setFillColor(colorBasic);
            }
        }
        break;
    }

    case TilesManagerStatus::statusGameStarting:
    {
        updateFocusOnLine(tilesAvangard, activeTiles, colorInFocus, colorBasic, colorBasic, 0);
        updateFocusOnLine(tilesFlank, activeTiles, colorInFocus, colorBasic, colorBasic, 1);
        updateFocusOnLine(tilesRear, activeTiles, colorInFocus, colorBasic, colorBasic, 2);
        break;
    }

    case TilesManagerStatus::statusGameStartingReleasingCard:
    {
        updateFocusOnLine(tilesAvangard, activeTiles, colorInFocus, colorBasic, colorDisabled, 0);
        updateFocusOnLine(tilesFlank, activeTiles, colorInFocus, colorBasic, colorDisabled, 1);
        updateFocusOnLine(tilesRear, activeTiles, colorInFocus, colorBasic, colorDisabled, 2);
        break;
    }

    case TilesManagerStatus::statusWaitingForAttack:
    {
        updateFocusOnLine(tilesAvangard, activeTiles, colorInFocus, colorBasic, colorDisabled, 0);
        updateFocusOnLine(tilesFlank, activeTiles, colorInFocus, colorBasic, colorDisabled, 1);
        updateFocusOnLine(tilesRear, activeTiles, colorInFocus, colorBasic, colorDisabled, 2);

        break;
    }

    //Поменять цвета
    case TilesManagerStatus::statusWaitingForPower:
    {
        updateFocusOnLine(tilesAvangard, activeTiles, colorInFocus, colorBasic, colorDisabled, 0);
        updateFocusOnLine(tilesFlank, activeTiles, colorInFocus, colorBasic, colorDisabled, 1);
        updateFocusOnLine(tilesRear, activeTiles, colorInFocus, colorBasic, colorDisabled, 2);

        break;
    }

    case TilesManagerStatus::statusWaitingForRemovingBody:
    {
        updateFocusOnLine(tilesAvangard, activeTiles, colorInFocus, Color::Blue, colorDisabled, 0);
        updateFocusOnLine(tilesFlank, activeTiles, colorInFocus, Color::Blue, colorDisabled, 1);
        updateFocusOnLine(tilesRear, activeTiles, colorInFocus, Color::Blue, colorDisabled, 2);

        break;
    }

    case TilesManagerStatus::statusReleasingCard:
    {
        for (auto tile : tiles)
        {
            if (tile->hasFocus() && tile->getStatus() == TileStatus::statusIsEmpty)
            {
                tile->setFillColor(colorInFocus);
            }
            else
            {
                if (tile->getStatus() == TileStatus::statusIsEmpty)
                {
                    tile->setFillColor(colorForReleasingUnit);
                }
            }
        }
        break;
    }

    case TilesManagerStatus::statusAttackingUnit:
    {
        for (auto tile : tiles)
        {
            tile->setFillColor(colorWhenCantAttack);
        }
        switch (round)
        {
        case RoundType::roundAvangard:
            focusTileOnStage(tilesAvangard, colorWhenCanAttack, colorInFocus);
            break;
        case RoundType::roundFlank:
            focusTileOnStage(tilesFlank, colorWhenCanAttack, colorInFocus);
            break;
        case RoundType::roundRear:
            focusTileOnStage(tilesRear, colorWhenCanAttack, colorInFocus);
            break;
        default:
            break;
        }
        break;
    }

    //Добавить другие случаи
    default:
        break;
    }
}

void TilesManager::setRound(RoundType &_round)
{
    round = _round;
}

void TilesManager::draw()
{
    for (auto tile : tiles)
    {
        tile->draw();
    }
}

TilesManagerStatus TilesManager::getStatus()
{
    return status;
}

void TilesManager::setTexture(Texture *_texture)
{
    for (auto tile : tiles)
    {
        tile->setTexture(_texture);
    }
}

bool TilesManager::hasEmptyTiles()
{
    for (auto tile : tiles)
    {
        if (tile->getStatus() == TileStatus::statusIsEmpty)
        {
            return 1;
        }
    }
    return 0;
}

void TilesManager::deleteUnit(int x, int y)
{
    switch (x)
    {
    case (0):
    {
        tilesAvangard[y]->deleteUnit();
        break;
    }
    case (1):
    {
        tilesFlank[y]->deleteUnit();
        break;
    }
    case (2):
    {
        tilesRear[y]->deleteUnit();
        break;
    }
    default:
        break;
    }
}

void TilesManager::setUnit(Unit &unit, int coordX, int coordY)
{
    switch (coordX)
    {
    case 0:
        tilesAvangard[coordY]->setUnit(unit);
        break;
    case 1:
        tilesFlank[coordY]->setUnit(unit);
        break;
    case 2:
        tilesRear[coordY]->setUnit(unit);
        break;
    default:
        break;
    }
}

TilesManager::~TilesManager()
{
    unitBuffer = nullptr;
}