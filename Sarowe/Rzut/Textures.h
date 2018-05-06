#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Textures
{
public:
    Textures(){
        font.loadFromFile("Data/font.ttf");
        setMassText.setFont(font);
        setMassText.setCharacterSize(15);
        setMassText.setString("Zmien Mase");
        setGravText.setFont(font);
        setGravText.setCharacterSize(15);
        setGravText.setString("Zmien Grawitacje");
        setAlphaText.setFont(font);
        setAlphaText.setCharacterSize(15);
        setAlphaText.setString("Zmien Kat");
        setVelText.setFont(font);
        setVelText.setCharacterSize(15);
        setVelText.setString("Zmien Predkosc");

        startSimText.setFont(font);
        startSimText.setCharacterSize(25);
        startSimText.setString("Backspace - rozpocznij symulacje");

        wykresyText.setFont(font);
        wykresyText.setCharacterSize(25);
        wykresyText.setString("W - pokaz wykresy");

        wykrPredkoscText.setFont(font);
        wykrPredkoscText.setCharacterSize(15);
        wykrPredkoscText.setString("1. Predkosc liniowa / czas");

        wykrPredkoscWznText.setFont(font);
        wykrPredkoscWznText.setCharacterSize(15);
        wykrPredkoscWznText.setString("2. Predkosc wznoszenia / czas");

        wykrPrzemieszczenieText.setFont(font);
        wykrPrzemieszczenieText.setCharacterSize(15);
        wykrPrzemieszczenieText.setString("3. Droga / czas");

        wykrWysokoscText.setFont(font);
        wykrWysokoscText.setCharacterSize(15);
        wykrWysokoscText.setString("4. Wysokosc / czas");

        wykrOdlegloscText.setFont(font);
        wykrOdlegloscText.setCharacterSize(15);
        wykrOdlegloscText.setString("5. Odleglosc / czas");

        setFileNameText.setFont(font);
        setFileNameText.setCharacterSize(15);
        setFileNameText.setString("Zmien nazwe\npliku zapisu");

        saveText.setFont(font);
        saveText.setCharacterSize(15);
        saveText.setString("Zapisz");

        loadText.setFont(font);
        loadText.setCharacterSize(15);
        loadText.setString("Wczytaj");

        arrowTexture.loadFromFile("Data/arrow.png");
        arrowSprite.setTexture(arrowTexture);

        backgroundTexture.loadFromFile("Data/background.png");
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(2, 2);

        hitLine.setFillColor(Color::White);
        hitLine.setSize(Vector2f(800, 10));
    };
    ~Textures(){};
    Font font;
    Text setMassText;
    Text setGravText;
    Text setAlphaText;
    Text setVelText;
    Text setFileNameText;

    Text velText;
    Text massText;
    Text gravText;
    Text alphaText;
    Text fileNameText;

    Text startSimText;
    Text wykresyText;

    Text saveText;
    Text loadText;

    Text wykrPredkoscText;
    Text wykrPredkoscWznText;
    Text wykrPrzemieszczenieText;
    Text wykrWysokoscText;
    Text wykrOdlegloscText;

    Texture arrowTexture;
    Sprite arrowSprite;

    Texture backgroundTexture;
    Sprite backgroundSprite;

    RectangleShape hitLine;
};
