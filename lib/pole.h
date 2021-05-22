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
    bool CheckLeader();
    std::vector<bool> CanBeMeleeAttackedRequest(int side);
private:
    std::vector<Position*> pole_;
};


#endif
