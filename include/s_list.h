#ifndef __S_LIST_H__
#define __S_LIST_H__
#include <string.h>
#include <s_clinkage.h>
#include <s_type.h>
#include <s_defines.h>

// ˫������ڵ�
struct list_head
{
struct list_head * next, *prev;
};


// ��ʼ���ڵ㣺����name�ڵ��ǰ�̽ڵ�ͺ�̽ڵ㶼��ָ��name����
#define LIST_HEAD_INIT(name)	{ &(name), &(name) }

// �����ͷ(�ڵ�)
//���½�˫�������ͷname��������name��ǰ�̽ڵ�ͺ�̽ڵ㶼��ָ��name����
#define LIST_HEAD(name) 												\
		struct list_head name = LIST_HEAD_INIT(name)

// ��ʼ���ڵ㣺��list�ڵ��ǰ�̽ڵ�ͺ�̽ڵ㶼��ָ��list����
static inline void INIT_LIST_HEAD(struct list_head * list)
{
	list->next = list;
	list->prev = list;
}


// ��ӽڵ㣺��new���뵽prev��next֮�䡣
static inline void __list_add(struct list_head * new, 
	struct list_head * prev, 
	struct list_head * next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}


// ���new�ڵ㣺��new��ӵ�head֮����new��Ϊhead�ĺ�̽ڵ㡣
static inline void list_add(struct list_head * new, struct list_head * head)
{
	__list_add(new, head, head->next);
}


// ���new�ڵ㣺��new��ӵ�head֮ǰ������new��ӵ�˫�����ĩβ��
static inline void list_add_tail(struct list_head * new, struct list_head * head)
{
	__list_add(new, head->prev, head);
}


// ��˫������ɾ��entry�ڵ㡣
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}


// ��˫������ɾ��entry�ڵ㡣
static inline void list_del(struct list_head * entry)
{
	__list_del(entry->prev, entry->next);
}


// ��˫������ɾ��entry�ڵ㡣
static inline void __list_del_entry(struct list_head * entry)
{
	__list_del(entry->prev, entry->next);
}


// ��˫������ɾ��entry�ڵ㣬����entry�ڵ��ǰ�̽ڵ�ͺ�̽ڵ㶼ָ��entry����
static inline void list_del_init(struct list_head * entry)
{
	__list_del_entry(entry);
	INIT_LIST_HEAD(entry);
}


// ��new�ڵ�ȡ��old�ڵ�
static inline void list_replace(struct list_head * old, 
	struct list_head * new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}


// ˫�����Ƿ�Ϊ��
static inline int list_empty(const struct list_head * head)
{
	return head->next == head;
}


// ��ýṹ��(TYPE)�ı�����Ա(MEMBER)�ڴ˽ṹ���е�ƫ������
#define s_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

// ����"�ṹ��(type)����"�е�"���Ա����(member)��ָ��(ptr)"
//����ȡָ�������ṹ�������ָ��
#define container_of(ptr, type, member) ({								\
	const __typeof( ((type *)0)->member ) *__mptr = (ptr);			\
	(type *)( (char *)__mptr - s_offsetof(type,member) );})

#define list_entry(ptr, type, member)									\
	container_of(ptr, type, member)

// ����˫������
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

