#include "Tile.h"
#include "configurations.cpp"

#include <iostream>
using namespace std;

Tile::Tile(RenderWindow &_window, Mouse &_mouse) : Clickable(_window, _mouse)
{
    unit = nullptr;
}

//Tile::Tile(RenderWindow &_window) : DrawableBox(_window) {};

void Tile::setUnit(Unit &_unit)
{
    this->unit = make_unique<Unit>(_unit);
    this->status = TileStatus::statusHasUnit;
    unit->setPosition(this->rect.getPosition().x, this->rect.getPosition().y);
    cout << "Tile::setUnit() : " << unit.get() << " ===" << endl;
}

void Tile::setStatus(TileStatus _status)
{
    this->status = _status;
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
        unit->draw();
    }
}

Tile::~Tile()
{
    unit = nullptr;
}

//===========================================
//===============TilesManager================
//===========================================

TilesManager::TilesManager(RenderWindow &_window, Mouse &_mouse, const Side &_side, GameTcpClient& _client) : side(_side), client(_client)
{
    status = TilesManagerStatus::statusNothingHappens;
    round = RoundType::roundAvangard;
    unitBuffer = nullptr;

    if (side == Side::sidePlayer)
    {
        //Тайлы авангарда
        for (int i = 0; i < 3; ++i)
        {
            tilesAvangard.push_back(make_unique<Tile>(_window, _mouse));
            tilesAvangard[i]->setSize(tileWidth, tileHeight);
            tilesAvangard[i]->setPosition(150 * 3 - 80, 100 * i + 200);
            tilesAvangard[i]->setStatus(TileStatus::statusIsEmpty);
            tilesAvangard[i]->setCoordinates(0, i);
            tiles.push_back(tilesAvangard[i].get());
        }
        //Тайлы фланга
        for (int i = 0; i < 3; ++i)
        {
            tilesFlank.push_back(make_unique<Tile>(_window, _mouse));
            tilesFlank[i]->setSize(tileWidth, tileHeight);
            tilesFlank[i]->setPosition(150 * 2 - 80, 100 * i + 200);
            tilesFlank[i]->setStatus(TileStatus::statusIsEmpty);
            tilesFlank[i]->setCoordinates(1, i);
            tiles.push_back(tilesFlank[i].get());
        }
        //Тайлы тыла
        for (int i = 0; i < 3; ++i)
        {
            tilesRear.push_back(make_unique<Tile>(_window, _mouse));
            tilesRear[i]->setSize(tileWidth, tileHeight);
            tilesRear[i]->setPosition(150 * 1 - 80, 100 * i + 200);
            tilesRear[i]->setStatus(TileStatus::statusIsEmpty);
            tilesRear[i]->setCoordinates(2, i);
            tiles.push_back(tilesRear[i].get());
        }
    }
    else if (side == Side::sideOpponent)
    {
        //Тайлы авангарда
        for (int i = 0; i < 3; ++i)
        {
            tilesAvangard.push_back(make_unique<Tile>(_window, _mouse));
            tilesAvangard[i]->setSize(tileWidth, tileHeight);
            tilesAvangard[i]->setPosition(150 * 1 + windowWidth - 520 - tileWidth, 100 * i + 200);
            tilesAvangard[i]->setStatus(TileStatus::statusIsEmpty);
            //Здесь, по логике, должно бытть (0, i), но, почему-то, это не работает
            tilesAvangard[i]->setCoordinates(2, i);
            tiles.push_back(tilesAvangard[i].get());
        }
        //Тайлы фланга
        for (int i = 0; i < 3; ++i)
        {
            tilesFlank.push_back(make_unique<Tile>(_window, _mouse));
            tilesFlank[i]->setSize(tileWidth, tileHeight);
            tilesFlank[i]->setPosition(150 * 2 + windowWidth - 520 - tileWidth, 100 * i + 200);
            tilesFlank[i]->setStatus(TileStatus::statusIsEmpty);
            tilesAvangard[i]->setCoordinates(1, i);
            tiles.push_back(tilesFlank[i].get());
        }
        //Тайлы тыла
        for (int i = 0; i < 3; ++i)
        {
            tilesRear.push_back(make_unique<Tile>(_window, _mouse));
            tilesRear[i]->setSize(tileWidth, tileHeight);
            tilesRear[i]->setPosition(150 * 3 + windowWidth - 520 - tileWidth, 100 * i + 200);
            tilesRear[i]->setStatus(TileStatus::statusIsEmpty);
            tilesAvangard[i]->setCoordinates(0, i);
            tiles.push_back(tilesRear[i].get());
        }

        //УДАЛИТЬ, ЗАГЛУШКА!!!
        for (auto tile : tiles)
        {
            tile->setStatus(TileStatus::statusHasUnit);
        }
    }
}

void TilesManager::setStatus(const TilesManagerStatus &_status)
{
    this->status = _status;
    switch (status)
    {
    case TilesManagerStatus::statusReleasingCard:
    {
        for (auto tile : tiles)
        {
            if (tile->getStatus() == TileStatus::statusIsEmpty)
            {
                tile->setFillColor(colorForReleasingUnit);
            }
            else
            {
                tile->setFillColor(colorWhenCantReleaseUnit);
            }
        }
    }
    default:
    {
        break;
    }
    }
}

void TilesManager::setUnitBuffer(Unit &_unit)
{
    this->unitBuffer = make_unique<Unit>(_unit);
}

auto pressTileOnStage{
    [](vector<unique_ptr<Tile>> *_tiles, TilesManager &_tileManager) {
        for (size_t i = 0; i < 3; ++i)
        {
            if ((*_tiles)[i]->hasFocus() && (*_tiles)[i]->getStatus() == TileStatus::statusHasUnit)
            {
                cout << "TilesManager::mouseIsPressed(): tile was clicked!" << endl;
                _tileManager.setTileBuffer((*_tiles)[i].get());
                return true;
            }
        }
        return false;
    }};

auto focusTileOnStage{
    [](vector<unique_ptr<Tile>> *_tiles, const Color &_colorWhenCanAttack, const Color &_colorInFocus) {
        for (size_t i = 0; i < 3; ++i)
        {
            if ((*_tiles)[i]->getStatus() == TileStatus::statusHasUnit)
            {
                (*_tiles)[i]->setFillColor(_colorWhenCanAttack);
                if ((*_tiles)[i]->hasFocus())
                {
                    (*_tiles)[i]->setFillColor(_colorInFocus);
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
    case TilesManagerStatus::statusGameStartingReleasingCard:
    {
        if (unitBuffer == nullptr)
        {
            cout << "TilesManager::mouseIsPressed:: ERROR! statusReleasingCard : unitBuffer is nullptr" << endl;
            return false;
        }
        for (auto tile : tiles)
        {
            tile->setFillColor(colorDisabled);
        }
        if (tilesFlank[1]->hasFocus())
        {
            cout << "TilesManager::mouseIsPressed::GameStarts RELEASING CARD!!!" << endl;

            tilesFlank[1]->setUnit(*unitBuffer);
            this->setNormalColors();
            //Эту информацию сразу отлавливает CardsManager
            client.sendCardReleased(19, unitBuffer->getPosX(), unitBuffer->getPosY());
            this->status = TilesManagerStatus::statusCardWasJustReleased;
            unitBuffer = nullptr;
            return true;
        }
        break;
    }
    case TilesManagerStatus::statusWaitingForAttack:
        for (auto tile : tiles)
        {
            if (tile->hasFocus() && tile->getStatus() == TileStatus::statusHasUnit)
            {
                //Потом надо заменить на отдачу информации серверу

                //Посылаем клиенту сообщения с координатами.
                //client.send(Action::AttackWasDone, this->tileBuffer->getCoordX(), this->tileBuffer->getCoordY(), tile->getCoordX(), tile->getCoordY());
                //В функции clientUpdate будет отлавливаться сообщение с обновленным массивом.
                //clientUpdate();

                cout << "Attacker: " << this->tileBuffer->getCoordX() << " " << this->tileBuffer->getCoordY() << endl;
                cout << "Attacked: " << tile->getCoordX() << " " << tile->getCoordY() << endl;

                tile->setStatus(TileStatus::statusHasDeadBody);
                //this->setTileBuffer(tile);

                cout << "TilesManager::mouseIsPressed(): Tile was attacked!" << endl;
                this->setStatus(TilesManagerStatus::statusNothingHappens);
                this->updateFocus();
                isPressed = true;
                return true;
            }
        }
        break;
    case TilesManagerStatus::statusReleasingCard:
    {
        if (unitBuffer == nullptr)
        {
            ////////////////////////////////////////////////////////////////////////////////////////COUT
            cout << "TilesManager::mouseIsPressed:: ERROR! statusReleasingCard : unitBuffer is nullptr" << endl;
            return false;
        }
        for (auto tile : tiles)
        {
            if (tile->hasFocus() && tile->getStatus() == TileStatus::statusIsEmpty)
            {
                ////////////////////////////////////////////////////////////////////////////////////////COUT
                cout << "TilesManager::mouseIsPressed::statusReleasingCard : RELEASING CARD!!!" << endl;

                tile->setUnit(*unitBuffer);
                this->setNormalColors();

                //Отсылаем инфу серверу!!!
                //client.sendMessage(ReleasedCard, unitBuffer.getId(), tile->getCoordX(), tile->getCoordX());

                //Эту информацию сразу отлавливает CardsManager
                this->status = TilesManagerStatus::statusCardWasJustReleased;
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
            //return pressTileOnStage(&tilesAvangard, *this);
            isPressed = pressTileOnStage(&tilesAvangard, *this);
            return false;
        }
        break;
        case RoundType::roundFlank:
        {
            //return pressTileOnStage(&tilesFlank, *this);
            isPressed = pressTileOnStage(&tilesFlank, *this);
            return false;
        }
        break;
        case RoundType::roundRear:
        {
            //return pressTileOnStage(&tilesRear, *this);
            isPressed = pressTileOnStage(&tilesRear, *this);
            return false;
        }
        break;
        }
    }
    break;
    }
}

void TilesManager::setNormalColors()
{
    for (auto tile : tiles)
    {
        tile->setFillColor(colorBasic);
    }
}

#define FOCUS 0

#if FOCUS == 0

//==================
//Старый updateFocus
//==================
void TilesManager::updateFocus()
{
    switch (status)
    {
    case TilesManagerStatus::statusNothingHappens:
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
    case TilesManagerStatus::statusGameStarting:
        for (auto tile : tiles)
        {
            if (tile->hasFocus())
            {
                tile->setFillColor(colorInFocus);
            }
            else
            {
                tile->setFillColor(colorBasic);
            }
        }
        break;
    case TilesManagerStatus::statusGameStartingReleasingCard:
        for (auto tile : tiles)
        {
            tile->setFillColor(colorDisabled);
        }
        if (tilesFlank[1]->hasFocus())
        {
            tilesFlank[1]->setFillColor(colorInFocus);
        }
        else
        {
            tilesFlank[1]->setFillColor(colorBasic);
        }
        break;
    case TilesManagerStatus::statusWaitingForAttack:
        for (auto tile : tiles)
        {
            if (tile->hasFocus() && tile->getStatus() == TileStatus::statusHasUnit)
            {
                tile->setFillColor(colorInFocus);
            }
            else if (tile->getStatus() == TileStatus::statusHasDeadBody)
            {
                tile->setFillColor(colorForDeadBody);
            }
            else
            {
                tile->setFillColor(colorWaitingForAttack);
            }
        }
        break;
    case TilesManagerStatus::statusReleasingCard:
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
    case TilesManagerStatus::statusAttackingUnit:
        for (auto tile : tiles)
        {
            tile->setFillColor(colorWhenCantAttack);
        }
        switch (round)
        {
        case RoundType::roundAvangard:
            focusTileOnStage(&tilesAvangard, colorWhenCanAttack, colorInFocus);
            break;
        case RoundType::roundFlank:
            focusTileOnStage(&tilesFlank, colorWhenCanAttack, colorInFocus);
            break;
        case RoundType::roundRear:
            focusTileOnStage(&tilesRear, colorWhenCanAttack, colorInFocus);
            break;
        default:
            break;
        }
        break;

    //Добавить другие случаи
    default:
        break;
    }
}

#endif

#if FOCUS == 1

void TilesManager::updateFocus()
{
    switch (status)
    {
    case TilesManagerStatus::statusNothingHappens:
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
    case TilesManagerStatus::statusGameStarting:
        for (auto tile : tiles)
        {
            if (tile->hasFocus())
            {
                tile->setFillColor(colorInFocus);
            }
            else
            {
                tile->setFillColor(colorBasic);
            }
        }
        break;
    case TilesManagerStatus::statusGameStartingReleasingCard:
        for (auto tile : tiles)
        {
            tile->setFillColor(colorDisabled);
        }
        if (tilesFlank[1]->hasFocus())
        {
            tilesFlank[1]->setFillColor(colorInFocus);
        }
        else
        {
            tilesFlank[1]->setFillColor(colorBasic);
        }
        break;
    case TilesManagerStatus::statusWaitingForAttack:
        for (auto tile : tiles)
        {
            if (activeTiles[tile->getCoordX()][tile->getCoordY()])
            {
                if (tile->hasFocus())
                {
                    tile->setFillColor(colorInFocus);
                }
                else
                {
                    tile->setFillColor(colorWaitingForAttack);
                }
            }
            else
            {
                tile->setFillColor(colorDisabled);
            }
        }
        break;
    case TilesManagerStatus::statusReleasingCard:
        for (auto tile : tiles)
        {
            if (activeTiles[tile->getCoordX()][tile->getCoordY()])
            {
                if (tile->hasFocus())
                {
                    tile->setFillColor(colorInFocus);
                }
                else
                {
                    tile->setFillColor(colorForReleasingUnit);
                }
            }
            else
            {
                tile->setFillColor(colorDisabled);
            }
        }
        break;
    case TilesManagerStatus::statusAttackingUnit:
        for (auto tile : tiles)
        {
            tile->setFillColor(colorDisabled);
        }
        switch (round)
        {
        case RoundType::roundAvangard:
            focusTileOnStage(&tilesAvangard, colorWhenCanAttack, colorInFocus);
            break;
        case RoundType::roundFlank:
            focusTileOnStage(&tilesFlank, colorWhenCanAttack, colorInFocus);
            break;
        case RoundType::roundRear:
            focusTileOnStage(&tilesRear, colorWhenCanAttack, colorInFocus);
            break;
        default:
            break;
        }
        break;

    //Добавить другие случаи
    default:
        break;
    }
}

#endif // FOCUS == 1

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

Unit *TilesManager::getUnitBuffer()
{
    return unitBuffer.get();
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

TilesManager::~TilesManager()
{
    unitBuffer = nullptr;
}