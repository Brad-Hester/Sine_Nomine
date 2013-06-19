#ifndef __CHexForest_H
#define __CHexForest_H

#include "CHexNode.h"

class CHexForest : public CHexNode
{
	private:
		static IMesh* mpMeshTree;
		IModel* mpForest[10];
	public:
		CHexForest(int gridX, int gridY);
};

#endif