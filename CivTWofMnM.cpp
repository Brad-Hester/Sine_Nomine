#include <iostream>
#include <vector>

#include "CEditor.h"
#include "CGame.h"

int main()
{
	#ifdef EDIT
		CEditor* pEditor = CEditor::GetInstance();

		while(pEditor->RunGameLoop());
	#else
		CGame* pGame = CGame::GetInstance();

		while(pGame->RunGameLoop());
	#endif

	return 0;
}
