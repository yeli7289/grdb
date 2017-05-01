// GDB is the graph structure implemented in the graph database

// The api i need
// 1. GeneralToNorm
// 2. select
// 3. join
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "graph.h"
MAX_NUM_VERTICES = 10;
// stack for Kosaraju's algorithm
struct stack_s {
    int top;
    vertexid_t items[MAX_NUM_VERTICES];
} stack = {-1,{}};

void stack_push(vertexid_t vid) {
    stack.top++;
    if (stack.top < MAX_NUM_VERTICES)
		stack.items[stack.top] = vid;
    else {
		printf("Stack is full!\n");
		exit(1);
    }
}
vertexid_t stack_pop() {
    return stack.top < 0 ? NULL : stack.items[stack.top--];
}
graph_t graph_new(vertexid_t id)
{
	graph_t g;
	vertex_t v;
	/* Create first vertex in graph */
	v = (vertex_t) malloc(sizeof(struct vertex));
	assert (v != NULL);
	vertex_init(v);
	v->id = id;
	/* Create new graph */
	g = (graph_t) malloc(sizeof(struct graph));
	assert (g != NULL);
	graph_init(g);
	graph_insert_vertex(g, v);
	return g;
}
void dfs(graph_t *g, vertexid_t id){
	vertex_t v = graph_find_vertex_by_id(g,id);
	v->visited = 1;
	for (int i=0, int c=v->deg; i<c; ++i){
		vertex_t n = graph_find_vertex_by_id(g,v->neighbors[i]);
		if (!n->visited)
			dfs(g,n->id);
	}
	stack_push(n->id);
}
void create_scc(graph_t *g, vertexid_t id, graph_t *g_new) {
	vertex_t v = graph_find_vertex_by_id(g, id);
	v->visited = 1;
	for (int i=0, int c=v->deg; i<c; ++i){
		vertex_t n = graph_find_vertex_by_id(g, v->neighbors[i]);
		if (!n->visited)
			create_scc(g,n->id, g_new);
	}
	// insert vertex v into g_new
	graph_insert_vertex(g_new,v);
}
void SCC(graph_t *scc, graph_t g) {
	// first round DFS to push the vertices to stack.
	for (vertex_t vex=g->v; vex!=NULL; vex = vex->next){
		dfs(&g,vex->id);
	}
	// TODO: the function to reverse the graph
	reverse(&g);
	// TODO: the function to reset the visit status
	reset_visited(&g);
	// second round DFS to construct the scc
	int i;
	vertexid_t vid;
	vertex_t v
	while ((vid = stack_pop())!=NULL){
		if (!(v=g.graph_find_vertex_by_id(vid)).visited) {
			grapg_t g_scc = graph_new(vid);
	    	create_scc(&g, vid, &g_scc);
	    	// insert scc
	    	if (scc==NULL)
	    		scc = g_scc;
	    	else {
	    		scc->next = g_scc;
	    		scc = scc->next;
	    	}
	    }
	}
}

bool existed(graph_t g, vertex_t v) {
	vertex_t w;
	vertex_t ret;
	for (w=v; w!=NULL; w=w->next){
		ret = graph_find_vertex_by_id(g, w->id);
		if (ret!=NULL) return true;
	}
	return false;
}
void 
cli_graph_scc() {
	graph_t sccs;
	graph_t *tmp = &sccs;
	graph_t *tmp2 = &tmp;

	// run scc algorithm on each of the norm graph and construcgt the new scc graph
	for (graph_t g = graphs; g != NULL; g = g->next)
		SCC(tmp, g);

	// join each of the sccs from norm graph
	bool found;
	while (tmp2 != NULL) {
		// select all the vertexids 
		vertex_t vs = tmp2->v;
		graph_t next = tmp2->next;
		grapg_t cur = tmp2;
		found = false;
		while(next != NULL) {
			if (existed(next, vs)) {
				join(tmp2,next);
				cur->next = next->next;
				found = true;
			} else 
				cur = cur->next;
			next = next->next;
		}
		if (!found)
			tmp2 = tmp2->next;
	}
	// print out all the scc graphs after the joining process
	int cnt = 0;
	graph_t g;
	for (g = sccs; g != NULL; g = g->next) {
		graph_print(g, 0); /* no tuples */
		printf("\n");
	}

}