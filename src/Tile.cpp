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
    ////////////////////////////////////////////////////////////////////////////////////////COUT
    cout << "Tile::setUnit === " << unit.get() << " ===" << endl;
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
TilesManager::TilesManager(RenderWindow &_window, Mouse &_mouse, const Side &_side) : side(_side)
{
    status = TilesManagerStatus::statusNothingHappens;
    stage = Stage::stageAvangard;
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
            tiles.push_back(tilesAvangard[i].get());
        }
        //Тайлы фланга
        for (int i = 0; i < 3; ++i)
        {
            tilesFlank.push_back(make_unique<Tile>(_window, _mouse));
            tilesFlank[i]->setSize(tileWidth, tileHeight);
            tilesFlank[i]->setPosition(150 * 2 - 80, 100 * i + 200);
            tilesFlank[i]->setStatus(TileStatus::statusIsEmpty);
            tiles.push_back(tilesFlank[i].get());
        }
        //Тайлы тыла
        for (int i = 0; i < 3; ++i)
        {
            tilesRear.push_back(make_unique<Tile>(_window, _mouse));
            tilesRear[i]->setSize(tileWidth, tileHeight);
            tilesRear[i]->setPosition(150 * 1 - 80, 100 * i + 200);
            tilesRear[i]->setStatus(TileStatus::statusIsEmpty);
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
            tiles.push_back(tilesAvangard[i].get());
        }
        //Тайлы фланга
        for (int i = 0; i < 3; ++i)
        {
            tilesFlank.push_back(make_unique<Tile>(_window, _mouse));
            tilesFlank[i]->setSize(tileWidth, tileHeight);
            tilesFlank[i]->setPosition(150 * 2 + windowWidth - 520 - tileWidth, 100 * i + 200);
            tilesFlank[i]->setStatus(TileStatus::statusIsEmpty);
            tiles.push_back(tilesFlank[i].get());
        }
        //Тайлы тыла
        for (int i = 0; i < 3; ++i)
        {
            tilesRear.push_back(make_unique<Tile>(_window, _mouse));
            tilesRear[i]->setSize(tileWidth, tileHeight);
            tilesRear[i]->setPosition(150 * 3 + windowWidth - 520 - tileWidth, 100 * i + 200);
            tilesRear[i]->setStatus(TileStatus::statusIsEmpty);
            tiles.push_back(tilesRear[i].get());
        }
    }
}

void TilesManager::setStatus(const TilesManagerStatus &_status)
{
    this->status = _status;
    switch (status)
    {
    case TilesManagerStatus::statusReleasingUnit:
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
            }
        }
        return;
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

void TilesManager::mouseIsPressed()
{
    switch (status)
    {
    case TilesManagerStatus::statusNothingHappens:
    {
    }
    break;
    case TilesManagerStatus::statusWaitingForAttack:
        for (auto tile : tiles)
        {
            if (tile->hasFocus()) //&& tile->getStatus() == TileStatus::statusHasUnit)
            {
                //Потом надо заменить на отдачу информации серверу
                tile->setStatus(TileStatus::statusHasDeadBody);

                cout << "TilesManager::mouseIsPressed(): Tile was attacked!" << endl;
                this->setStatus(TilesManagerStatus::statusNothingHappens);
                break;
            }
        }
    case TilesManagerStatus::statusReleasingUnit:
    {
        if (unitBuffer == nullptr)
        {
            ////////////////////////////////////////////////////////////////////////////////////////COUT
            cout << "ERROR, TilesManager::mouseIsPressed:: unitBuffer is nullptr" << endl;

            return;
        }
        for (auto tile : tiles)
        {
            if (tile->hasFocus() && tile->getStatus() == TileStatus::statusIsEmpty)
            {
                ////////////////////////////////////////////////////////////////////////////////////////COUT
                cout << "TilesManager::mouseIsPressed:: RELEASING UNIT!!!" << endl;

                tile->setUnit(*unitBuffer);
                this->setNormalColors();
                this->status = TilesManagerStatus::statusCardWasJustReleased;
                unitBuffer = nullptr;
                return;
            }
        }
    }
    break;
    case TilesManagerStatus::statusAttackingUnit:
    {
        switch (stage)
        {
        case Stage::stageAvangard:
        {
            pressTileOnStage(&tilesAvangard, *this);
        }
        break;
        case Stage::stageFlank:
        {
            pressTileOnStage(&tilesFlank, *this);
        }
        break;
        case Stage::stageRear:
        {
            pressTileOnStage(&tilesRear, *this);
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
            else
            if(tile->getStatus()==TileStatus::statusHasDeadBody)
            {
                tile->setFillColor(colorForDeadBody);
            }
            else
            {
                tile->setFillColor(colorBasic);
            }
        }
        break;
    case TilesManagerStatus::statusWaitingForAttack:
        for (auto tile : tiles)
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
        break;
    case TilesManagerStatus::statusReleasingUnit:
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
        switch (stage)
        {
        case Stage::stageAvangard:
            focusTileOnStage(&tilesAvangard, colorWhenCanAttack, colorInFocus);
            break;
        case Stage::stageFlank:
            focusTileOnStage(&tilesFlank, colorWhenCanAttack, colorInFocus);
            break;
        case Stage::stageRear:
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

void TilesManager::setStage(Stage _stage)
{
    stage = _stage;
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