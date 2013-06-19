#ifndef __CHexNode_H
#define __CHexNode_H

#include "CTLManager.h"
#include "CUnit.h"

enum ETerrainType{PLAIN, FOREST, WATER};
enum EBorderType{NONE, ACTIVE};

class CHexNode
{
	protected:
		// Visuals
		static IMesh* mpMeshHex;
		static IMesh* mpMeshBorder;

		IModel* mpModelHex;
		IModel* mpModelBorder;

		// Path finding tools
		int			 mGridX;
		int			 mGridY;
		int			 mDistance;
		int			 mTerrainHeight;
		int			 mCost;
		CHexNode*	 mpParent;
		ETerrainType meTerrainType;
		EBorderType  meBorderType;
		CUnit*		 mpUnit;

	public:
		CHexNode(int gridX, int gridY); 
		virtual ~CHexNode();

		float		 GetWorldX();
		float		 GetWorldY();
		float		 GetWorldZ();
		int			 GetGridX();
		int			 GetGridY();
		int			 GetTerrainHeight();
		ETerrainType GetTerrainType();
		CHexNode*	 GetParent();
		int			 GetCost();
		int			 GetDistance();
		CUnit*		 GetUnit();
		
		void SetParent(CHexNode* pParent);
		void SetTerrainHeight(int height);
		void SetBorderType(EBorderType eType);
		void SetCost(int cost);
		void SetDistance(int dist);

		void AddTerrainHeight(int height);

		bool AddNewUnit(EUnitType eType);
		bool AddUnit(CUnit* pUnit);
		bool MoveUnitTo(CHexNode* pTarget);

		int DistanceTo(CHexNode* pTarget);
		
		void Update(float frameTime);
};

#endif