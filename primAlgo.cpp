#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;

/*
Name: Christopher Bingham
Email: cbingha2@binghamton.edu
Assignment: Week 5 - Prim's Algorithm
*/


//INFINITY Global Constant
const int INFIN = 999999;

int parentNode(int index) {
    /*
    Returns the index of the parent node of the node at the given index.
    Parameters:
       - index <int>: The index of the node for which to find the parent.
    Returns:
        - <int>: The index of the parent node.
    */
    return (index - 1) / 2;
}

int leftChildNode(int index) {
    /*
    Returns the index of the left child node of the node at the given index.
    Parameters:
       - index <int>: The index of the node for which to find the left child.
    Returns:
        - <int>: The index of the left child node.
    */
    return (2 * index) + 1;
}

int rightChildNode(int index) {
    /*
    Returns the index of the right child node of the node at the given index.
    Parameters:
       - index <int>: The index of the node for which to find the right child.
    Returns:
        - <int>: The index of the right child node.
    */
    return (2 * index) + 2;
}

class Edge {
    /*
    The edge of a given graph that connects vertices together
    Parameters:
        vertex <int>: Vertex the edge leads to
        length <int>: Distance costt
        vector<Edge> next: Edges that it can branch to next
    */
    public:
        int vertex;
        int length;
        vector<Edge> next;
};

class Vertex {
    /*
    The vertex of a given graph
    Parameters:
        distance <int>: Cost to add this vertex
        vector<Edge> edges: Other edges that this vertex connects to
    */
    public:
        int distance;
        vector<Edge> edges;

};

class HeapElement {
    /*
    Creates the heap element for us to either insert or get when we remove from the heap
    Parameters:
        vertexNumber <int>: The vertex number
        distanceCost <int>: Keep track of the cost to add to this vertex
    */
    public:
        int vertexNumber;
        int distanceCost;
};

class Heap {
    /*
    Create the min Heap design used for us to get the minimum edges between our vertices
    Parameters:
        vector<HeapElement> minHeap: The min heap used to keep track of the vertices
        vector<int> index: Used to keep track of the vertices index
        heapSize <int>: The size of the heap
    */
    public:
        vector<HeapElement> minHeap;
        vector<int> index;
        int heapSize;
};

void heapSwap (Heap &heap, int index1, int index2) {
    /*
    Swap the two indices within the heap
    Parameters:
        heap <Heap>: The reference to the heap in which to make the changes
        index1 <int>: Index of first value to swap
        index2 <int>: Index of second value to swap 
    Returns:
        None
    */
    HeapElement temp;
    temp = heap.minHeap[index1];
    heap.minHeap[index1] = heap.minHeap[index2];
    heap.minHeap[index2] = temp;

    heap.index[heap.minHeap[index1].vertexNumber] = index1;
    heap.index[heap.minHeap[index2].vertexNumber] = index2;

}

void heapPercolateUp (Heap &heap, int index) {
    /*
    Move the index of the vertex higher depending if distance cost is less than its parent
    Parameters:
        heap <Heap>: Heap that will be modified as percolate occurs
        index <int>: Index of the vertex to check for percolate possibility 
    Returns:
        None
    */
    while((index > 0) && (heap.minHeap[parentNode(index)].distanceCost > heap.minHeap[index].distanceCost)) {
        heapSwap(heap, index, parentNode(index));
        index = parentNode(index);
    }
}

void heapPercolateDown (Heap &heap, int index) {
    /*
    Move the index of the vertex lower depending if the distance is higher than its children nodes
    Parameters:
        heap <Heap>: Heap that will be modified as percolate occurs
        index <int>: Index of the vertex to check for percolate possibility 
    Returns:
        None
    */
    int small;
    while(index < heap.heapSize) {
        small = index;
        if((leftChildNode(index) < heap.heapSize) && (heap.minHeap[leftChildNode(index)].distanceCost < heap.minHeap[small].distanceCost)) {
            small = leftChildNode(index);
        }
        if((rightChildNode(index) < heap.heapSize) && (heap.minHeap[rightChildNode(index)].distanceCost < heap.minHeap[small].distanceCost)) {
            small = rightChildNode(index);
        }
        if (index == small) {
            return;
        }

        heapSwap(heap, index, small);
        index = small;
    }
}

int heapPop (Heap &heap, HeapElement &element) {
    /*
    Remove an element from the heap
    Parameters:
        heap <Heap>: Heap that will be modified to remove the element
        element <HeapElement>: Used to keep track of the element taken out of the heap
    Returns:
        1 (Heap is not empty)
        0 (Heap is empty)
    */
    int index;
    int small;
    HeapElement temp;

    if (heap.heapSize == 0){
        return 0;
    }

    element = heap.minHeap[0];
    heap.minHeap[0] = heap.minHeap[heap.heapSize - 1];
    heap.index[heap.minHeap[0].vertexNumber] = 0;
    heap.heapSize--;

    heapPercolateDown(heap, 0);

    return 1;
}

void heapInsert (Heap &heap, int vertex, int distance) {
    /*
    Insert an element into the heap
    Parameters:
        heap <Heap>: Heap that the element is inserted into
        vertex <int>: Vertex of the heap element
        distance <int>: Cost for the heap element
    Returns:
        None
    */
    int index;
    HeapElement temp;

    heap.minHeap[heap.heapSize].vertexNumber = vertex;
    heap.minHeap[heap.heapSize].distanceCost = distance;
    heap.index[vertex] = heap.heapSize;
    index = heap.heapSize;
    heap.heapSize++;

    heapPercolateUp(heap, index);

}

void heapDecreaseKey (Heap &heap, int vertex, int distance) {
    /*
    Decrease the key whenever the distance changes
    Parameters:
        heap <Heap>: Heap that it modified for the vertex change
        vertex <int> : Vertex number
        distance <int> : Edge cost
    Return:
        None
    */
    int vertexIndex = heap.index[vertex];

    if (heap.minHeap[vertexIndex].vertexNumber != vertex) {
        exit(1);
    }
    heap.minHeap[vertexIndex].distanceCost = distance;
    heapPercolateUp(heap, vertexIndex);
}

void heapShow (Heap &heap) {
    /*
    Display the heap show proper changes in values and heap removal
    Parameters:
        heap <Heap>: the heap used to display the information
    Returns:
        None
    */
    cout << "Heap : " << heap.heapSize << " elements\n";
    for (int i = 0; i < heap.heapSize; ++i) {
        cout << "[v " << heap.minHeap[i].vertexNumber << " dist " << heap.minHeap[i].distanceCost << " (idx " << heap.index[heap.minHeap[i].vertexNumber] << " i " << i << ")]";
    }
    cout << "\n";

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

int readFile(ifstream &inputFile, int argc, char *argv[]) {
    /*
    Reads the input file and builds the array to be sorted.
    Parameters:
       - inputFile <ifstream>: The input file stream from which to read the integers.
       - argc <int>: The number of command line arguments provided.
    Returns:
        - 0: Successful execution
        - 1: Error in opening the input file
    */
    if (argc != 2) {
        string fileName;
        cout << "Enter File Name:" << endl;
        cin >> fileName;
        inputFile.open(fileName);
        if (!inputFile) {
            cout << "Cannot Open File: " << fileName << endl;
            cout << "CommandLine Usage: ./primAlgo <input_file>" << endl;
            return 1;
        }
    } else {
        inputFile.open(argv[1]);
        if (!inputFile) {
            cout << "Cannot Open File: " << argv[1] << endl;
            cout << "CommandLine Usage: ./primAlgo <input_file>" << endl;
            return 1;
        }
    }
    return 0;
}


int main(int argc, char *argv[]) {
    /*
    Main function to execute the prim algorithm.
    CommandLine Usage:
        - ./primAlgo <input_file>
    Command line arguments:
        - argv[1]: The name of the input file containing the integers to be sorted.
        - If no command line argument is provided, the user will be prompted to enter the file name.
    Returns:
        - 0: Successful execution
        - 1: Error in opening the input file
    */

    //Read the input file and build the array to be sorted
    ifstream inputFile;

    if (readFile(inputFile, argc, argv)) {
        return 1;
    }
    // cout << "STEP 1 COMPLETE";

    //Setting up values
    vector<Vertex> v;
    vector<Edge> e;
    int numV;
    int numE;
    Heap heap;
    int i;
    int eI;
    vector<int> s;
    int v1;
    int v2;
    int length;
    HeapElement element;
    // vector<Edge> outEdge;
    int outEdge;
    int max;
    inputFile >> numV;
    inputFile >> numE;

    //Initialize some of the data
    v.resize(numV);
    e.resize(2*numE);

    heap.heapSize = 0;
    heap.minHeap.resize(numV);
    heap.index.resize(numV);

    //Add all of the vertices
    for (int i = 0; i < numV; i++) {
        // cout << "STUCK";
        v[i].distance = INFIN;
        v[i].edges.resize(0);
        heapInsert(heap, i, INFIN);
    }

    // Add all of the edges
    eI = 0;

    for(i = 0; i < numE; ++i) {
        inputFile >> v1;
        inputFile >> v2;
        inputFile >> length;
        // cout << "STEP 2.1 COMPLETE";
        e[eI].length = length;
        e[eI].vertex = v2;
        e[eI].next.insert(e[eI].next.end(), v[v1].edges.begin(), v[v1].edges.end());
        v[v1].edges.push_back(e[eI]);
        ++eI;
        // cout << "STEP 2.2 COMPLETE";
        e[eI].length = length;
        e[eI].vertex = v1;
        e[eI].next.insert(e[eI].next.end(), v[v2].edges.begin(), v[v2].edges.end());
        v[v2].edges.push_back(e[eI]);
        ++eI;
    }

    // heapShow(heap);
    // cout << "STEP 2 COMPLETE";

    // Get the total cost and get the MST the builds that min cost
    v[0].distance = 0;
    heapDecreaseKey(heap, 0, 0);

    vector<int> parent(numV, -1);
    vector<bool> inMST(numV, false);

    int totalCost = 0;

    // heapShow(heap);

    while (heapPop(heap, element)) {
        totalCost += element.distanceCost;

        // v[element.vertexNumber].distance = 0;

        v1 = element.vertexNumber;

        inMST[v1] = true;

        for(outEdge = 0; outEdge != v[v1].edges.size(); outEdge++) {
            v2 = v[v1].edges[outEdge].vertex;

            if (!inMST[v2] && v[v1].edges[outEdge].length < v[v2].distance) {

                parent[v2] = v1;


                v[v2].distance = v[v1].edges[outEdge].length;
                heapDecreaseKey(heap, v2, v[v1].edges[outEdge].length);
            }
        }
        // heapShow(heap);
    }
    // cout << "STEP 3 COMPLETE";
    // Display the total cost and then the MST that leads to that cost
    cout << totalCost << "\n";
    for (int i = 1; i < numV; ++i) {
        if (parent[i] != -1) {
            cout << parent[i] << " " << i << " " << v[i].distance << "\n";
        }
    }

    return 0;

}