#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50
#define MAX_QUEUE_SIZE 20
#define INF 100



typedef struct {
	int queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;

void queue_init(QueueType* q) {
	q->front = q->rear = 0;
}

int is_empty(QueueType* q) {
	return (q->front == q->rear);
}

int is_full(QueueType* q) {
	return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front);
}

void enqueue(QueueType* q, int item) {
	if (is_full(q)) {
		printf("Queue is full\n");
		return;
	}
	q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
	q->queue[q->rear] = item;
}

int dequeue(QueueType* q) {
	if (is_empty(q)) {
		printf("Queue is empty\n");
		return -1;
	}
	q->front = (q->front + 1) % MAX_QUEUE_SIZE;
	return q->queue[q->front];
}

typedef struct node {
	int vertex;
	struct node* link;
}Node;

typedef struct adjList {
	Node* head;
}AdjList;

typedef struct graph {
	int n;
	AdjList* array;
	int* visited;
}Graph;

typedef struct KGraph { // 크루스칼 용 그래프 구조체
	int adj_mat[MAX][MAX];
	int n;
}KGraph;

typedef struct edge {
	int key;
	int u;
	int v;
}Edge;


Node* createNode(int v) {

	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->vertex = v;
	newNode->link = NULL;
	return newNode;

}

Graph* createGraph(int vertices, int city_num[]) {
	Graph* g = (Graph*)malloc(sizeof(Graph));
	g->n = vertices;

	g->array = (AdjList*)malloc(vertices * sizeof(AdjList));
	g->visited = (int*)malloc(vertices * sizeof(int));

	for (int i = 0; i < vertices; i++) {
		int w = city_num[i];
		g->array[w].head = NULL;
		g->visited[w] = 0;
	}

	return g;
}

void init_graph(Graph* g, int vertices, int city_num[]) {
	for (int i = 0; i < vertices; i++) {
		int w = city_num[i];
		g->visited[w] = 0;
	}
}

void init_KGraph(KGraph* kg) {
	kg->n = 0;
	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < MAX; j++) {
			if (i == j) kg->adj_mat[i][j] = 0;
			else kg->adj_mat[i][j] = INF;
		}
	}
}

int insert_weight_edge(KGraph* kg, int u, int v, int weight) {

	if (kg->adj_mat[u][v] == INF) {
		kg->adj_mat[u][v] = kg->adj_mat[v][u] = weight;
		return 1;
	}
	else {
		return 0;
	}
}

void addEdge(Graph* g, int u, int v) {
	Node* check = NULL;
	Node* newNode = createNode(v);

	if (g->array[u].head == NULL) {
		newNode->link = g->array[u].head;
		g->array[u].head = newNode;
	}
	else {
		check = g->array[u].head;
		while (check->link != NULL) {
			check = check->link;
		}
		check->link = newNode;
	}

	newNode = createNode(u);
	if (g->array[v].head == NULL) {
		newNode->link = g->array[v].head;
		g->array[v].head = newNode;
	}
	else {
		check = g->array[v].head;
		while (check->link != NULL) {
			check = check->link;
		}
		check->link = newNode;
	}
}

void bfs(Graph* g, int city_num[]) {

	QueueType q;

	for (int i = 0; i < g->n; i++) {

		int start = city_num[i];

		if (!g->visited[start]) {
			printf("(");
			queue_init(&q);
			g->visited[start] = 1;
			enqueue(&q, start);

			while (!is_empty(&q)) {
				int curr = dequeue(&q);
				printf("%d, ", curr);

				Node* tmp = g->array[curr].head;
				while (tmp) {
					int adjVertex = tmp->vertex;

					if (g->visited[adjVertex] == 0) {
						g->visited[adjVertex] = 1;
						enqueue(&q, adjVertex);

					}
					tmp = tmp->link;
				}
			}
			printf(") ");
		}
	}
}

void dfs(Graph* g, int v) {
	g->visited[v] = 1;
	printf("%d, ", v);
	Node* w;
	for (w = g->array[v].head; w; w = w->link) {
		if (!g->visited[w->vertex])
			dfs(g, w->vertex);
	}
}

void connected_dfs(Graph* g, int v) {
	g->visited[v] = 1;
	Node* w;
	for (w = g->array[v].head; w; w = w->link) {
		if (!g->visited[w->vertex])
			connected_dfs(g, w->vertex);
	}
}

int count;
int parent[MAX];

void init_set(int n) {
	parent[n] = -1;
}

int find_set(int v) {
	while (parent[v] >= 0) {
		v = parent[v];
	}
	return v;
}

void union_set(int x, int y) {

	int tmp;

	if (parent[x] < 0 && parent[y] < 0) {
		if (parent[x] <= parent[y]) {
			tmp = parent[y];
			parent[y] = x;
			parent[x] += tmp;
		}
		else {
			tmp = parent[x];
			parent[x] = y;
			parent[y] += tmp;
		}
	}

	else if (parent[x] < 0 && parent[y] >= 0) {
		tmp = parent[x];
		parent[x] = parent[y];
		parent[parent[y]] += tmp;
	}

	else if (parent[x] >= 0 && parent[y] < 0) {
		tmp = parent[y];
		parent[y] = parent[x];
		parent[parent[x]] += tmp;
	}

	else {
		tmp = parent[x];
		parent[x] = parent[y];
		parent[tmp] = parent[y];
	}
}

int compare(const void* a, const void* b) {
	Edge* x = (Edge*)a;
	Edge* y = (Edge*)b;

	if (x->key < y->key) return -1;
	if (x->key > y->key) return 1;
	return 0;
}

int kruskal(Edge edge[], int n, int city_num[]) {
	qsort(edge, count, sizeof(Edge), compare);
	int x, y;
	int ret = 0;
	int selected = 0;
	for (int i = 0; i < n; i++) {
		init_set(city_num[i]);
	}

	for (int i = 0; selected < n - 1; i++) {
		x = edge[i].u;
		y = edge[i].v;

		if (x < -10 || y < -10) break;



		if (find_set(x) != find_set(y)) {
			ret += edge[i].key;

			printf("(%d, %d), ", x, y);
			selected++;

			union_set(x, y);
		}
	}

	return ret;
}


int main() {
	while (1) {
		int exit = 0;
		int result;
		printf("아주대학교 지하 통로 구축 서비스\n");

		int n;
		int city_num[MAX];
		Edge edge[INF];
		int idx = 0;
		Graph* g;
		KGraph* kg = (KGraph*)malloc(sizeof(KGraph));
		init_KGraph(kg);
		do {
			printf("건물 수를 입력해 주세요\n");
			scanf("%d", &n);

			if (n == 0) { // 프로그램 종료 조건
				exit = 1;
				break;
			}

			if (n > 20) { // 건물 수 제한 값 넘는 경우
				printf("최대 건물 수를 초과했습니다.\n");
				continue;
			}


			char input[1000];
			printf("건물 번호를 입력해 주세요\n");

			getchar();
			gets(input);

			int city_cnt = 1;

			for (int i = 0; input[i]; i++) {
				if (input[i] == ' ') city_cnt++;
			}

			char* ptr = strtok(input, " ");

			while (ptr != NULL) {
				city_num[idx++] = atoi(ptr);
				ptr = strtok(NULL, " ");

			}

			if (city_cnt > n) {
				printf("제시된 도시 수보다 도시 이름의 개수가 더 많습니다\n");
				continue;
			}
			else break;

		} while (1);

		if (exit) break; // 프로그램 종료 조건

		g = createGraph(n, city_num);

		printf("건물 간 통로 구축 정보를 입력하세요\n");


		while (1) { // 건물 간 통로 구축 정보 입력
			char line[20];
			gets(line);
			if (strcmp(line, ".") == 0) break;
			char* ptr = strtok(line, ", ");
			int u = atoi(ptr);
			ptr = strtok(NULL, ", ");
			int v = atoi(ptr);
			ptr = strtok(NULL, "\0");
			int cost = atoi(ptr);

			int check1 = 0;
			int check2 = 0;

			for (int i = 0; i < idx; i++) {
				if (u == city_num[i]) {
					check1 = 1;
				}
				if (v == city_num[i]) {
					check2 = 1;
				}
			}

			if (check1 && check2) {
				int good = insert_weight_edge(kg, u, v, cost);
				if (good) {
					addEdge(g, u, v);
				}
				else {
					printf("중복된 정보입니다\n");
				}
			}
			else {
				printf("건물이 잘못 입력되었습니다\n");
			}
		}

		for (int i = 0; i < MAX; i++) {
			for (int j = i; j < MAX; j++) {
				if (kg->adj_mat[i][j] != INF && kg->adj_mat[i][j] != 0) {
					edge[count].key = kg->adj_mat[i][j];
					edge[count].u = i;
					edge[count].v = j;
					count++;
				}
			}
		}

		int connected_check = 0;

		for (int i = 0; i < g->n; i++) {

			int start = city_num[i];
			if (!g->visited[start]) {
				connected_check++;
				connected_dfs(g, city_num[i]);
			}
		}

		if (connected_check > 1) {
			printf("전체 건물들을 연결할 수 없습니다\n");
		}
		else {
			printf("성공적인 계획입니다\n");
		}

		init_graph(g, n, city_num);

		printf("dfs: ");
		for (int i = 0; i < g->n; i++) {

			int start = city_num[i];
			if (!g->visited[start]) {

				printf("(");
				dfs(g, city_num[i]);
				printf(") ");
			}

		}

		printf("\n");

		init_graph(g, n, city_num);

		printf("bfs: ");
		bfs(g, city_num);
		printf("\n");

		printf("Kruskal: ");
		int mst = kruskal(edge, n, city_num);
		printf("%d\n", mst);


		printf("------------------------------------------------------------\n");

	}

	printf("서비스를 종료합니다");
	return 0;
}