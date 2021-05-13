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

Tile::~Tile() {}

//===========================================
//===============TilesManager================
//===========================================
TilesManager::TilesManager(RenderWindow &_window, Mouse &_mouse, const Side &_side) : side(_side)
{
    status = TilesManagerStatus::statusNothingHappens;
    unitBuffer = nullptr;

    if (side == Side::sidePlayer)
    {
        for (int i = 0; i < numberOfTiles; ++i)
        {
            tiles.push_back(new Tile(_window, _mouse));
            tiles[i]->setSize(tileWidth, tileHeight);
            tiles[i]->setPosition(150 * (i % 3 + 1) - 80, 100 * (i / 3 + 1) + 100);
            tiles[i]->setStatus(TileStatus::statusIsEmpty);

            //Случайным образом выбрал тайлы, с которыми будет работать команда, вычисляющая, на каких полях можно ставить юнитов
            if (i % 3 + 1 == i / 3 + 1)
            {
                tiles[i]->setStatus(TileStatus::statusHasUnit);
            }
        }
    }
    else if (side == Side::sideOpponent)
    {
        for (int i = 0; i < numberOfTiles; ++i)
        {
            tiles.push_back(new Tile(_window, _mouse));
            tiles[i]->setSize(tileWidth, tileHeight);
            //Числа захардкожены!!!
            tiles[i]->setPosition(150 * (i % 3 + 1) + windowWidth - 520 - tileWidth, 100 * (i / 3 + 1) + 100);
            tiles[i]->setFillColor(Color::White);

            //Случайным образом выбрал тайлы, с которыми будет работать команда, вычисляющая, какие поля можно атаковать.
            if (i % 3 == i / 3 + 2)
            {
                tiles[i]->setStatus(TileStatus::statusHasUnit);
            }
        }
    }
}

TilesManager::~TilesManager() {}

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
        return;
        break;
    }
    case TilesManagerStatus::statusReleasingUnit:
    {
        if (unitBuffer == nullptr)
        {
            ////////////////////////////////////////////////////////////////////////////////////////COUT
            cout << "ERROR, TilesManager::mouseIsPressed:: === unitBuffer is nullptr === " << endl;

            return;
        }
        for (auto tile : tiles)
        {
            if (tile->hasFocus() && tile->getStatus() == TileStatus::statusIsEmpty)
            {
                ////////////////////////////////////////////////////////////////////////////////////////COUT
                cout << "TilesManager::mouseIsPressed:: === RELEASING UNIT!!! === " << endl;

                tile->setUnit(unitBuffer);
                this->setNormalColors();
                this->status = TilesManagerStatus::statusCardWasJustReleased;
                return;
            }
        }
    }
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
            {
                tile->setFillColor(colorBasic);
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

    //Добавить другие случаи
    default:
        break;
    }
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

Unit* TilesManager::getUnitBuffer()
{
    return unitBuffer;
}