#include "headers/tree.h"

TreeNode::TreeNode() :QWidget() {
	lowCount = 0;
	index = 0;
	low = nullptr;
	high = nullptr;
	right = nullptr;
	left = nullptr;
	data = "";
}
TreeNode::TreeNode(const TreeNode& copy) : QWidget() {//QWidget에 TreeNode에 대한 convert constructor 필요
	data = copy.data;
	lowCount = copy.lowCount;
	index = copy.index;
	low = new TreeNode*[copy.lowCount];
	for (int i = 0;i < copy.lowCount;i++)
		low[i] = copy.low[i];
	high = copy.high;
	left = copy.left;
	right = copy.right;
}

TreeNode& TreeNode::operator=(const TreeNode& node) {
	if (&node == this)
		return *this;
    if (this->low != nullptr)
		clear();
	data = node.data;
	lowCount = node.lowCount;
	index = node.index;
	low = new TreeNode*[node.lowCount];
	for (int i = 0;i < node.lowCount;i++)
		low[i] = node.low[i];
	high = node.high;
	left = node.left;
	return *this;
}
TreeNode& TreeNode::operator+(TreeNode& node) {
	TreeNode* highNode = new TreeNode;
	*highNode += *this;
	*highNode += node;
	return *highNode;
}
TreeNode& TreeNode::operator+=(TreeNode& node) {
	if (&node == this)
		return *this;
	node.disconnectUpperLink();
	addLowNode(&node);
	return *this;
}

void TreeNode::arrCopy(TreeNode** to, TreeNode** from, int toSize, int index) {	//배열의 단순 복사와 삭제 복사를 나눈다. index가 -1이면 단순복사, 아니면 index에 해당하는 원소 삭제 복사
	int i, j;												//index는 from에서 사라질 원소이다.
    if (to == nullptr || from == nullptr);	//배열 포인터가 NULL인 상태에서 접근하면 안된다.
										//예외처리
	if (index == -1) {
		for (i = 0;i < toSize - 1;i++)	//현재 to보다 from이 원소 한개가 적다. 때문에 -1해줌.
			to[i] = from[i];
	}
	else {	//index에 해당하는 원소를 하나만 뺄 예정.
		for (i = 0, j = 0;i < toSize;i++) {	//현재 to보다 from이 원소 한개가 더 많다. 그리고 from에서 한 개의 원소를 없애고 to에 넣는다. 따라서 오류 없이 복사 가능. - 호출하는 부분의 코드를 보면 이해에 도움이 됨
			if (index != i) {
				to[j] = from[i];	//from에서 index 하나 빼고 to에 대입.
				to[j]->index = j;	//index가 소거된 원소 뒤로 1씩 밀리므로, 재설정.
				j++;
			}
			else
				continue;
		}
	}
}
TreeNode* TreeNode::addLowNode() {
	TreeNode** addTemp;
	TreeNode* addNode = new TreeNode;
	lowCount++;
	addTemp = low;
	low = new TreeNode*[lowCount];	//한 칸 더 큰 배열 생성
    if (addTemp != nullptr) {
		TreeNode::arrCopy(low, addTemp, lowCount, -1);
		delete[] addTemp;
	}
	low[lowCount - 1] = addNode;	//아래로 연결
	addNode->high = this;	//위로도 연결
	addNode->index = lowCount - 1;	//index를 갱신
	addNode->connectLevelLink();
	addNode->setParent(this);
	return addNode;
}
void TreeNode::addLowNode(TreeNode* addNode) {		//트리의 특정 노드(currentNode)에 주어진 하위노드 추가
	TreeNode** addTemp;
	lowCount++;
	addTemp = low;
	low = new TreeNode*[lowCount];	//한 칸 더 큰 배열 생성
    if (addTemp != nullptr) {
		TreeNode::arrCopy(low, addTemp, lowCount, -1);
		delete[] addTemp;
	}
	low[lowCount - 1] = addNode;	//아레로 연결
	addNode->high = this;	//위로도 연결
	addNode->index = lowCount - 1;	//index를 갱신
	addNode->connectLevelLink();
	addNode->setParent(this);
}
void TreeNode::addLowNode(const int num) {
	TreeNode** addTemp;
	TreeNode* addNode;
	addTemp = low;
	low = new TreeNode*[lowCount + num];
    if (addTemp != nullptr) {
		TreeNode::arrCopy(low, addTemp, lowCount + num, -1);
		delete[] addTemp;
	}
	for (int i = 0; i < num; i++) {
		addNode = new TreeNode;
		addNode->high = this;
		addNode->index = lowCount;
		low[lowCount] = addNode;
		lowCount++;
		addNode->connectLevelLink();
		addNode->setParent(this);
	}
}
void TreeNode::disconnectUpperLink() {
	TreeNode** temp;
    if (high != nullptr) {
        high->low[index] = nullptr;
		high->lowCount--;
        if (left != nullptr)
			left->right = right;
        if (right != nullptr)
			right->left = left;
        left = nullptr;
        right = nullptr;
		temp = new TreeNode*[high->lowCount];
		TreeNode::arrCopy(temp, high->low, lowCount, index);
		delete[] high->low;
		high->low = temp;
        high = nullptr;
	}
}
void TreeNode::connectLevelLink() {
    if (high != nullptr) {
		if (high->lowCount == 1) {
            left = nullptr;
            right = nullptr;
		}
		else if (index == 0) {
            left = nullptr;
			right = high->low[index + 1];
			right->left = this;
		}
		else if (index == high->lowCount - 1) {
			left = high->low[index - 1];
			left->right = this;
            right = nullptr;
		}
		else {
			left = high->low[index - 1];
			left->right = this;
			right = high->low[index + 1];
			right->left = this;
		}
	}
}
int TreeNode::getDepth() {
	int depth = 0;
	TreeNode* temp;
	temp = this;
    while (temp->high != nullptr) {
		temp = temp->high;
		depth++;
	}
	return depth;
}
TreeNode* TreeNode::getRoot() {
	TreeNode* temp;
	temp = this;
    while (temp->high != nullptr)
		temp = temp->high;
	return temp;
}
void TreeNode::remove() {
	disconnectUpperLink();
	clear();
	delete this;
}
void TreeNode::clear() {
	TreeNode* removeTemp;
	TreeNode** arrayTemp;
	QQueue<TreeNode*> queue;
	for (int i = 0; i < lowCount; i++) {
		queue.enqueue(low[i]);
	}
	do {
		if (queue.isEmpty() == true)
			break;
		removeTemp = queue.dequeue();
		for (int i = 0; i < removeTemp->lowCount; i++) {
			queue.enqueue(removeTemp->low[i]);
		}
        removeTemp->setParent(nullptr);
		delete removeTemp;
	} while (1);
	lowCount = 0;
    low = nullptr;
}
void TreeNode::search(const QString data) {
	QQueue<TreeNode*> queue;
	TreeNode* searchTemp;
	queue.enqueue(this);
	searchedNodeList.clear();
	do {
		if (queue.isEmpty() == true)
			break;
		searchTemp = queue.dequeue();
		for (int i = 0; i < searchTemp->lowCount; i++) {
			queue.enqueue(searchTemp->low[i]);
		}
		if (searchTemp->data == data)
			searchedNodeList.append(searchTemp);
	} while (1);
}
void TreeNode::searchAll(const QString data) {
	TreeNode* rootNode;
	rootNode = getRoot();
	rootNode->search(data);
}


