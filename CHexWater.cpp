#include "CHexWater.h"

CHexWater::CHexWater(int gridX, int gridY) : CHexNode(gridX, gridY)
{
	mpModelHex->SetSkin("Hex_Water.bmp");
	meTerrainType = WATER;
}