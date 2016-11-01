#ifndef TREE_H
#define TREE_H

#include <QWidget>
#include <QList>
#include <QQueue>
#include <QString>

class TreeNode : public QWidget {
public:
    TreeNode();
    TreeNode(const TreeNode& copy);//copy constructor
    TreeNode& operator=(const TreeNode& node);//�̹� low�� �ִ� ��忡 ���Խ� ���ԵǴ� ����� low�� delete�ȴ�
    TreeNode& operator+(TreeNode& node);//�� ��带 ���� ���� ������ ���ο� ��带 ��ȯ
    TreeNode& operator+=(TreeNode& node);//������ ���� ��尡 ������ ���� ����� subTree�� �ȴ�

    TreeNode* addLowNode();//low node�� �� �� ����鼭 �ּҸ� ��ȯ.
    void addLowNode(TreeNode* lowNode);//�־��� node�� currentNode�� �������� ����
    void addLowNode(int num);//Ư�� ������ŭ low node�� ����
    void disconnectUpperLink();//����, level������ ������ ����
    int getDepth();//top�� depth�� 0, Node�� ���̸� ��ȯ
    TreeNode* getRoot();//Root node (top node)�� ��ȯ
    void clear();//����� ���� ���鸸 ����. remove�� ������ �����ϴ� �ݸ鿡 clear�� ������常 �����Ѵ�.
    void search(const int data);//Ư�� data�� ������ ��带 ã��. ��, �� node�� ������ ������忡���� ã�´�.

    int data;//string data
    int lowCount;//low�迭�� ũ��
    int index;//���� level�� ��� �� �� ��°�� ��ġ�ϴ��� ���
    TreeNode** low;//���� node�迭�� ����
    TreeNode* high;//���� node�� ����
    QList<TreeNode*> searchedNodeList;	//search���� ã�� ������ ������ ����Ʈ

private:
    void arrCopy(TreeNode** to, TreeNode** from, int toSize, int index);//treeNode �迭�� ������
};

#endif // TREE_H
