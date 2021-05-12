#include "Tile.h"

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
    this->status = statusHasUnit;
    unit->setPosition(this->rect.getPosition().x, this->rect.getPosition().y);
}

void Tile::setStatus(TileStatus _status)
{
    this->status = _status;
}

int Tile::getStatus()
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
TilesManager::TilesManager()
{
    status = statusNothingHappens;
    unitBuffer = nullptr;
}

TilesManager::~TilesManager() {}

void TilesManager::setStatus(const commandStatus &_status)
{
    this->status = _status;
    switch (status)
    {
    case statusReleasingUnit:
    {
        for (auto tile : tiles)
        {
            if (tile->getStatus() == statusIsEmpty)
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

void TilesManager::setTiles(vector<Tile *> _tiles)
{
    for (auto _tile : _tiles)
    {
        tiles.push_back(_tile);
    }
}

void TilesManager::mouseIsPressed()
{
    switch (status)
    {
    case statusNothingHappens:
    {
        return;
        break;
    }
    case statusReleasingUnit:
    {
        if (unitBuffer == nullptr)
        {
            cout << "ERROR, TilesManager::mouseIsPressed:: === unitBuffer is nullptr === " << endl;
            return;
        }
        for (auto tile : tiles)
        {
            if (tile->hasFocus() && tile->getStatus() == statusIsEmpty)
            {
                cout << "TilesManager::mouseIsPressed:: === RELEASING UNIT!!! === " << endl;
                tile->setUnit(unitBuffer);
                this->setNormalColors();
                this->status = statusNothingHappens;
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
    case statusNothingHappens:
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
    case statusReleasingUnit:
        for (auto tile : tiles)
        {
            if (tile->hasFocus() && tile->getStatus() == statusIsEmpty)
            {
                tile->setFillColor(colorInFocus);
            }
            else
            {
                if (tile->getStatus() == statusIsEmpty)
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