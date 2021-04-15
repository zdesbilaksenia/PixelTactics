#ifndef PIXELTACTICS_POWERS_H
#define PIXELTACTICS_POWERS_H

using std::vector;

typedef void (*power)();

void PlusPower();

void Resurrect();

void HitFrontRowHeroes();

void HitOneColumn();

void TransferDamage();

class Powers {

private:
    vector<power> frontLinePowerVector;
    vector<power> middleLinePowerVector;
    vector<power> backLinePowerVector;
    vector<power> leaderPowerVector;
public:
    void FillVectors() {};

    void ReturnPower(int ID) {};
};

/*void Powers::FillVectors() {

    frontLinePowerVector.push_back(PlusPower);
    frontLinePowerVector.push_back(Resurrect);

    middleLinePowerVector.push_back(HitFrontRowHeroes);

    backLinePowerVector.push_back(HitOneColumn);

    leaderPowerVector.push_back(TransferDamage);

}*/

#endif //PIXELTACTICS_POWERS_H
