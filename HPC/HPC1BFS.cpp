#include <iostream>
#include <queue>
#include <omp.h>
using namespace std;

class Node
{
public:
    Node *right;
    Node *left;
    int data;

    Node(int data) : data(data), left(nullptr), right(nullptr) {}
};

class BFS
{
private:
    Node *root;

public:
    BFS() : root(nullptr) {}
    void createTree(); // Function to manually create the tree structure
    void bfs();
};

void BFS::createTree()
{
    int rootData;
    cout << "Enter the root value: ";
    cin >> rootData;
    root = new Node(rootData);

    queue<Node *> q;
    q.push(root);

    while (!q.empty())
    {
        Node *currNode = q.front();
        q.pop();

        int leftData, rightData;
        cout << "Enter left child value of " << currNode->data << " (-1 if none): ";
#pragma omp critical
        cin >> leftData;

        if (leftData != -1)
        {
            Node *leftNode = new Node(leftData);
            currNode->left = leftNode;
            q.push(leftNode);
        }

        cout << "Enter right child value of " << currNode->data << " (-1 if none): ";
#pragma omp critical
        cin >> rightData;

        if (rightData != -1)
        {
            Node *rightNode = new Node(rightData);
            currNode->right = rightNode;
            q.push(rightNode);
        }
    }
}

void BFS::bfs()
{
    if (!root)
    {
        cout << "Tree is empty\n";
        return;
    }

    queue<Node *> q;
    q.push(root);

    while (!q.empty())
    {
        int qSize = q.size();

#pragma omp parallel for
        for (int i = 0; i < qSize; i++)
        {
            Node *currNode;
#pragma omp critical
            {
                currNode = q.front();
                q.pop();
            }

            cout << currNode->data << "\t";

            if (currNode->left)
#pragma omp critical
                q.push(currNode->left);

            if (currNode->right)
#pragma omp critical
                q.push(currNode->right);
        }
    }
}

int main()
{
    BFS tree;
    tree.createTree();
    tree.bfs();
    return 0;
}

/*
Run Commands:
1) g++ -fopenmp BFS.cpp –o bfs
2) ./bfs
*/
// Output
// Enter the root value: 1
// Enter left child value of 1 (-1 if none): 2
// Enter right child value of 1 (-1 if none): 3
// Enter left child value of 2 (-1 if none): 4
// Enter right child value of 2 (-1 if none): 5
// Enter left child value of 3 (-1 if none): 6
// Enter right child value of 3 (-1 if none): 7
// Enter left child value of 4 (-1 if none): -1
// Enter right child value of 4 (-1 if none): -1
// Enter left child value of 5 (-1 if none): -1
// Enter right child value of 5 (-1 if none): -1
// Enter left child value of 6 (-1 if none): -1
// Enter right child value of 6 (-1 if none): -1
// Enter left child value of 7 (-1 if none): -1
// Enter right child value of 7 (-1 if none): -1
// 1       2       3       4       5       6       7