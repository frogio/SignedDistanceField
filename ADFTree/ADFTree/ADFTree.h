#pragma once
#include <vector>
#include <queue>
#include "glut.h"
#include "Vector2D.h"

using namespace std;

class TreeNode;

class ADFTree {	
public:
	TreeNode* root;
	vector<TreeNode*> renderTree;
	int targetDepth;

	ADFTree(int _targetDepth);
	static double GetSurfaceDist(Vector2D& pos);
	void buildTree(Vector2D _min, Vector2D _max);
	void RenderTree();

};

class TreeNode {
public:
	TreeNode* nodes[4];
	int depth;
	Vector2D min;
	Vector2D max;
	double distances[4];

	TreeNode(Vector2D _min, Vector2D _max, int _depth);
	Vector2D GetCorner(int index);
	bool CheckNeedDivide();
	bool CheckError();
	void subdivide();
	
};