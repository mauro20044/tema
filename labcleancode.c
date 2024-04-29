#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} NODE;

typedef struct Graph {
    int vertices;
    int *visited;
    NODE **adjacency_lists;
} GPH;

NODE *create_node(int v) {
    NODE *new_node = malloc(sizeof(NODE));
    if (new_node == NULL) {
        fprintf(stderr, "Eroare: Nu s-a putut aloca memorie pentru un nou nod.\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = v;
    new_node->next = NULL;
    return new_node;
}

GPH *create_graph(int vertices) {
    GPH *graph = malloc(sizeof(GPH));
    if (graph == NULL) {
        fprintf(stderr, "Eroare: Nu s-a putut aloca memorie pentru graful principal.\n");
        exit(EXIT_FAILURE);
    }
    graph->vertices = vertices;
    graph->adjacency_lists = malloc(vertices * sizeof(NODE *));
    graph->visited = malloc(vertices * sizeof(int));
    if (graph->adjacency_lists == NULL || graph->visited == NULL) {
        fprintf(stderr, "Eroare: Nu s-a putut aloca memorie pentru listele de adiacență sau vectorul de vizitare.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < vertices; i++) {
        graph->adjacency_lists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

void add_edge(GPH *graph, int src, int dest) {
    NODE *new_node = create_node(dest);
    new_node->next = graph->adjacency_lists[src];
    graph->adjacency_lists[src] = new_node;

    new_node = create_node(src);
    new_node->next = graph->adjacency_lists[dest];
    graph->adjacency_lists[dest] = new_node;
}

void insedg(int nr_of_vertices, int nr_of_edges, GPH *graph) {
    int src, dest;
    printf("Adauga %d muchii (de la 0 la %d)\n", nr_of_edges, nr_of_vertices-1);
    for (int i = 0; i < nr_of_edges; i++) {
        scanf("%d%d", &src, &dest);
        add_edge(graph, src, dest);
    }
}

int is_empty(NODE *queue) {
    return queue == NULL;
}

void enqueue(NODE **queue, int data) {
    NODE *new_node = create_node(data);

    if (is_empty(*queue))
        *queue = new_node;
    else {
        NODE *temp = *queue;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

int dequeue(NODE **queue) {
    if (is_empty(*queue)) {
        fprintf(stderr, "Eroare: Încercare de a scoate un element dintr-o coadă goală.\n");
        exit(EXIT_FAILURE);
    }
    int data = (*queue)->data;
    NODE *temp = *queue;
    *queue = (*queue)->next;
    free(temp);
    return data;
}

void print_graph(GPH *graph) {
    for (int i = 0; i < graph->vertices; i++) {
        printf("Vertex %d: ", i);
        NODE *temp = graph->adjacency_lists[i];
        while (temp) {
            printf("%d ", temp->data);
            temp = temp->next;
        }
        printf("\n");
    }
}

void wipe_visited_list(GPH *graph, int nr_of_vertices) {
    for (int i = 0; i < nr_of_vertices; i++) {
        graph->visited[i] = 0;
    }
}

void DFS(GPH *graph, int vertex_nr) {
    if (vertex_nr < 0 || vertex_nr >= graph->vertices || graph->visited[vertex_nr])
        return;

    graph->visited[vertex_nr] = 1;
    printf("%d->", vertex_nr);

    NODE *temp = graph->adjacency_lists[vertex_nr];
    while (temp != NULL) {
        int connected_vertex = temp->data;
        DFS(graph, connected_vertex);
        temp = temp->next;
    }
}

void BFS(GPH *graph, int start) {
    NODE *queue = NULL;

    graph->visited[start] = 1;
    enqueue(&queue, start);

    while (!is_empty(queue)) {
        int current = dequeue(&queue);
        printf("%d ", current);

        NODE *temp = graph->adjacency_lists[current];

        while (temp != NULL) {
            int adj_vertex = temp->data;
            if (adj_vertex >= 0 && adj_vertex < graph->vertices && !graph->visited[adj_vertex]) {
                graph->visited[adj_vertex] = 1;
                enqueue(&queue, adj_vertex);
            }
            temp = temp->next;
        }
    }
}

int main() {
    int nr_of_vertices, nr_of_edges, starting_vertex;

    printf("Cate noduri are graful? ");
    if (scanf("%d", &nr_of_vertices) != 1 || nr_of_vertices <= 0) {
        fprintf(stderr, "Eroare: Numărul de noduri trebuie să fie un număr pozitiv nenul.\n");
        return EXIT_FAILURE;
    }

    printf("Cate muchii are graful? ");
    if (scanf("%d", &nr_of_edges) != 1 || nr_of_edges < 0) {
        fprintf(stderr, "Eroare: Numărul de muchii trebuie să fie un număr pozitiv sau zero.\n");
        return EXIT_FAILURE;
    }

    GPH *graph = create_graph(nr_of_vertices);
    insedg(nr_of_vertices, nr_of_edges, graph);

    printf("De unde plecam in DFS? ");
    if (scanf("%d", &starting_vertex) != 1 || starting_vertex < 0 || starting_vertex >= nr_of_vertices) {
        fprintf(stderr, "Eroare: Nodul de start trebuie să fie un număr valid între 0 și %d.\n", nr_of_vertices - 1);
        return EXIT_FAILURE;
    }

    printf("Parcurgere cu DFS: ");
    DFS(graph, starting_vertex);
    wipe_visited_list(graph, nr_of_vertices);
    printf("\n");

    printf("De unde plecam in BFS? ");
    if (scanf("%d", &starting_vertex) != 1 || starting_vertex < 0 || starting_vertex >= nr_of_vertices) {
        fprintf(stderr, "Eroare: Nodul de start trebuie să fie un număr valid între 0 și %d.\n", nr_of_vertices - 1);
        return EXIT_FAILURE;
    }

    printf("Parcurgere cu BFS: ");
    BFS(graph, starting_vertex);

    return EXIT_SUCCESS;
}

