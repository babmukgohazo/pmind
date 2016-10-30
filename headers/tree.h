#ifndef TREE_H
#define TREE_H

#include <QWidget>
#include <QPoint>
#include <QList>
#include <QQueue>
#include <QString>

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
	void search(const QString data);//특정 data를 가지는 노드를 찾음. 단, 이 node를 포함한 하위노드에서만 찾는다.
	void searchAll(const QString data);//연결되어있는 모든 노드에서 찾는다.

	QString data;//string data
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

#endif // TREE_H
