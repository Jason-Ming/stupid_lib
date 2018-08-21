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

// ����˫������
#define list_for_each_all(pos, head)									\
		for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_all_safe(pos, n, head)							\
		for (pos = (head)->next, n = pos->next; pos != (head);			\
		pos = n, n = pos->next)

#define list_for_each_all_reverse(pos, head)							\
		for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define list_for_each_all_reverse_safe(pos, p, head)					\
		for (pos = (head)->prev, p = pos->prev; pos != (head);			\
		pos = p, p = pos->prev)

#define list_for_each(pos, head, start, end)							\
		for (pos = ((start) == (head))?((start)->next):(start); 		\
		pos != (((end) == (head))?(head):(end)->next); pos = (pos)->next)

#define list_for_each_safe(pos, n, head, start, end)					\
		for (pos = ((start) == (head))?((start)->next):(start), n = pos->next; \
		pos != (((end) == (head))?(head):(end)->next); pos = n, n = pos->next)

#define list_for_each_reverse(pos, head, start, end)					\
		for (pos = ((end) == (head))?((end)->prev):(end);				\
		pos != (((start) == (head))?(head):(start)->prev); pos = pos->prev)

#define list_for_each_reverse_safe(pos, p, head, start, end)			\
		for (pos = ((end) == (head))?((end)->prev):(end), p = pos->prev; \
		pos != (((start) == (head))?(head):(start)->prev); pos = p, p = pos->prev)

#define list_entry(ptr, type, member)									\
		container_of(ptr, type, member)

#endif

