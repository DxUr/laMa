#include "syntax_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct LinkedList LinkedList;
struct LinkedList {
    LinkedList *previous_sibling;
    void *value;
};

struct Node {
    NodeType type;
};


typedef struct {
    Node base;
    int value;
} NodeIdentifier;


typedef struct {
    Node base;
    LinkedList *last_child;
} NodeSpace;


typedef struct {
    Node base;
    const NodeIdentifier *identifier;
    const Node *value;
} NodeLet;


NodeType nodeGetType(const Node *p_node) {
    return p_node->type;
}

static inline void _indent(int p_indent) { for (;p_indent; p_indent--) printf("  "); }

void nodeExpose(const Node *p_node) {
    if (!p_node){
        return;
    }
    static int indent = 0;
    switch (p_node->type) {
        case NODE_SPACE:
            NodeSpace *space = (NodeSpace*)p_node;
            _indent(indent);
            puts("space");
            indent++;
            for (LinkedList *child = space->last_child; child; child = child->previous_sibling)
                nodeExpose((Node*)child->value);
            indent--;
            return;
        case NODE_LET:
            NodeLet *let = (NodeLet*)p_node;
            _indent(indent);
            printf("let %u\n", let->identifier->value);
            indent++;
            nodeExpose(let->value);
            indent--;
            return;
        default:
            assert(0);
    }
}

Node *nodeIdentifierCreate(int p_value)
{
    NodeIdentifier *id = (NodeIdentifier *)malloc(sizeof(NodeIdentifier));
    *id = (NodeIdentifier){
        .base.type = NODE_IDENTIFIER,
        .value = p_value};
    return (Node *)id;
}

Node *nodeSpaceCreate() {
        NodeSpace *space = (NodeSpace*)malloc(sizeof(NodeSpace));
        *space = (NodeSpace){
            .base.type = NODE_SPACE,
            .last_child = NULL
        };
        return (Node*)space;
}


Node *nodeLetCreate(const Node *p_identifier) {
    assert(p_identifier && p_identifier->type == NODE_IDENTIFIER);
    NodeLet *let = (NodeLet*)malloc(sizeof(NodeLet));
    *let = (NodeLet){
        .base.type = NODE_LET,
        .identifier = (NodeIdentifier*)p_identifier
    };
    return (Node*)let;
}


void nodeSpaceAddChild(Node *p_node, const Node *p_child) {
    assert(p_node && p_child && p_node->type == NODE_SPACE);
    LinkedList *ll = (LinkedList*)malloc(sizeof(LinkedList));
    ll->value = (void*)p_child;
    ll->previous_sibling = ((NodeSpace*)p_node)->last_child;
    ((NodeSpace*)p_node)->last_child = ll;
}


void nodeLetSetValue(Node *p_node, const Node *p_value) {
    assert(p_node && p_value && p_node->type == NODE_LET);
    ((NodeLet*)p_node)->value = p_value;
}



