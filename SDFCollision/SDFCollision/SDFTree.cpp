#include "SDFTree.h"

SDFTree::SDFTree(int _targetDepth) : targetDepth(_targetDepth){}

void SDFTree::SetLope(Lope* lope) {
	this->lope = lope;
}

TreeNode * SDFTree::GetNodeByPos(Vector2D pos) {

	queue<TreeNode*> queue;

	queue.push(root);

	while (queue.empty() == false) {
		
		TreeNode* curNode = queue.front();
		queue.pop();

		if(curNode != NULL){

			if (curNode->min.x <= pos.x && pos.x <= curNode->max.x &&
				curNode->min.y <= pos.y && pos.y <= curNode->max.y) {

				if(curNode->nodes[0] != NULL){					// 자식 노드가 존재하면 게속 탐색
					for (int i = 0; i < 4; i++)
						queue.push(curNode->nodes[i]);
				}
				else											// 리프노드일 경우 타겟 노드에 도착
					return curNode;
			}
		}
	}

	return NULL;

}

void SDFTree::buildTree(Vector2D _min, Vector2D _max) {

	queue<TreeNode*> queue;

	root = new TreeNode(_min, _max, 1);

	queue.push(root);

	while (queue.empty() == false) {
	
		TreeNode* curNode = queue.front();
		renderTree.push_back(curNode);
		queue.pop();
		
		if (curNode->CheckNeedDivide() /*&& curNode->CheckError()*/ ) {				// ADFTree로 구성
			
			if(curNode->depth < targetDepth){
				curNode->subdivide();

				for (int i = 0; i < 4; i++)
					queue.push(curNode->nodes[i]);
			}
		}
	}

}


TreeNode::TreeNode(Vector2D _min, Vector2D _max, int _depth) : min(_min), max(_max), depth(_depth + 1){

	for (int i = 0; i < 4; i++)
		nodes[i] = NULL;

}

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

	// 0. 거리장 부호가 모두 음수이면 경계 내부

	// 두가지를 체크.
	// 1. Node가 경계에 걸쳐져 있을 경우
	// 2. 노드의 대각길이보다 표면 거리 길이가 더 긴가? (Node가 경계 외부에 있을 경우)

	int negativeNum = 0;
	double crossVec = (max - min).GetSize();
	bool subdivide = true;

	for (int i = 0; i < 4; i++) {
		
		Vector2D corner = GetCorner(i);
		distances[i] = SDFTree::GetSurfaceDist(corner);

		if (distances[i] <= 0.0)
			negativeNum++;

		if (crossVec < fabs(distances[i]))		// Node가 경계 외부에 있을 경우
			subdivide = false;
	}

	if (0 < negativeNum && negativeNum < 4)		// Node가 경계에 걸쳐 있을 경우
		subdivide = true;

	return subdivide;				// 그렇지 않을경우 divide

}

double SDFTree::GetSurfaceDist(Vector2D& pos) {

	double radius = 0.2;
	double cx = 0.3f, cy = 0.3f;
	double sdf = (pos.x - cx) * (pos.x - cx) + (pos.y - cy) * (pos.y - cy) - radius * radius;

	double square_cx = 0.1f, square_cy = 0.1f;
	double square_radius = 0.3f;
	double square_sdf = fmax(fabs(pos.x - square_cx), fabs(pos.y - square_cy)) - square_radius * square_radius;
	
	return fmin(sdf, square_sdf);

	//return fmax(fabs(pos.x - cx), fabs(pos.y - cy)) - radius * radius;

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

void SDFTree::RenderTree() {

	glColor3d(0, 0, 1);
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
	
	glColor3d(1, 1, 1);

	glBegin(GL_LINES);
	glEnd();
	
}

double SDFTree::Interpolate(Vector2D pos, TreeNode* curNode) {
	
	double nodeX = curNode->max.x - curNode->min.x,
			nodeY = nodeX,									// 노드의 전체 길이 계산, QuadTree이므로 가로, 세로는 동일
			caretX = pos.x - curNode->min.x,
			caretY = pos.y - curNode->min.y,				// 노드 내 캐럿좌표의 길이 계산
			xa = caretX / nodeX,
			ya = caretY / nodeY;							// 캐럿좌표 길이와 노드 길이간 비율 계산

	double p = curNode->distances[0] * (1 - xa) + curNode->distances[3] * xa,
			q = curNode->distances[1] * (1 - xa) + curNode->distances[2] * xa;
			// lerp 결과 p, q

	double result = p * (1 - ya) + q * ya;
			// 최종 interpolation
	/*
	printf("dist 1 2 : %lf %lf\n", curNode->distances[1], curNode->distances[2]);
	printf("dist 0 1 : %lf %lf\n", curNode->distances[0], curNode->distances[1]);
	printf("interpolation result : %lf\n\n", result);
	*/
	return result;
}

Vector2D SDFTree::GetGradient(Vector2D pos, double epsilon) {
	// 유한 차분법으로 SDF 그래디언트를 구한다.
	// SDF 그래디언트는 표면으로 향하는 법선 벡터
	// 오일러 방법으로 미분
	Vector2D incX = Vector2D(pos.x + epsilon, pos.y);
	Vector2D incY = Vector2D(pos.x, pos.y + epsilon);

	TreeNode * targetNode = GetNodeByPos(pos);
	TreeNode * xNode = GetNodeByPos(incX);
	TreeNode * yNode = GetNodeByPos(incY);
	
	double dist = Interpolate(pos, targetNode);
	double distX = Interpolate(incX, xNode);
	double distY = Interpolate(incY, yNode);

	Vector2D result = Vector2D((distX - dist) / epsilon, (distY - dist) / epsilon);
	
	return result.GetUnitVector();
}

void SDFTree::CollisionDetect() {

	for (int i = 0; i < lope->length; i++) {
		Particle* p = lope->particles[i];
		Vector2D pos = p->GetPosition();
		TreeNode* node = GetNodeByPos(pos);
		
		if (node == NULL) 					// Particle의 position이 sdfTree에 없을 경우
			continue;
		else {								// sdfTree 내부에 들어갔을 경우, 충돌 조사 시작
		
			double sdf = Interpolate(pos, node);

			if (sdf <= 0) {					// 파티클이 원 내부로 들어갔을 경우.
											// 충돌 처리 시작.

				Vector2D gradient = GetGradient(pos);
				// 그래디언트 계산
				/*
				Vector2D curVec = p->position - p->old_position;
				Vector2D newAcc = curVec - gradient * (1 + 0.1f) * curVec.Dot(gradient);
				p->SetCurVector(newAcc);
				*/
				//p->AddForce(newAcc);
				// 반발력 계산

				p->SetPosition(p->GetPosition() - gradient * sdf);
				// 관통 보정
				// sdf크기만큼 그래디언트 방향으로 입자의 위치를 옮긴다.

			}

		}	
		//	printf("particle is in sdf\n");
		
	}

}

bool TreeNode::CheckError() {

	double error = 0.0000001;
	for (int i = 0; i < 4; i++) {
		Vector2D edgeCenter = (GetCorner(i % 4) + GetCorner((i + 1) % 4)) / 2;
		double realDist = SDFTree::GetSurfaceDist(edgeCenter);
		double approxDist = (distances[i % 4] + distances[(i + 1) % 4]) / 2;
		double diff = fabs(realDist - approxDist);

		if (diff > error)
			return true;
	}

	Vector2D nodeCenter = (max + min) / 2;
	double realDist = SDFTree::GetSurfaceDist(nodeCenter);
	double approxDist = (distances[0] + distances[1] + distances[2] + distances[3]) / 4;
	double diff = fabs(realDist - approxDist);

	if (diff > error)
		return true;

	return false;

}