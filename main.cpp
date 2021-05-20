#include <iostream>
#include "DataBase/MySQL.h"
#include "Card.h"
#include "Powers.h"
#include "PowerMapper.h"
#include <vector>

using std::vector;
using std::cout;
using std::cin;

int main() {
    int n;

    MySQL sql;
    vector<Card> cards = sql.GetCards();

    cout << "Распечатываем карточки\n";
    cout << "ID   |    NAME    |  STRENGTH   |     HP  \n";
    for (int i = 0; i < cards.size(); i++) {
        cout << cards[i].ID << "   |     " << cards[i].name << "      |     " << cards[i].strength << "   |  "
             << cards[i].HP << "\n";
    }

    cout << "Рандомный отряд\n";
    vector<Hero *> heroes;
    for (int i = 0; i < cards.size(); i++) {
        heroes.push_back(new Hero(cards[i]));
    }

    cout << "Назначаем лидера\nВведите номер: ";
    cin >> n;
    heroes[n - 1]->MakeLeader();

    cout << "Маппим способности\n";
    PowerMapper mapper;
    for (int i = 0; i < heroes.size(); i++) {
        mapper.MapPowers(*heroes[i]);
    }

    int option;
    int *m = new int[10];
    vector<Hero *> check;
    cout << "Играем\n";
    while (true) {
        cout << "ID    |    Имя    |    HP   | Сила  \n";
        for (int i = 0; i < heroes.size(); i++) {
            cout << heroes[i]->GetID() << " |  " << heroes[i]->GetName() << " |  " << heroes[i]->GetCurHealth()
                 << "  |   " << heroes[i]->GetCurStrength()
                 << "\n";
        }
        check.clear();
        cout << "Введите ID персонажа: ";
        cin >> n;
        cout << "1-Аттаковать, 2-Способность авангарда, 3-Способность фланга, 4-Способность тыла\n";
        cin >> option;
        switch (option) {
            case 1: {
                cout << "Введите ID персонажа, которого атакуем: ";
                cin >> m[0];
                heroes[n]->Attack(*heroes[m[0] - 1], heroes[n - 1]->GetCurStrength());
                break;
            }
            case 2: {
                cout << cards[n - 1].name << ": " << cards[n - 1].frontLinePower << "\n";
                cout << "Сколько героев задействовано в спопобности: ";
                cin >> m[0];
                for (int i = 0; i < m[0]; i++) {
                    cout << "Выберите героев для способности: ";
                    cin >> m[1];
                    check.push_back(heroes[m[1] - 1]);
                }
                heroes[n - 1]->frontLinePower(check);
                cout << "Готово\n";
                break;
            }
            case 3: {
                cout << cards[n - 1].name << ": " << cards[n - 1].middleLinePower << "\n";
                cout << "Сколько героев задействовано в спопобности: ";
                cin >> m[0];
                for (int i = 0; i < m[0]; i++) {
                    cout << "Выберите героев для способности: ";
                    cin >> m[1];
                    check.push_back(heroes[m[1] - 1]);
                }
                heroes[n - 1]->middleLinePower(check);
                cout << "Готово\n";
                break;
            }
            case 4: {
                cout << cards[n - 1].name << ": " << cards[n - 1].backLinePower << "\n";
                cout << "Сколько героев задействовано в спопобности: ";
                cin >> m[0];
                for (int i = 0; i < m[0]; i++) {
                    cout << "Выберите героев для способности: ";
                    cin >> m[1];
                    check.push_back(heroes[m[1] - 1]);
                }
                heroes[n - 1]->backLinePower(check);
                cout << "Готово\n";
                break;
            }
        }
    }

    return 0;
}
