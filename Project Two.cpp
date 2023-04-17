//============================================================================
// Name        : Project Two.cpp
// Author      : Afahri Kerr
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Program designed to load a data file from a csv file,
//               print a course list in alphanumeric order, and print a 
//               specific course and any prerequisites
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;

//Displays the user menu of options to choose from
void displayMenu() {
    cout << "1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "9. Exit\n" << endl;
    cout << "What would you like to do? ";
}

// define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string courseTitle;
    vector<string> preRequisite;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Node* newNode);
    void inOrder(Node* node);
    void destruct(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    Node* GetRoot();
    void InOrder();
    void Insert(Node* node);
    void Search(string courseId);
    void loadCourse(string filename);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    destruct(root);
}

//destructs left and right pointer nodes and deletes them
void BinarySearchTree::destruct(Node* node) {
    if (node != nullptr) {
        destruct(node->left);
        node->left = nullptr;
        destruct(node->right);
        node->right = nullptr;
        delete node;
    }
};

//get root of tree
Node* BinarySearchTree::GetRoot() {
    Node* treeRoot = this->root;

    return treeRoot;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    this->inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Node* node) {
    // if root equal to null ptr
    if (root == nullptr) {
        // root is equal to new node course
        root = node;
    }
    // else
    else {
        // add Node root and course
        this->addNode(root, node);
    }
}

/**
 * Search for a course
 */
void BinarySearchTree::Search(string courseId) {
    // set current node equal to root
    Node* currNode = root;

    // keep looping downwards until bottom reached or matching courseId found
    while (currNode != nullptr) {
        // if match found, print course information
        if (currNode->course.courseId.compare(courseId) == 0) {
            cout << currNode->course.courseId << ", " << currNode->course.courseTitle << endl;
            cout << "Prerequisites: ";
            //prints all prerequisite courseId's for the course
            for (string preReq : currNode->course.preRequisite) {
                if (preReq == currNode->course.preRequisite.back()) {
                    cout << preReq << endl;
                }
                else {
                    cout << preReq << ", ";
                }
            }

            cout << endl;
            return;
        }

        // if course is smaller than current node then traverse left
        if (currNode->course.courseId.compare(courseId) > 0) {
            currNode = currNode->left;
        }
        // else larger so traverse right
        else {
            currNode = currNode->right;
        }
    }

    //if course not found
    cout << courseId << " not found." << endl;
    return;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course course to be added
 */
void BinarySearchTree::addNode(Node* node, Node* newNode) {
    // if node is larger then add to left
    if (node->course.courseId.compare(newNode->course.courseId) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = newNode;
        }
        // else recurse down the left node
        else {
            this->addNode(node->left, newNode);
        }
    }

    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = newNode;
        }
        //else
        else {
            // recurse down the left node
            this->addNode(node->right, newNode);
        }
    }
}

//Print courses in alphanumeric order
void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {

        //InOrder node left
        inOrder(node->left);

        //output courseID, title
        cout << node->course.courseId << ", " << node->course.courseTitle << endl;

        //InOrder right
        inOrder(node->right);
    }
}

//loads file and creates course list
void loadCourse(string filename, BinarySearchTree* bst) {
    ifstream file(filename);
    if (file.is_open()) {
        cout << "File loaded.\n" << endl;

        int num;
        string line;
        string word;

        while (getline(file, line)) {

            num = 0;
            Node* node = new Node();
            stringstream str(line);

            while (num < 2) {
                getline(str, word, ',');
                if (num == 0) {
                    node->course.courseId = word;
                }
                else {
                    node->course.courseTitle = word;
                }
                num++;
            }
            while (getline(str, word, ',')) {
                node->course.preRequisite.push_back(word);
            }

            //inserts node into bst
            bst->Insert(node);
        }
    }
    //file error if file not found
    else {
        cout << "File error, please try again.\n" << endl;
        return;
    }
}


int main() {
    BinarySearchTree* bst = new BinarySearchTree();
    string fileChoice;
    string courseChoice;
    int userChoice = 0;

    cout << "Welcome to the course planner.\n" << endl;

    while (userChoice != 9) {
        displayMenu();
        cin >> userChoice;

        //Check that user has input an integer
        if (!cin) {
            cin.clear();
            cin.ignore(1000, '\n');
        }

        switch (userChoice) {

            //Load file that user enters
        case 1:
            cout << "Please enter name of flie to load: ";
            cin >> fileChoice;

            loadCourse(fileChoice, bst);
            break;

            //Prints courses in order
        case 2:
            bst->InOrder();
            cout << endl;
            break;

            //Prints course information for course that user enters
        case 3:
            cout << "Please enter course to search for: ";
            cin >> courseChoice;
            cout << endl;

            transform(courseChoice.begin(), courseChoice.end(), courseChoice.begin(), ::toupper);

            bst->Search(courseChoice);
            break;

            //Exits the program
        case 9:
            cout << "Thank you for using the course planner!";
            break;

        default:
            cout << "\nThat is not a valid option!\n" << endl;
            break;
        }
    }

    return 0;
}

