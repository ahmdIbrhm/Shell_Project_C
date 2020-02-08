#ifndef LIST_H_
#define LIST_H_

typedef struct commandBack{
	int id;
	int pid;
	char *status;
	char *name;
}commandBack;

typedef commandBack element;
typedef struct node{
	struct node *next;
	 element e;
}node;
typedef struct list{
	node *head;
	node *tail;
	int size;
}list;

list createList()
{
	list l;
	l.head = l.tail=NULL;
	l.size = 0;
	return l;
}
void insertTail(list *l, element e)
{
	node *temp;
	temp = (node *)malloc(sizeof(node));
	temp->e = e;
	temp->next = NULL;
	if(l->size==0)
		l->head=l->tail=temp;
	else
	{
		l->tail->next = temp;
		l->tail=temp;
	}
	l->size++;
}
element getElement(list *l,char *s)
{
	node *temp;
	temp=l->head;
	while(temp!=NULL)
	{
		if(temp->e.name==s)
			return temp->e;
		temp=temp->next;
	}
}
int removeHead(list *l) {
	if (l->size==0)
		return 0;
	else
	{
		node *temp;
		temp = l->head;
		l->head = l->head->next;
		free(temp);
		l->size--;
		return 1;
	}
}

int removeTail(list *l) {
	node *temp = l->tail;
	if (l->size==0)
		return 0;
	else
	{
		if (l->size == 1)
			l->head = l->tail = NULL;
		else
		{
			node *it = l->head;
			while (it->next != l->tail)
				it = it->next;
			it->next = NULL;
			l->tail = it;
		}
		free(temp);
		l->size--;
		return 1;
	}
}
int removeItem(list *l,element e)
{
	node *temp;
	temp=l->head;
	node *curr;
	if(l->head->e.id==e.id)
		removeHead(l);
	else if(l->tail->e.id==e.id)
		removeTail(l);
	else
	{
		while(temp!=NULL)
		{
			if(temp->next->e.id==e.id)
			{
				curr=temp->next;
				temp->next=temp->next->next;
				free(curr);
				return 1;
			}
			temp=temp->next;
		}
		l->size--;
	}
	return 0;
}
void printList(list l)
{
	node *temp;
	temp=l.head;
	while(temp!=NULL)
	{
		printf("[%d]+   %s          %s\n",temp->e.id,temp->e.status,temp->e.name);	
		temp=temp->next;
	}
}
#endif