#ifndef __CHexGrid_H
#define __CHexGrid_H

#include <vector>
#include <deque>
#include "CHexNode.h"

using namespace std;

class CHexGrid
{
	private:
		vector<vector<CHexNode*>*> mGrid;

		unsigned int mWidth;
		unsigned int mHeight;

		CHexNode* mpFocus;
	public:
		CHexGrid(unsigned int width, unsigned int height);
		CHexGrid(string filePath);
		~CHexGrid();

		CHexNode* GetHex(unsigned int x, unsigned int y);
		CHexNode* GetFocus();
		CHexNode* GetLast();

		void ExtendGrid(int x, int y, bool useDefault);

		deque<CHexNode*>* FindPath(CHexNode* pStart, CHexNode* pTarget);

		unsigned int GetWidth();
		unsigned int GetHeight();

		void SaveToFile(string fileName = "~DEFAULT");

		void SetFocus(CHexNode* pNode);

		void Update(float frameTime);
};

#endif