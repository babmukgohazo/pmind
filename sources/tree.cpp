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
TreeNode::TreeNode(const TreeNode& copy) : QWidget() {//QWidget�� TreeNode�� ���� convert constructor �ʿ�
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

void TreeNode::arrCopy(TreeNode** to, TreeNode** from, int toSize, int index) {	//�迭�� �ܼ� ����� ���� ���縦 ������. index�� -1�̸� �ܼ�����, �ƴϸ� index�� �ش��ϴ� ���� ���� ����
	int i, j;												//index�� from���� ����� �����̴�.
    if (to == nullptr || from == nullptr);	//�迭 �����Ͱ� NULL�� ���¿��� �����ϸ� �ȵȴ�.
										//����ó��
	if (index == -1) {
		for (i = 0;i < toSize - 1;i++)	//���� to���� from�� ���� �Ѱ��� ����. ������ -1����.
			to[i] = from[i];
	}
	else {	//index�� �ش��ϴ� ���Ҹ� �ϳ��� �� ����.
		for (i = 0, j = 0;i < toSize;i++) {	//���� to���� from�� ���� �Ѱ��� �� ����. �׸��� from���� �� ���� ���Ҹ� ���ְ� to�� �ִ´�. ���� ���� ���� ���� ����. - ȣ���ϴ� �κ��� �ڵ带 ���� ���ؿ� ������ ��
			if (index != i) {
				to[j] = from[i];	//from���� index �ϳ� ���� to�� ����.
				to[j]->index = j;	//index�� �Ұŵ� ���� �ڷ� 1�� �и��Ƿ�, �缳��.
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
	low = new TreeNode*[lowCount];	//�� ĭ �� ū �迭 ����
    if (addTemp != nullptr) {
		TreeNode::arrCopy(low, addTemp, lowCount, -1);
		delete[] addTemp;
	}
	low[lowCount - 1] = addNode;	//�Ʒ��� ����
	addNode->high = this;	//���ε� ����
	addNode->index = lowCount - 1;	//index�� ����
	addNode->connectLevelLink();
	addNode->setParent(this);
	return addNode;
}
void TreeNode::addLowNode(TreeNode* addNode) {		//Ʈ���� Ư�� ���(currentNode)�� �־��� ������� �߰�
	TreeNode** addTemp;
	lowCount++;
	addTemp = low;
	low = new TreeNode*[lowCount];	//�� ĭ �� ū �迭 ����
    if (addTemp != nullptr) {
		TreeNode::arrCopy(low, addTemp, lowCount, -1);
		delete[] addTemp;
	}
	low[lowCount - 1] = addNode;	//�Ʒ��� ����
	addNode->high = this;	//���ε� ����
	addNode->index = lowCount - 1;	//index�� ����
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


