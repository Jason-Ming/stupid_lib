#ifndef __S_LIST_H__
#define __S_LIST_H__
#include <string.h>
#include <s_clinkage.h>
#include <s_type.h>
#include <s_defines.h>

// 双向链表节点
struct list_head
{
struct list_head * next, *prev;
};


// 初始化节点：设置name节点的前继节点和后继节点都是指向name本身。
#define LIST_HEAD_INIT(name)	{ &(name), &(name) }

// 定义表头(节点)
//：新建双向链表表头name，并设置name的前继节点和后继节点都是指向name本身。
#define LIST_HEAD(name) 												\
		struct list_head name = LIST_HEAD_INIT(name)

// 初始化节点：将list节点的前继节点和后继节点都是指向list本身。
static inline void INIT_LIST_HEAD(struct list_head * list)
{
	list->next = list;
	list->prev = list;
}


// 添加节点：将new插入到prev和next之间。
static inline void __list_add(struct list_head * new, 
	struct list_head * prev, 
	struct list_head * next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}


// 添加new节点：将new添加到head之后，是new称为head的后继节点。
static inline void list_add(struct list_head * new, struct list_head * head)
{
	__list_add(new, head, head->next);
}


// 添加new节点：将new添加到head之前，即将new添加到双链表的末尾。
static inline void list_add_tail(struct list_head * new, struct list_head * head)
{
	__list_add(new, head->prev, head);
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

//iterator begin from start->next to end->prev
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

