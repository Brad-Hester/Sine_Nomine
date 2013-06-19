#ifndef CH
#define CH

#include "CTLManager.h"
#include "CUnit.h"

enum EGameState {FRONT_END, IN_GAME, PAUSE, SHUTDOWN};

//GUI Class
class CGUI
{

private:
	static const int butHeight = 50;
	static const int butWidth = 200;

	//Button Hover
	bool ButtonHover(ISprite* button, int MX, int MY);

	EGameState meState;
	CUnit* mpUnit; // Unit to display stats of
public:
	//Constructor
	CGUI();

	//Front end menu
	void FrontEndSetup();
	int FrontEndUpdate();
	void FrontEndShutdown();

	//In Game Interface
	void InGameSetup();
	void InGameUpdate();
	void InGameShutdown();

	void DisplayUnit(CUnit* pUnit);
	
	//Pause Menu
	void PauseSetup();
	int PauseUpdate();
	void PauseShutdown();

	EGameState Update();

	//Destructor
	~CGUI();

};

#endif