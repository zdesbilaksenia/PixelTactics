#pragma once
#ifndef POLE_H
#define POLE_H

#include "position.h"
#include <vector>
#include <iostream>


class Pole{
public:
    Pole();
    void Show();
    std::vector <Position*>& GetVector();
    Position* GetPosition(int cell, int line, int side);
    void SetPosition(Position* position);
    void InfoPosition(Position* position);
    int CheckLeader();
    std::vector<bool> CanBeMeleeAttackedRequest(int side);
    Position* GetLeaderPos(int player);
private:
    std::vector<Position*> pole_;
};


#endif
