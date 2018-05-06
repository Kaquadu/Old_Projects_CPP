#include "stdafx.h"
#include <math.h>
#include <fstream>
#include "SFML\Graphics.hpp"
#include "SFML\System.hpp"
#pragma once

using namespace std;
using namespace sf;

RectangleShape Background;
RectangleShape PenBackground;
RectangleShape InfoBackground;

Texture AngleTexture;
Texture AngleTexture2;
Sprite SetAngleText;
Text AngleText;

Texture LengthTexture;
Texture LengthTexture2;
Sprite SetLengthText;
Text LengthText;

Texture MassTexture;
Texture MassTexture2;
Sprite SetMassText;
Text MassText;

Texture GravityTexture;
Texture GravityTexture2;
Sprite SetGravityText;
Text GravityText;

Texture TimeTexture;
Texture TimeTexture2;
Sprite SetTimeText;
Text TimeText;

Texture StartSimTexture;
Texture StartSimTexture2;
Sprite StartSimText;

Texture GraphsTexture;
Texture GraphsTexture2;
Sprite GraphsText;

Texture PomocTexture;
Texture PomocTexture2;
Sprite PomocText;

Texture NazwaTexture;
Texture NazwaTexture2;
Sprite NazwaSText;
Text NazwaText;

Texture WczytajTexture;
Texture WczytajTexture2;
Sprite WczytajText;

Texture ZapiszTexture;
Texture ZapiszTexture2;
Sprite ZapiszText;

Text InformationText;

Text CurrVelText;
Text CurrAccText;
Text CurrRotAccText;
Text CurrRotVelText;
Text CurrAlpha;
Text CurrForceText;
Text SimTime;

Event WhatYaDoin;
Font font;