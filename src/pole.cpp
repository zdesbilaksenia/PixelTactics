#include "../lib/pole.h"

Pole::Pole(){
    pole_.resize(0);
    int i = 0;
    for(int cell = 0; cell < 3; cell++){
        for(int line = 0; line < 3; line++){
            for(int side = 0; side < 2; side++){
                Position position(cell,line,side);
                pole_.push_back(position);
                i++;
            }
        }
    }
}

void Pole::Show(){
    Position PolePosition;
    for(int itcell = 0; itcell < 3; itcell++){
        for(int itline = 2; itline >= 0; itline--){
            PolePosition = GetPosition(itcell, itline, 0);
            if(PolePosition.isEmpty())
                std::cout << 0;
            else
                std::cout << 1;
        }
        std::cout << " ";

        for(int itline = 0; itline < 3; itline++){
            PolePosition = GetPosition(itcell, itline, 1);
            if(PolePosition.isEmpty())
                std::cout << 0;
            else
                std::cout << 1;
        }
        std::cout << std::endl;
    }
}

std::vector <Position>& Pole::GetVector(){
    return pole_;
}

Position& Pole::GetPosition(int cell, int line, int side){
    for(int i = 0; i < pole_.size(); i++){
        if((pole_[i].GetCell() == cell) && (pole_[i].GetLine() == line) && (pole_[i].GetSide() == side))
            return pole_[i];
    }
}

void Pole::SetPosition(Position& position){
    for(int i = 0; i < pole_.size(); i++){
        if((pole_[i].GetCell() == position.GetCell()) && (pole_[i].GetLine() == position.GetLine()) && (pole_[i].GetSide() == position.GetSide()))
            pole_[i] = position;
    }
}

void Pole::InfoPosition(Position& position){
    if(!position.isEmpty()){
        std::cout << "Здоровье Героя:" << position.GetHero().GetHealth() << " " << "Атака Героя:" << position.GetHero().GetStrength() << std::endl; 
    }
}

bool Pole::CheckLeader(){
    Position FirstPlayerLeader = (*this).GetPosition(1,1,0);
    if(FirstPlayerLeader.isEmpty()){
        std::cout << "Победил второй игрок!!!!!!";
        return false;
    }
    Position SecondPlayerLeader = (*this).GetPosition(1,1,1);
    if(SecondPlayerLeader.isEmpty()){
        std::cout << "Победил первый игрок!!!!!!";
        return false;
    }
    return true;
}
