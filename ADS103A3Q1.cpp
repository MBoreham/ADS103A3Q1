// ADS103 Assignment 3
// Marcus Boreham
// ADS103A3Q1.cpp 

//-------------------------- LIBRARIES ----------------------------------------

#include <iostream>
#include <stack>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//---------------------------- CLASSES -----------------------------------------

// Node class
// Objects of the class contain (int) data and Pointers to child nodes
class Node
{
public:
    int data;
    Node* left;
    Node* right;

    // Constructor
    Node(int value)
    {
        data = value;
        left = NULL;
        right = NULL;
    }

    // Default constructor
    Node()
    {
        data = 0;
        left = NULL;
        right = NULL;
    }
};

// Class for a balanced AVL tree
class AVL
{

public:

    Node* root;

    AVL()
    {
        root = NULL;
    }

    //Returns an integer value of the depth of 
    // a subtree from a defined node
    int height(Node* node)
    {
        if (node == NULL)
            return -1;

        return max(height(node->left), height(node->right)) + 1;
    }

    // Returns the difference in balance
    // between two child nodes
    int balanceFactor(Node* node)
    {
        return height(node->left) - height(node->right);
    }

    // Rotates a left-heavy tree to the right
    Node* R_Rotation(Node* parent)
    {
        Node* temp = parent->left;        
        parent->left = temp->right;
        temp->right = parent;

        return temp;
    }

    // Rotates a right-heavey tree to the left
    Node* L_Rotation(Node* parent)
    {
        Node* temp = parent->right;       
        parent->right = temp->left;
        temp->left = parent;

        return temp;
    }

    // Rotates a left-right heavy tree to the left then the right
    Node* LR_Rotation(Node* parent)
    {
        parent->left = L_Rotation(parent->left);    

        return R_Rotation(parent);                   
    }

    // Rotates a right-left heavy tree to the right then the left
    Node* RL_Rotation(Node* parent)
    {
        parent->right = R_Rotation(parent->right);   

        return L_Rotation(parent);                    
    }

    // Uses the balanceFactor function to decide
    // which rotation function is called
    // Returns the parent node after rotation
    Node* balance(Node* node)
    {
        int bFactor = balanceFactor(node);

        if (bFactor > 1) // Tree is left heavy
        {
            if (balanceFactor(node->left) > 0)
                node = R_Rotation(node);         

            else
                node = LR_Rotation(node);             
        }

        else if (bFactor < -1) // Tree is right heavy
        {
            if (balanceFactor(node->right) > 0)
                node = RL_Rotation(node);             
                              
            else
                node = L_Rotation(node);            
        }
        return node;
    }

    // Creates a new node and stores the passed in value as its data element
    // Balances the nodes after insertion and returns parent node after balancing
    Node* insert(Node* parent, int value)
    {
        // Creates a new node when an 
        // appropriate empty space is found
        if (parent == NULL) 
        {
            Node* temp = new Node;
            temp->data = value;
            parent = temp;
            parent->left = NULL;
            parent->right = NULL;
        }

        // Recursive traversal down left of tree
        // then balance after insertion
        else if (value <= parent->data)
        {
            parent->left = insert(parent->left, value);
            parent = balance(parent);
        }

        // Recursive traversal down right of tree
        // then balance after insertion
        else if (value > parent->data)
        {
            parent->right = insert(parent->right, value);
            parent = balance(parent);
        }
        return parent;
    }

    // Returns the current state of the AVL tree as a string 
    string returnAVL()
    {
        int count = 0;
        string result;

        stack<Node*> stack1;
        stack<Node*> stack2;
        
        Node* printNode = root;

        stack1.push(printNode);

        while (true)
        {   
            // Adds the children of stack1 element to stack2
            // prints the pops all elements of stack1
            result += to_string(count) + ": ";
            while (!stack1.empty())
            {
                printNode = stack1.top();

                if (printNode->left != NULL)
                    stack2.push(printNode->left);

                if (printNode->right != NULL)
                    stack2.push(printNode->right);
           
                // Converts the integer element to string
                result += to_string(printNode->data) + " ";
           
                stack1.pop();
            }
            
            result += "\n";
            count++;
            // Moves the elements of stack2 into stack1
            // pops the elements of stack2
            while (!stack2.empty())
            {
                stack1.push(stack2.top());
                stack2.pop();
            }
            // Exit the function if both stacks are empty
            if (stack1.empty() && stack2.empty())
                return result;
        }
    }

    // Postorder deletion via recursion
    // Taken from - https://www.geeksforgeeks.org/write-a-c-program-to-delete-a-tree/
    void deleteNodes(Node* node)
    {
        if (node == NULL)
            return;

        deleteNodes(node->left);
        deleteNodes(node->right);

        delete node;
    }

};

//---------------------------------------- MAIN --------------------------------------

int main()
{
    AVL avl; // Object of AVL class

    //-------- Variables & Containers----------
    int size;

    string line;
    string numbers;
    string fileOutput;

    vector<string> fileContents;
    vector<int> fileElements;
    //-----------------------------------------

    // Opens a flie and pushes the contents into a vector
    fstream aFile;
    aFile.open("input-q1a2.txt", fstream::in);

    if (aFile.is_open())
    {
        while (getline(aFile, line))
            fileContents.push_back(line);

        aFile.close();
    }
    else
        cout << " Unable to open file";

    // Assign file contents into variables
    size = stoi(fileContents[0]);
    numbers = fileContents[1];
    
    // Takes the string containing the numbers and streams them
    // by word split by spaces (default)
    // Populates the integer vector with the individual numbers
    stringstream ss(numbers);
    string word;
    while (ss >> word)
        fileElements.push_back(stoi(word));
        
    
    // Loop through the integer numbers from the file
    // Insert each number and store the returned string
    // output of each insertion and its changes from balancing
    string temp;
    for (int i = 0; i < 8; i++)
    {
        avl.root = avl.insert(avl.root, fileElements[i]);
        temp = avl.returnAVL();
        
        fileOutput += temp + "\n";
    }

    // Output the finalised AVL tree to file
    ofstream outFile;
    outFile.open("output-q1-a2.txt", ios::out);

    if (outFile.is_open())
    {
        outFile << fileOutput;

        outFile.close();
    }
    else cout << " Unable to open file";
    

    // Delete the nodes of the AVL tree
    avl.deleteNodes(avl.root);
}

