#include "Tile.h"
#include "configurations.cpp"

#include <iostream>
using namespace std;

Tile::Tile(RenderWindow &_window, Mouse &_mouse) : Clickable(_window, _mouse)
{
    unit = nullptr;
}

//Tile::Tile(RenderWindow &_window) : DrawableBox(_window) {};

void Tile::setUnit(Unit *_unit)
{
    this->unit = _unit;
    this->status = TileStatus::statusHasUnit;
    unit->setPosition(this->rect.getPosition().x, this->rect.getPosition().y);
    ////////////////////////////////////////////////////////////////////////////////////////COUT
    cout << "Tile::setUnit === " << unit << " ===" << endl;
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
        for (size_t i = 0; i < 9; ++i)
        {
            if (tiles[i]->getStatus() == TileStatus::statusIsEmpty)
            {
                tiles[i]->setFillColor(colorForReleasingUnit);
            }
            else
            {
                tiles[i]->setFillColor(colorWhenCantReleaseUnit);
            }
        }
    }
    default:
    {
        break;
    }
    }
}

void TilesManager::setUnitBuffer(Unit *_unit)
{
    this->unitBuffer = _unit;
}

void TilesManager::mouseIsPressed()
{
    switch (status)
    {
    case TilesManagerStatus::statusNothingHappens:
    {
        for (int i = 0; i < numberOfTiles; ++i)
        {
            if (tiles[i]->hasFocus())
            {
                cout << "TilesManager::mouseIsPressed() tile was clicked!" << endl;
            }
        }
        return;
        break;
    }
    case TilesManagerStatus::statusReleasingUnit:
    {
        if (unitBuffer == nullptr)
        {
            ////////////////////////////////////////////////////////////////////////////////////////COUT
            cout << "ERROR, TilesManager::mouseIsPressed:: unitBuffer is nullptr" << endl;

            return;
        }
        for (int i = 0; i < numberOfTiles; ++i)
        {
            if (tiles[i]->hasFocus() && tiles[i]->getStatus() == TileStatus::statusIsEmpty)
            {
                ////////////////////////////////////////////////////////////////////////////////////////COUT
                cout << "TilesManager::mouseIsPressed:: RELEASING UNIT!!!" << endl;

                tiles[i]->setUnit(unitBuffer);
                this->setNormalColors();
                this->status = TilesManagerStatus::statusCardWasJustReleased;
                unitBuffer = nullptr;
                return;
            }
        }
    }
    }
}

void TilesManager::setNormalColors()
{
    for (int i = 0; i < numberOfTiles; ++i)
    {
        tiles[i]->setFillColor(colorBasic);
    }
}

void TilesManager::updateFocus()
{
    switch (status)
    {
    case TilesManagerStatus::statusNothingHappens:
        for (int i = 0; i < numberOfTiles; ++i)
        {
            if (tiles[i]->hasFocus())
            {
                tiles[i]->setFillColor(colorInFocus);
            }
            else
            {
                tiles[i]->setFillColor(colorBasic);
            }
        }
        break;
    case TilesManagerStatus::statusReleasingUnit:
        for (int i = 0; i < numberOfTiles; ++i)
        {
            if (tiles[i]->hasFocus() && tiles[i]->getStatus() == TileStatus::statusIsEmpty)
            {
                tiles[i]->setFillColor(colorInFocus);
            }
            else
            {
                if (tiles[i]->getStatus() == TileStatus::statusIsEmpty)
                {
                    tiles[i]->setFillColor(colorForReleasingUnit);
                }
            }
        }
        break;

    //Добавить другие случаи
    default:
        break;
    }
}

void TilesManager::draw()
{
    for (int i = 0; i < numberOfTiles; ++i)
    {
        tiles[i]->draw();
    }
}

TilesManagerStatus TilesManager::getStatus()
{
    return status;
}

void TilesManager::setTexture(Texture *_texture)
{
    for (int i = 0; i < numberOfTiles; ++i)
    {
        tiles[i]->setTexture(_texture);
    }
}

Unit *TilesManager::getUnitBuffer()
{
    return unitBuffer;
}

bool TilesManager::hasEmptyTiles()
{
    for (int i = 0; i < numberOfTiles; ++i)
    {
        if (tiles[i]->getStatus() == TileStatus::statusIsEmpty)
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