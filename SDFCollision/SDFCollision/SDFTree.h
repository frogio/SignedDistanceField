#pragma once
#include <vector>
#include <queue>
#include "glut.h"
#include "Vector2D.h"
#include "Lope.h"

using namespace std;

class TreeNode;

class SDFTree {	
public:
	TreeNode* root;
	vector<TreeNode*> renderTree;
	int targetDepth;
	Lope* lope;

	SDFTree(int _targetDepth);
	static double GetSurfaceDist(Vector2D& pos);
	void buildTree(Vector2D _min, Vector2D _max);
	void RenderTree();
	TreeNode* GetNodeByPos(Vector2D pos);
	double Interpolate(Vector2D pos, TreeNode * curNode);
	Vector2D GetGradient(Vector2D pos, double epsilon = 0.000001);
	void CollisionDetect();
	void SetLope(Lope* lope);
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
	bool CheckError();						// ADFTree
	void subdivide();

};