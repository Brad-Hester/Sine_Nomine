#include "CHexForest.h"

IMesh* CHexForest::mpMeshTree = 0;

CHexForest::CHexForest(int gridX, int gridY) : CHexNode(gridX, gridY)
{
	mpModelHex->SetSkin("Hex_Plain.bmp");
	meTerrainType = FOREST;

	if(mpMeshTree == 0)
	{
		mpMeshTree = CTLManager::GetInstance()->GetEngine()->LoadMesh("Tree.x");
	}
	
	// Plant trees
	for(int i = 0; i < 10; i++)
	{
		mpForest[i] = mpMeshTree->CreateModel(GetWorldX() + ((float)rand() / (float)RAND_MAX)*1.4f + -0.7f , GetWorldY(), GetWorldZ() + ((float)rand() / (float)RAND_MAX)*1.4f + -0.7f);
		mpForest[i]->Scale(0.02f);
	}
}