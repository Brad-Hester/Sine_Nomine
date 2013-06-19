#include "CGUI.h"

#include <sstream>
using std::stringstream;

const unsigned int LEVEL_COUNT = 20;

CTLManager* pManager = CTLManager::GetInstance();
CAudioManager* mpAudio = pManager->GetAudio();
I3DEngine* myEngine = pManager->GetEngine();
ICamera* mpCamera = pManager->GetCamera();


IFont* PauseFont;
IFont* GameFont;
ISprite* FrontEndBackdrop; //GUI Interface
ISprite* PauseBackdrop;
ISprite* GameBackdrop;
ISprite* playButton;
ISprite* quitButton;
ISprite* playPButton;
ISprite* quitPButton;
ISprite* portrait;
ISprite* levelButton[LEVEL_COUNT];

//GUI member functions

CGUI::CGUI()
{
	FrontEndSetup();
	
	mpUnit = 0;
}

EGameState CGUI::Update()
{
	switch(meState)
	{
		case FRONT_END:
			FrontEndUpdate();
		break;
		case PAUSE:
			PauseUpdate();
		break;
		case IN_GAME:
			InGameUpdate();
		break;
	}

	return meState;
}

void CGUI::FrontEndSetup()
{	
	//Dynamic Position for the backdrop and buttons
	int height = myEngine->GetHeight() /2 - 250;
	int width = myEngine->GetWidth() /2 - 225;	

	playButton = myEngine->CreateSprite( "play.png", width + 125, height + 175);
	quitButton = myEngine->CreateSprite( "quit.png", width + 125, height + 250);
	FrontEndBackdrop = myEngine->CreateSprite( "mainmenu.png", width, height, 0.9f );

	meState = FRONT_END;
}

int CGUI::FrontEndUpdate()
{
	int MX = myEngine->GetMouseX();
	int MY = myEngine->GetMouseY();

	//If Mouse Click
	if (myEngine->KeyHit( Mouse_LButton ))
	{	
		//And its over a button
		if(ButtonHover(playButton, MX,MY))
		{
			FrontEndShutdown();
			InGameSetup();
			mpAudio->play(CLICK);

			return 1;
		}
		if(ButtonHover(quitButton, MX, MY))
		{
			FrontEndShutdown();
			meState = SHUTDOWN;
			mpAudio->play(CLICK);

			return 2;
		}
	}	

	return 0;
}
void CGUI::FrontEndShutdown()
{
	myEngine->RemoveSprite( playButton );
	myEngine->RemoveSprite( quitButton );
	myEngine->RemoveSprite( FrontEndBackdrop);
}

void CGUI::PauseSetup()
{
	//Dynamic Position for the backdrop and buttons
	int height = myEngine->GetHeight() /2 - 250;
	int width = myEngine->GetWidth() /2 - 225;

	playPButton = myEngine->CreateSprite( "play.png", width + 125, height + 175);
	quitPButton = myEngine->CreateSprite( "quit.png", width + 125, height + 250);
	PauseBackdrop = myEngine->CreateSprite( "pausemenu.png", width, height, 0.1f);

	meState = PAUSE;
}

int CGUI::PauseUpdate()
{
	int MX = myEngine->GetMouseX();
	int MY = myEngine->GetMouseY();

	//If Mouse Click
	if (myEngine->KeyHit( Mouse_LButton ))
	{	
		//And its over a button
		if(ButtonHover(playPButton, MX,MY))
		{
			PauseShutdown();
			InGameSetup();
			mpAudio->play(CLICK);

			return 1;
		}
		if(ButtonHover(quitPButton, MX, MY))
		{
			PauseShutdown();
			meState = SHUTDOWN;
			mpAudio->play(CLICK);

			return 2;
		}
	}

	if(myEngine->KeyHit(Key_Escape))
	{
		PauseShutdown();
		InGameSetup();
		mpAudio->play(CLICK);
	}
		
	return 0;
					
}

void CGUI::PauseShutdown()
{
	myEngine->RemoveSprite( playPButton );
	myEngine->RemoveSprite( quitPButton );
	myEngine->RemoveSprite( PauseBackdrop);
}

void CGUI::InGameSetup()
{
	//Dynamically place the backdrop
	int height = myEngine->GetHeight() /*/2*/ - 200;
	int width = myEngine->GetWidth() /2 - 800;

	//Load a font
	GameFont = myEngine->LoadFont( "Airstrip One", 26 );

	GameBackdrop = myEngine->CreateSprite( "bottomI.png", width, height, 0.2f);

	// Coming from pause, might be a unit selected
	if(mpUnit != 0)
	{
		portrait = myEngine->CreateSprite("portrait.png", myEngine->GetWidth() / 2 - 100, myEngine->GetHeight() - 220, 0.1f);
	}

	meState = IN_GAME;
}

void CGUI::InGameUpdate()
{
	if(mpUnit != 0)
	{
		int centerX = myEngine->GetWidth() / 2;
		int height = myEngine->GetHeight();

		stringstream outText;
		outText << "health: " << mpUnit->GetHitPoints();
		GameFont->Draw( outText.str(), centerX - 200, height - 75, kWhite, kRight );
		outText.str(""); 
		outText << "movement: " << mpUnit->GetMovement();
		GameFont->Draw( outText.str(), centerX - 200, height - 50, kWhite, kRight );
		outText.str(""); // How to clear outText for further draw calls
		outText << "attack: " << mpUnit->GetAttack();
		GameFont->Draw( outText.str(), centerX + 200, height - 75, kWhite ); 
		outText.str("");
		outText << "defense: " << mpUnit->GetDefense();
		GameFont->Draw( outText.str(), centerX + 200, height - 50, kWhite );
		outText.str("");

		// New selection
		if(portrait == 0)
		{
			portrait = myEngine->CreateSprite("portrait.png", centerX - 100, height - 220, 0.1f);
		}
	}

	if(myEngine->KeyHit(Key_Escape))
	{
		InGameShutdown();
		PauseSetup();
		mpAudio->play(CLICK);
	}
}

void CGUI::InGameShutdown()
{
	myEngine->RemoveFont( GameFont );
	myEngine->RemoveSprite( GameBackdrop);
	if(portrait != 0)
	{
		myEngine->RemoveSprite(portrait);
	}
}

bool CGUI::ButtonHover(ISprite* button, int MX, int MY)
{
	//Return Value (True for button click)
	bool click = false;

	//Get the coordinates of the button
	float ButX = button->GetX();
	float ButY = button->GetY();

	//Create a bounding box
	float MaxX = ButX + butWidth;
	float MaxY = ButY + butHeight;

	//If the mouse is within the button boundaries
	if (MX < MaxX && MX > ButX && MY < MaxY && MY > ButY)
	{
		click = true;
	}

	return click;
}

void CGUI::DisplayUnit(CUnit* pUnit)
{
	mpUnit = pUnit;

	// Deselect
	if(pUnit == 0 && portrait != 0)
	{
		myEngine->RemoveSprite(portrait);
		portrait = 0;
	}
}

CGUI::~CGUI()
{
}