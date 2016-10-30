#ifndef TREE_H
#define TREE_H

#include <QWidget>
#include <QPoint>
#include <QList>
#include <QQueue>

class TreeNode : public QWidget {
public:
	TreeNode();
	TreeNode(const TreeNode& copy);//copy constructor

	TreeNode& operator=(const TreeNode& node);//이미 low가 있는 노드에 대입시 대입되는 노드의 low가 delete된다
	TreeNode& operator+(TreeNode& node);//두 노드를 하위 노드로 가지는 새로운 노드를 반환
	TreeNode& operator+=(TreeNode& node);//우측에 쓰인 노드가 좌측에 쓰인 노드의 subTree가 된다

	static void arrCopy(TreeNode** to, TreeNode** from, int toSize, int index);//treeNode 배열을 복사함
	TreeNode* addLowNode();//low node를 한 개 만들면서 주소를 반환.
	void addLowNode(TreeNode* lowNode);//주어진 node를 currentNode의 하위노드로 연결
	void addLowNode(int num);//특정 개수만큼 low node를 생성
	void disconnectUpperLink();//상위, level노드와의 연결을 끊음
	int getDepth();//top의 depth는 0, Node의 깊이를 반환
	TreeNode* getRoot();//Root node (top node)를 반환
	void remove();//노드와 그의 하위노드들까지 모두 제거
	void clear();//노드의 하위 노드들만 제거. remove는 노드까지 포함하는 반면에 clear는 하위노드만 제거한다.
	void search(const int data);//특정 data를 가지는 노드를 찾음. 단, 이 node를 포함한 하위노드에서만 찾는다.
	void searchAll(const int data);//연결되어있는 모든 노드에서 찾는다.

	int data;//test를 위해 사용
	int lowCount;//low배열의 크기
	int index;//같은 level의 노드 중 몇 번째에 위치하는지 기록
	TreeNode** low;//하위 node배열을 지목
	TreeNode* high;//상위 node를 지목
	TreeNode* right;//같은 level의 노드 중 우측노드 지목
	TreeNode* left;//같은 level의 노드 중 좌측노드 지목
	QList<TreeNode*> searchedNodeList;	//search에서 찾은 노드들을 저장할 리스트

private:
	void connectLevelLink();//상위노드와 연결된 상태에서 level노드와 연결
};

TreeNode::TreeNode() :QWidget() {
	lowCount = 0;
	index = 0;
	low = '\0';
	high = '\0';
	right = '\0';
	left = '\0';
	data = 0;
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
	if (this->low != '\0')
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
	if (to == '\0' || from == '\0');	//배열 포인터가 NULL인 상태에서 접근하면 안된다.
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
	if (addTemp != '\0') {
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
	if (addTemp != '\0') {
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
	if (addTemp != '\0') {
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
	if (high != '\0') {
		high->low[index] = '\0';
		high->lowCount--;
		if (left != '\0')
			left->right = right;
		if (right != '\0')
			right->left = left;
		left = '\0';
		right = '\0';
		temp = new TreeNode*[high->lowCount];
		TreeNode::arrCopy(temp, high->low, lowCount, index);
		delete[] high->low;
		high->low = temp;
		high = '\0';
	}
}
void TreeNode::connectLevelLink() {
	if (high != '\0') {
		if (high->lowCount == 1) {
			left = '\0';
			right = '\0';
		}
		else if (index == 0) {
			left = '\0';
			right = high->low[index + 1];
			right->left = this;
		}
		else if (index == high->lowCount - 1) {
			left = high->low[index - 1];
			left->right = this;
			right = '\0';
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
	while (temp->high != '\0') {
		temp = temp->high;
		depth++;
	}
	return depth;
}
TreeNode* TreeNode::getRoot() {
	TreeNode* temp;
	temp = this;
	while (temp->high != '\0')
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
		removeTemp->setParent('\0');
		delete removeTemp;
	} while (1);
	lowCount = 0;
	low = '\0';
}
void TreeNode::search(const int data) {
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
void TreeNode::searchAll(const int data) {
	TreeNode* rootNode;
	rootNode = getRoot();
	rootNode->search(data);
}

#endif // TREE_H
