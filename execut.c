#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>


typedef struct Node {
    int destination;
    struct Node* next;
} Node;

typedef struct Graph {
    int numVertices;
    Node** adjList;
} Graph;

Graph* graph;

void initializeGraph(int numVertices) {
    graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->adjList = (Node**)malloc(numVertices * sizeof(Node*));
	int i;
    for (i = 0; i < numVertices; ++i) {
        graph->adjList[i] = NULL;
    }
}

bool checkEdge(int origin, int destination) {
    Node* current = graph->adjList[origin - 1];
    while (current != NULL) {
        if (current->destination == destination)
            return true;
        current = current->next;
    }
    return false;
}

void addEdge(int origin, int destination) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->destination = destination;
    newNode->next = graph->adjList[origin - 1];
    graph->adjList[origin - 1] = newNode;

    newNode = (Node*)malloc(sizeof(Node));
    newNode->destination = origin;
    newNode->next = graph->adjList[destination - 1];
    graph->adjList[destination - 1] = newNode;
}

int minDistance(int dist[], bool visited[], int numVertices) {
    int min = INT_MAX, minIndex;
	int v;
    for (v = 0; v < numVertices; v++)
        if (!visited[v] && dist[v] <= min)
            min = dist[v], minIndex = v;
    return minIndex;
}

void printPath(int previous[], int destination) {
    if (previous[destination - 1] == -1) {
        printf("%d", destination);
        return;
    }
    printPath(previous, previous[destination - 1] + 1);
    printf(" -> %d", destination);
}

void dijkstraAlgorithm(int origin, int destination, int numVertices) {
    int dist[numVertices];
    bool visited[numVertices];
    int previous[numVertices];
	int i;
	int count;
    for (i = 0; i < numVertices; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
        previous[i] = -1;
    }

    dist[origin - 1] = 0;

    for (count = 0; count < numVertices - 1; count++) {
        int u = minDistance(dist, visited, numVertices);
        visited[u] = true;
        Node* current = graph->adjList[u];

        while (current != NULL) {
            int v = current->destination;
            if (!visited[v - 1] && dist[u] != INT_MAX && dist[u] + 1 < dist[v - 1]) {
                dist[v - 1] = dist[u] + 1;
                previous[v - 1] = u;
            }
            current = current->next;
        }
    }

    if (dist[destination - 1] == INT_MAX) {
        printf("No path exists from vertex %d to vertex %d\n", origin, destination);
    } else {
        printf("Distance between vertex %d and vertex %d: %d\n", origin, destination, dist[destination - 1]);
        printf("Path: ");
        printPath(previous, destination);
        printf("\n");
    }
}

int createExampleGraphs(int option) {
    int nVertices;
    switch (option) {
        case 1:
            nVertices = 7;
            initializeGraph(nVertices);
            addEdge(1, 2);
            addEdge(2, 3);
            addEdge(3, 7);
            addEdge(6, 5);
            addEdge(5, 4);
            addEdge(4, 1);
            return nVertices;
        case 2:
            nVertices = 6;
            initializeGraph(nVertices);
            addEdge(1, 2);
            addEdge(2, 3);
            addEdge(1, 4);
            addEdge(4, 5);
            addEdge(5, 6);
            return nVertices;
        case 3:
            nVertices = 4;
            initializeGraph(nVertices);
            addEdge(1, 2);
            addEdge(2, 4);
            addEdge(1, 3);
            addEdge(3, 2);
            return nVertices;
    }
    return 0;
}

int main() {
    int nVertices;
    setlocale(LC_ALL, "English");
    char response;

    printf("Do you want to load a test/example graph? (enter 'y' for yes or 'n' for no): ");
    while (1) {
        scanf(" %c", &response);
        response = tolower(response);
        if (response == 'y' || response == 'n') {
            break;
        } else {
            printf("Please enter 'y' for yes or 'n' for no: ");
        }
    }

    if (response == 'y') {
        bool validChoice = true;
        while (validChoice) {
            printf("Which example graph do you want to choose? Enter a number from 1 to 3: ");
            int option;
            scanf("%d", &option);
            if (option >= 1 && option <= 3) {
                nVertices = createExampleGraphs(option);
                validChoice = false;
            } else {
                printf("Enter a number between 1 and 3!\n");
            }
        }
    } else {
        printf("Enter the number of vertices: ");
        scanf("%d", &nVertices);
        initializeGraph(nVertices);

        printf("Now enter the edges in the format 'origin, destination' (type 'exit' to stop):\n");
        int numEdges = 0;
        int maxEdges = (nVertices * (nVertices - 1)) / 2;

        while (getchar() != '\n');
        while (numEdges < maxEdges) {
            printf("Edge (or 'exit' to stop): ");
            char input[100];
            fgets(input, 100, stdin);
            if (strcmp(input, "exit\n") == 0) {
                break;
            }

            char* comma = strchr(input, ',');
            if (comma == NULL) {
                printf("Error: Please enter the edges in the correct format 'origin, destination'.\n");
                continue;
            }

            char* newline = strchr(input, '\n');
            if (newline != NULL) {
                *newline = '\0';
            }

            *comma = '\0';
            int origin, destination;
            if (sscanf(input, "%d", &origin) != 1 || sscanf(comma + 1, "%d", &destination) != 1) {
                printf("Error: Please enter the edges in the correct format 'origin, destination'.\n");
                continue;
            }

            if (origin == -1 || destination == -1)
                break;
            if (origin < 1 || origin > nVertices || destination < 1 || destination > nVertices || origin == destination) {
                printf("Error: Invalid edge.\n");
                continue;
            }
            if (checkEdge(origin, destination)) {
                printf("Error: Edge already exists.\n");
                continue;
            }
            addEdge(origin, destination);
            numEdges++;
            if (numEdges == maxEdges) {
                printf("Maximum number of edges for this graph (%d) reached.\n", maxEdges);
                break;
            }
        }
    }

    int origin, destination;
    printf("Enter the origin vertex: ");
    scanf("%d", &origin);
    if (origin < 1 || origin > nVertices) {
        printf("Error: Invalid origin vertex.\n");
        return EXIT_FAILURE;
    }

    printf("Enter the destination vertex: ");
    scanf("%d", &destination);
    if (destination < 1 || destination > nVertices) {
        printf("Error: Invalid destination vertex.\n");
        return EXIT_FAILURE;
    }

    dijkstraAlgorithm(origin, destination, nVertices);
	int i;
    for (i = 0; i < nVertices; ++i) {
        Node* current = graph->adjList[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(graph->adjList);
    free(graph);

    return EXIT_SUCCESS;
}

