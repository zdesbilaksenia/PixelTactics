#include "Command.h"
#include "Card.h"

const int maxNumberOfCards = 6;

class CommandTakeCard : public Command
{
    public:
        CommandTakeCard(CardsManager& _cardsManager, GameTcpClient &_client);

        void execute() override;
        ~CommandTakeCard();

    private:
        CardsManager& cardsManager;
        GameTcpClient& client;
};