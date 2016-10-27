#ifndef TREE_H
#define TREE_H

#include <QWidget>
#include <QPoint>
#include <QList>
#include <QQueue>
using namespace std;

class TreeNode : public QWidget {
public:
	TreeNode();
	TreeNode(const TreeNode& copy);//copy constructor

	TreeNode& operator=(const TreeNode& node);//�̹� low�� �ִ� ��忡 ���Խ� ���ԵǴ� ����� low�� delete�ȴ�
	TreeNode& operator+(TreeNode& node);//�� ��带 ���� ���� ������ ���ο� ��带 ��ȯ
	TreeNode& operator+=(TreeNode& node);//������ ���� ��尡 ������ ���� ����� subTree�� �ȴ�

	static void arrCopy(TreeNode** to, TreeNode** from, int toSize, int index);//treeNode �迭�� ������
	TreeNode* addLowNode();//low node�� �� �� ����鼭 �ּҸ� ��ȯ.
	void addLowNode(TreeNode* lowNode);//�־��� node�� currentNode�� �������� ����
	void addLowNode(int num);//Ư�� ������ŭ low node�� ����
	void disconnectUpperLink();//����, level������ ������ ����
	int getDepth();//top�� depth�� 0, Node�� ���̸� ��ȯ
	void remove();//this�� ������ sub tree�� ����
	void clear();//this�� �����ϰ� sub tree�� ����

	int data;//test�� ���� ���
	int lowCount;//low�迭�� ũ��
	int index;//���� level�� ��� �� �� ��°�� ��ġ�ϴ��� ���
	TreeNode** low;//���� node�迭�� ����
	TreeNode* high;//���� node�� ����
	TreeNode* right;//���� level�� ��� �� ������� ����
	TreeNode* left;//���� level�� ��� �� ������� ����

protected:
	void connectLevelLink();//�������� ����� ���¿��� level���� ����
};

class Tree : public TreeNode {
public:

	TreeNode& operator=(const TreeNode& node);
	TreeNode& operator+=(TreeNode& node);

	void search(const int data);//Ư�� data�� ������ ��带 ã��
	static int getDepth(TreeNode* node);//�־��� node�� ���̸� ��ȯ
	int getCount();//tree�� �޸� ����� ������ ��ȯ

	QList<TreeNode*> searchedNodeList;	//search���� ã�� ������ ������ ����Ʈ

private:
	using TreeNode::remove;
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
TreeNode::TreeNode(const TreeNode& copy) : QWidget() {//QWidget�� TreeNode�� ���� convert constructor �ʿ�
	data = copy.data;
	lowCount = copy.lowCount;
	index = copy.index;
	low = new TreeNode*[copy.lowCount];
	for (int i = 0;i < copy.lowCount;i++)
		low[i] = copy.low[i];
	high = copy.high;
	left = copy.left;
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

void TreeNode::arrCopy(TreeNode** to, TreeNode** from, int toSize, int index) {	//�迭�� �ܼ� ����� ���� ���縦 ������. index�� -1�̸� �ܼ�����, �ƴϸ� index�� �ش��ϴ� ���� ���� ����
	int i, j;												//index�� from���� ����� �����̴�.
	if (to == '\0' || from == '\0');	//�迭 �����Ͱ� NULL�� ���¿��� �����ϸ� �ȵȴ�.
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
	if (addTemp != '\0') {
		TreeNode::arrCopy(low, addTemp, lowCount, -1);
		delete[] addTemp;
	}
	low[lowCount - 1] = addNode;	//�Ʒ��� ����
	addNode->high = this;	//���ε� ����
	addNode->index = lowCount - 1;	//index�� ����
	addNode->connectLevelLink();
	return addNode;
}
void TreeNode::addLowNode(TreeNode* addNode) {		//Ʈ���� Ư�� ���(currentNode)�� �־��� ������� �߰�
	TreeNode** addTemp;
	lowCount++;
	addTemp = low;
	low = new TreeNode*[lowCount];	//�� ĭ �� ū �迭 ����
	if (addTemp != '\0') {
		TreeNode::arrCopy(low, addTemp, lowCount, -1);
		delete[] addTemp;
	}
	low[lowCount - 1] = addNode;	//�Ʒ��� ����
	addNode->high = this;	//���ε� ����
	if (lowCount >= 2) {		//�ι�° ������ ��尣 ������ �ʿ�.
		addNode->left = low[lowCount - 2];
		low[lowCount - 2]->right = addNode;	//���� ��� ���� �� ������ ����
	}
	addNode->index = lowCount - 1;	//index�� ����
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
		delete removeTemp;
	} while (1);
	lowCount = 0;
	low = '\0';
}

TreeNode& Tree::operator=(const TreeNode& node) {
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
TreeNode& Tree::operator+=(TreeNode& node) {
	if (&node == this)
		return *this;
	node.disconnectUpperLink();
	addLowNode(&node);
	return *this;
}

void Tree::search(const int data) {		//Ʈ���� ť�� Ǯ�鼭 Ž��. BFS����. ȸ�ΰ� �����Ƿ� BFS ������ ����ϴ�.
	QQueue<TreeNode*> queue;
	TreeNode* searchTemp;
	queue.enqueue(this);		//top���� �����Ѵ�.
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
int Tree::getDepth(TreeNode* node) {
	int depth = 0;
	TreeNode* temp;
	temp = node;
	while (temp->high != '\0') {
		temp = temp->high;
		depth++;
	}
	return depth;
}
int Tree::getCount() {
	QQueue<TreeNode*> queue;
	TreeNode* temp;
	int count = 0;
	queue.enqueue(this);		//top���� �����Ѵ�.
	do {
		if (queue.isEmpty() == true)
			break;
		temp = queue.dequeue();
		count++;
		for (int i = 0; i < temp->lowCount; i++) {
			queue.enqueue(temp->low[i]);
		}
	} while (1);
	return count;
}

#endif // TREE_H
