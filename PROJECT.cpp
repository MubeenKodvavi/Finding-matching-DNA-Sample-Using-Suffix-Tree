#include<iostream>
#include<unordered_map>
#include<string>
#include<fstream>
#include<bits/stdc++.h>

using namespace std;

class SNode {
public:
    static int NodeCount;
    int suffix;
    
    SNode () : 
        suffix(-1) {};
};

class Edge {
public:
    int NodeLow;
    int NodeHigh;
    int sindexl;
    int eindexl;
    void insert ();
    void deletee ();
    static int getHKey(int node, int c);
    static Edge searchedge(int node, int c);
    
    Edge () : NodeLow(-1) {};
    Edge (int begin, int first, int last) :
        NodeLow (begin),
        NodeHigh (SNode::NodeCount++),
        sindexl (first),
        eindexl (last) {};    

};
int Edge::getHKey(int nodeID, int c) {
    return (nodeID + (c << 59));
}

class suffixTree {
    public:
        int root;
        int beginpos;
        int endIndex;

        suffixTree() :
            root(0), 
            beginpos(-1),
            endIndex(-1){};
        suffixTree(int root, int begin, int end) :
            root(root),
            beginpos(begin),
        endIndex(end) {};
        bool endNode() {return beginpos > endIndex;} 
        bool endEdge() {return endIndex >= beginpos;} 
        void movetonearestparent();
};

string DNA;
int DNALength;

int SNode::NodeCount = 1;

SNode * DSA;
struct Key {
    int nodeID;
    int asciiChar;
};
unordered_map <int, Edge> edgeHash;

void Edge::insert() {
    int key = getHKey(NodeLow, DNA[sindexl]);
    edgeHash[key] = *this;
}

void Edge::deletee() {
     int key = getHKey(NodeLow, DNA[sindexl]);
     edgeHash.erase(key);
}

Edge Edge::searchedge(int node, int asciiChar) {
    int key = getHKey(node, asciiChar);
    unordered_map<int, Edge>::const_iterator search = edgeHash.find(key);
    if (search != edgeHash.end()) {
        return edgeHash.at(key);
    }
    return Edge();
}
void suffixTree::movetonearestparent() {
    if (endNode()) {
    }
    else {
        Edge e = Edge::searchedge(root, DNA[beginpos]);
        if(e.NodeLow == -1) {
            cout <<  root << " " << beginpos << " " << DNA[beginpos] << endl;
        }
        int labelLength = e.eindexl - e.sindexl;

        while (labelLength <= (endIndex - beginpos)) {
            beginpos += labelLength + 1;
            root = e.NodeHigh;
            if (beginpos <= endIndex) {
                e = Edge::searchedge(e.NodeHigh, DNA[beginpos]);
          if(e.NodeLow == -1) {
            cout <<  root << " " << beginpos << " " << DNA[beginpos] << endl;
        }
                labelLength = e.eindexl - e.sindexl;
            }
        }
        
    }
}

int breakEdge(suffixTree &s, Edge &e) {
    e.deletee();

    Edge *newEdge = new Edge(s.root, e.sindexl, e.sindexl + s.endIndex - s.beginpos);
    newEdge -> insert();
    DSA[newEdge -> NodeHigh].suffix = s.root;
    e.sindexl += s.endIndex - s.beginpos + 1;
    e.NodeLow = newEdge -> NodeHigh;
    e.insert();
    return newEdge->NodeHigh;
}

void Buildtree(suffixTree &tree, int previ) {
    int parentNode;
    int previousParentNode = -1;
    while (true) {
        Edge e;
        parentNode = tree.root;

        if (tree.endNode() ) {
            e = Edge::searchedge(tree.root, DNA[previ]);
            if (e.NodeLow != -1)
                break;
        }
        else {
            e = Edge::searchedge(tree.root, DNA[tree.beginpos]);
            int diff = tree.endIndex - tree.beginpos;
            if (DNA[e.sindexl + diff + 1] == DNA[previ])
                break; 
            parentNode = breakEdge(tree, e);
        }
        Edge *newEdge = new Edge(parentNode, previ, DNALength);
        newEdge -> insert();
        if (previousParentNode > 0)
            DSA[previousParentNode].suffix = parentNode;
        previousParentNode = parentNode;
        if (tree.root == 0)
            tree.beginpos++;
        else {
            tree.root = DSA[tree.root].suffix;
        }
        tree.movetonearestparent();
    }

    if (previousParentNode > 0)
        DSA[previousParentNode].suffix = parentNode;
    tree.endIndex++;
    tree.movetonearestparent();
}

bool search(string pattern) {
    int len = pattern.length();
    Edge e = Edge::searchedge(0, pattern[0]);
    int j = 0;
    int i = -1;
    if (e.NodeLow != -1) {
        while(i < len) {
            j = 0;
            while (e.eindexl >= e.sindexl + j)   
                    {
                        if (DNA[e.sindexl + j] == pattern[i + j + 1]) { 
                            j++;
                            if (i + j  + 1 >= len) {
                                cout << "DISEASE EXISTS IN THIS PERSON" << endl;
                                return true;
                            }
                        }
                        else {
                            cout << "DISEASE DOES NOT EXIST IN THIS PERSON." << endl;
                            return false;     
                        }
                   }
            e = Edge::searchedge(e.NodeHigh, pattern[i + j + 1]);
            if (e.NodeLow == -1) {
                cout << "DISEASE DOES NOT EXIST IN THIS PERSON" << endl;
                return false;    
            }
            i+=j;
        }
    }
    return true;
}


int main () {
  cout << "Enter String" << endl;
  ifstream in, init;
  in.open("person2.txt");
  init.open("disease4.txt");
  getline(in, DNA);
  DNALength = DNA.length() - 1;
  DSA = (SNode *)malloc(2*DNALength*(sizeof (SNode)));
  suffixTree tree (0, 0, -1);
  for (int i = 0; i <= DNALength; i++)
      Buildtree(tree, i);
  cout << "Enter patterns or enter \"exit\" to exit." << endl;
  string pattern;
  getline(init, pattern);
  search(pattern);
  return 0;
}
