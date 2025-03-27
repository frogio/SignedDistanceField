#include "ADFTree.h"

ADFTree::ADFTree(int _targetDepth) : targetDepth(_targetDepth) {}

void ADFTree::buildTree(Vector2D _min, Vector2D _max) {

	queue<TreeNode*> queue;

	root = new TreeNode(_min, _max, 1);

	queue.push(root);

	while (queue.empty() == false) {
	
		TreeNode* curNode = queue.front();
		renderTree.push_back(curNode);
		queue.pop();
		
		if (curNode->CheckNeedDivide() && curNode->CheckError()){	// 노드가 경계 근처에 있고, 보간 에러율이 크지 않을경우 나누지 않는다.
			
			if(curNode->depth < targetDepth){
				curNode->subdivide();

				for (int i = 0; i < 4; i++)
					queue.push(curNode->nodes[i]);
			}
		}
	}

}


TreeNode::TreeNode(Vector2D _min, Vector2D _max, int _depth) : min(_min), max(_max), depth(_depth + 1) {}

Vector2D TreeNode::GetCorner(int index) {
	
	switch (index) {
		case 0:
			return min;
		case 1:
			return Vector2D(min.x, max.y);
		case 2:
			return max;
		case 3:
			return Vector2D(max.x, min.y);
	}

	return Vector2D();

}

bool TreeNode::CheckNeedDivide() {

	// 두가지를 체크.
	// 1. Node가 경계에 걸쳐져 있을 경우
	// 2. 노드의 대각길이보다 표면 거리 길이가 더 긴가? (Node가 경계 외부에 있을 경우)

	int negativeNum = 0;
	double crossVec = (max - min).GetLength();
	bool subdivide = true;

	for (int i = 0; i < 4; i++) {
		
		Vector2D corner = GetCorner(i);
		distances[i] = ADFTree::GetSurfaceDist(corner);

		if (distances[i] <= 0.0)
			negativeNum++;

		if (crossVec < fabs(distances[i]))		// Node가 경계 외부에 있을 경우
			subdivide = false;
	}

	if (0 < negativeNum && negativeNum < 4)		// Node가 경계에 걸쳐 있을 경우
		subdivide = true;

	return subdivide;				// 그렇지 않을경우 divide

}

double ADFTree::GetSurfaceDist(Vector2D& pos) {

	double radius = 0.5;
	double cx = 0.65, cy = 0.65;

	return fmax(fabs(pos.x - cx), fabs(pos.y - cy)) - radius * radius;

	//return powf(pos.x - cx, 2.0) + powf(pos.y - cy, 2.0) - radius * radius;
}

void TreeNode::subdivide() {

	Vector2D center = (max + min) / 2;

	Vector2D quadrant1Max = max;
	Vector2D quadrant1Min = center;
	
	nodes[0] = new TreeNode(quadrant1Min, quadrant1Max, depth + 1);

	Vector2D quadrant2Max = Vector2D(center.x, max.y);
	Vector2D quadrant2Min = Vector2D(min.x, center.y);

	nodes[1] = new TreeNode(quadrant2Min, quadrant2Max, depth + 1);

	Vector2D quadrant3Max = center;
	Vector2D quadrant3Min = min;

	nodes[2] = new TreeNode(quadrant3Min, quadrant3Max, depth + 1);

	Vector2D quadrant4Max = Vector2D(max.x, center.y);
	Vector2D quadrant4Min = Vector2D(center.x, min.y);

	nodes[3] = new TreeNode(quadrant4Min, quadrant4Max, depth + 1);

}

void ADFTree::RenderTree() {
	
	glBegin(GL_LINES);

	for (int i = 0;i < renderTree.size(); i++) {
		TreeNode* node = renderTree[i];
		
		glVertex2d(node->max.x, node->max.y);
		glVertex2d(node->min.x, node->max.y);

		glVertex2d(node->min.x, node->max.y);
		glVertex2d(node->min.x, node->min.y);

		glVertex2d(node->min.x, node->min.y);
		glVertex2d(node->max.x, node->min.y);

		glVertex2d(node->max.x, node->min.y);
		glVertex2d(node->max.x, node->max.y);

	}

	glEnd();
}

bool TreeNode::CheckError() {
	
	double error = 0.00001;

	for (int i = 0; i < 4; i++) {
		Vector2D edgeCenter = (GetCorner(i % 4) + GetCorner((i + 1) % 4)) / 2.f;
		double realDist = ADFTree::GetSurfaceDist(edgeCenter);
		double approxDist = (distances[i] + distances[(i + 1) % 4]) / 2.f;
		double diff = fabs(realDist - approxDist);
		if (diff > error)
			return true;
	}
	
	Vector2D nodeCenter = (max + min) / 2;
	double realDist = ADFTree::GetSurfaceDist(nodeCenter);
	double approxDist = (distances[0] + distances[1] + distances[2] + distances[3]) / 4.f;
	double diff = fabs(realDist - approxDist);

	if (diff > error)
		return true;

	return false;
}