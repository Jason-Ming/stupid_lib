#ifndef __S_BINTREE_H__
#define __S_BINTREE_H__
#include <string.h>
#include <s_clinkage.h>
#include <s_type.h>
#include <s_defines.h>

// 二叉树节点
struct bintree_node
{
    struct bintree_node *left, *right, *root;
};

// 初始根节点：将节点的左子树和右子树和根节点都是指向节点本身。
static inline void INIT_BINTREE_ROOT(struct bintree_node * bintree)
{
	bintree->left = bintree;
	bintree->right = bintree;
    bintree->root = bintree;
}


// 添加节点：将new作为左子树插入到root之后，，并将原左子树作为新节点的左子树。
static inline void __bintree_add_left_left(struct bintree_node * new, 
	struct bintree_node * root,
	struct bintree_node * left)
{
	left->root = new;
	new->left = left;
	new->root = root;
	root->left = new;
}

// 添加节点：将new作为左子树插入到root之后，，并将原左子树作为新节点的左子树。
static inline void BINTREE_ADD_LEFT_LEFT(struct bintree_node * new, struct bintree_node * root)
{
	__list_add(new, root, root->left);
}

// 添加节点：将new作为左子树插入到root之后，，并将原左子树作为新节点的右子树。
static inline void __bintree_add_left_right(struct bintree_node * new, 
	struct bintree_node * root,
	struct bintree_node * left)
{
	left->root = new;
	new->right = left;
	new->root = root;
	root->left = new;
}

// 添加节点：将new作为左子树插入到root之后，，并将原左子树作为新节点的右子树。
static inline void BINTREE_ADD_LEFT_RIGHT(struct bintree_node * new, struct bintree_node * root)
{
	__list_add(new, root, root->left);
}

// 添加节点：将new作为右子树插入到root之后，，并将原右子树作为新节点的左子树。
static inline void __bintree_add_right_left(struct bintree_node * new, 
	struct bintree_node * root,
	struct bintree_node * right)
{
	right->root = new;
	new->left = right;
	new->root = root;
	root->right = new;
}

// 添加节点：将new作为右子树插入到root之后，，并将原右子树作为新节点的左子树。
static inline void BINTREE_ADD_RIGHT_LEFT(struct bintree_node * new, struct bintree_node * root)
{
	__list_add(new, root, root->right);
}

// 添加节点：将new作为右子树插入到root之后，，并将原右子树作为新节点的右子树。
static inline void __bintree_add_right_right(struct bintree_node * new, 
	struct bintree_node * root,
	struct bintree_node * right)
{
	right->root = new;
	new->right = right;
	new->root = root;
	root->right = new;
}

// 添加节点：将new作为右子树插入到root之后，，并将原右子树作为新节点的右子树。
static inline void BINTREE_ADD_RIGHT_RIGHT(struct bintree_node * new, struct bintree_node * root)
{
	__list_add(new, root, root->right);
}


// 从双链表中删除entry节点。
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}


// 从双链表中删除entry节点。
static inline void list_del(struct list_head * entry)
{
	__list_del(entry->prev, entry->next);
}


// 从双链表中删除entry节点。
static inline void __list_del_entry(struct list_head * entry)
{
	__list_del(entry->prev, entry->next);
}


// 从双链表中删除entry节点，并将entry节点的前继节点和后继节点都指向entry本身。
static inline void list_del_init(struct list_head * entry)
{
	__list_del_entry(entry);
	INIT_LIST_HEAD(entry);
}


// 用new节点取代old节点
static inline void list_replace(struct list_head * old, 
	struct list_head * new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}


// 双链表是否为空
static inline int list_empty(const struct list_head * head)
{
	return head->next == head;
}


// 获得结构体(TYPE)的变量成员(MEMBER)在此结构体中的偏移量。
#define s_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

// 根据"结构体(type)变量"中的"域成员变量(member)的指针(ptr)"
//来获取指向整个结构体变量的指针
#define container_of(ptr, type, member) ({								\
	const __typeof( ((type *)0)->member ) *__mptr = (ptr);			\
	(type *)( (char *)__mptr - s_offsetof(type,member) );})

#define list_entry(ptr, type, member)									\
	container_of(ptr, type, member)

// 遍历双向链表
#define LIST_FOR_EACH_ALL(p_list_head)									\
	for (struct list_head *__head = &((p_list_head)->list), *__pos = (__head)->next; \
        __pos != (__head); __pos = __pos->next)

#define LIST_FOR_EACH_ALL_SAFE(p_list_head)							\
	for (struct list_head *__head = &((p_list_head)->list), *__pos = (__head)->next, *__n = __pos->next; \
        __pos != (__head); __pos = __n, __n = __pos->next)

#define LIST_FOR_EACH_ALL_REVERSE(p_list_head)							\
	for (struct list_head *__head = &((p_list_head)->list), *__pos = (__head)->prev; \
        __pos != (__head); __pos = __pos->prev)

#define LIST_FOR_EACH_ALL_REVERSE_SAFE(p_list_head)					\
	for (struct list_head *__head = &((p_list_head)->list), *__pos = (__head)->prev, *__p = __pos->prev; \
        __pos != (__head); __pos = __p, __p = __pos->prev)

//iterator begin from begin->next to end->prev
#define list_for_each(p_list_begin, p_list_end)							\
	for (struct list_head *__begin = &((p_list_begin)->list), *__end = &((p_list_end)->list), *__pos = __begin->next; \
        __pos != (__end); __pos = (__pos)->next)

#define list_for_each_safe(p_list_begin, p_list_end)					\
	for (struct list_head *__begin = &((p_list_begin)->list), *__end = &((p_list_end)->list), *__pos = __begin->next, *__n = __pos->next; \
        __pos != (__end); __pos = __n, __n = __pos->next)

#define list_for_each_reverse(p_list_begin, p_list_end)					\
	for (struct list_head *__begin = &((p_list_begin)->list), *__end = &((p_list_end)->list), *__pos = (__end)->prev; \
        __pos != (__begin); __pos = __pos->prev)

#define list_for_each_reverse_safe(p_list_begin, p_list_end)			\
	for (struct list_head *__begin = &((p_list_begin)->list), *__end = &((p_list_end)->list), *__pos = (__end)->prev, *__p = __pos->prev; \
        __pos != (__begin); __pos = __p, __p = __pos->prev)


#define LIST_GET_ITERATOR(node_type) list_entry(__pos, node_type, list)
#define LIST_RMV_NODE(p_list_node) list_del_init(&p_list_node->list)
#endif


