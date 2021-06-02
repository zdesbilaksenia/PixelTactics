//Window
const int windowHeight = 720;
const int windowWidth = 1080;

//Menu
const int playButtonWidth = 300;
const int playButtonHeight = 80;

//Buttons
const int buttonWidth = 200;
const int buttonHeight = 40;

//Cards
const int cardWidth = 150;
const int cardHeight = 250;
const int cardsInStackPosX = 40;
const int cardsInStackPosY = 500;
const int cardsInHandPosX = 300;
const int cardsInHandPosY = 500;
const int maxNumberOfCardsInHand = 5;

//Units
const int unitWidth = 100;
const int unitHeight = 150;
const int statPictureWidth = 25;
const int statPictureHeight = 25;
const int infoRectWidth = 616;
const int infoRectHeight = 171;
const int infoRectPosX = 300;
const int infoRectPosY = 10;
const int textDescPosX = infoRectPosX + 60;
const int textDescPosY = infoRectPosY + 55;


//Tiles
const int numberOfTiles = 9;
const int tileWidth = 120;
const int tileHeight = 80;

//Background
const float parallaxCoefficient = -0.05f;
const int backgroundWidth = windowWidth * 1.2;
const int backgroundHeight = windowHeight;
const float backgroundPosX = 200;
const float backgroundPosY = -100;
const int lowerPanelWidth = windowWidth;
const int lowerPanelHeight = windowWidth / 4;
const int lowerPanelPosX = 0;
const int lowerPanelPosY = windowHeight - windowWidth / 4 + 100;
const int stageTextWidth = 400;
const int stageTextHeight = 100;
const int stageTextPosX = (windowWidth - 400) / 2;
const int stageTextPosY = 50;

//Game
const int numberOfCards = 20;
const int numberOfUnits = 11;


//StageText
const int numberOfTexts = 6;
const int textWidth = 460;
const int textHeight = 50;
const int textPosX = (windowWidth - textWidth)/2;
const int textPosY = 300;
const int maxNumberOfMoments = 1000;

//Чтобы визуально сжать код
#define GAME_ELEMENTS 1