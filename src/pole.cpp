#include "pole.h"

Pole::Pole()
{
    pole_.resize(0);
    for (int line = 0; line < 3; line++)
    {
        for (int cell = 0; cell < 3; cell++)
        {
            int side = 0;
            Position *position = new Position(cell, line, side);
            pole_.push_back(position);
        }
    }
    for (int line = 0; line < 3; line++)
    {
        for (int cell = 0; cell < 3; cell++)
        {
            int side = 1;
            Position *position = new Position(cell, line, side);
            pole_.push_back(position);
        }
    }
}

void Pole::Show()
{
    Position *PolePosition;
    for (int itcell = 0; itcell < 3; itcell++)
    {
        for (int itline = 2; itline >= 0; itline--)
        {
            PolePosition = GetPosition(itcell, itline, 0);
            if (PolePosition->isEmpty())
                std::cout << 0;
            else
                std::cout << 1;
        }
        std::cout << " ";

        for (int itline = 0; itline < 3; itline++)
        {
            PolePosition = GetPosition(itcell, itline, 1);
            if (PolePosition->isEmpty())
                std::cout << 0;
            else
                std::cout << 1;
        }
        std::cout << std::endl;
    }
}

std::vector<Position *> &Pole::GetVector()
{
    return pole_;
}

Position *Pole::GetPosition(int cell, int line, int side)
{
    for (int i = 0; i < pole_.size(); i++)
    {
        if ((pole_[i]->GetCell() == cell) && (pole_[i]->GetLine() == line) && (pole_[i]->GetSide() == side))
            return pole_[i];
    }
}

void Pole::SetPosition(Position *position)
{
    for (int i = 0; i < pole_.size(); i++)
    {
        if ((pole_[i]->GetCell() == position->GetCell()) && (pole_[i]->GetLine() == position->GetLine()) &&
            (pole_[i]->GetSide() == position->GetSide()))
            pole_[i] = position;
    }
}

void Pole::InfoPosition(Position *position)
{
    if (!position->isEmpty())
    {
        std::cout << "Здоровье Героя:" << position->GetHero().GetCurHealth() << " "
                  << "Атака Героя:"
                  << position->GetHero().GetCurStrength() << std::endl;
    }
}

bool Pole::CheckLeader()
{
    Position *FirstPlayerLeader = GetPosition(1, 1, 0);
    if (FirstPlayerLeader->isEmpty())
    {
        std::cout << "Победил второй игрок!!!!!!";
        return false;
    }
    Position *SecondPlayerLeader = GetPosition(1, 1, 1);
    if (SecondPlayerLeader->isEmpty())
    {
        std::cout << "Победил первый игрок!!!!!!";
        return false;
    }
    return true;
}

std::vector<bool> Pole::CanBeMeleeAttackedRequest(int side)
{
    std::vector<bool> CanBeMeleeAttacked(0);
    if (side == 0)
    {
        for (int itcell = 0; itcell < 3; itcell++)
        {
            for (int itline = 0; itline < 3; itline++)
            {
                Position *position = GetPosition(itcell, itline, 1);
                if (!(position->isEmpty()) && !(position->GetHero().IsDead()))
                {
                    CanBeMeleeAttacked.push_back(true);
                    for (int j = itline + 1; j < 3; j++)
                    {
                        CanBeMeleeAttacked.push_back(false);
                    }
                    itline = 3;
                }
                else
                {
                    CanBeMeleeAttacked.push_back(false);
                }
            }
        }
    }
    else
    {
        for (int itcell = 0; itcell < 3; itcell++)
        {
            for (int itline = 0; itline < 3; itline++)
            {
                Position *position = GetPosition(itcell, itline, 0);
                if (position->isEmpty() == false)
                {
                    CanBeMeleeAttacked.push_back(true);
                    for (int j = itline + 1; j < 3; j++)
                    {
                        CanBeMeleeAttacked.push_back(false);
                    }
                    itline = 3;
                }
                else
                {
                    CanBeMeleeAttacked.push_back(false);
                }
            }
        }
    }
    std::vector<bool> CanBeMeleeAttackedResult(0);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < CanBeMeleeAttacked.size(); j = j + 3){
            CanBeMeleeAttackedResult.push_back(CanBeMeleeAttacked[i+j]);
        }
    }
    return CanBeMeleeAttackedResult;
}
Position *Pole::GetLeaderPos(int player)
{
    Position *leader;
    leader = GetPosition(1, 1, player);
}
