//Implement the work of the dynamic structure: Queue. 
//Adding an item · Deleting · Sorting: In work, use two sorting methods: quick and insert. Compare efficiency.

#include <iostream>
#include <ctime>
#include <fstream>
using namespace std;

typedef struct Queue
{
	int data;
	Queue *next;
	Queue *prev;

	Queue() : data(0), next(nullptr), prev(nullptr) {};
	Queue(int x) : data(x), next(nullptr), prev(nullptr) {};
	Queue(int x, Queue *prev, Queue *next) : data(x), prev(prev), next(next) {};
};

void print(Queue *q)
{
	cout << "{ ";
	while (q)
	{
		cout << q->data << " ";
		q = q->next;
	}
	cout << "}" << endl;
	cout << endl;
}

void clear(Queue *&q)
{
	if (!q)
		return;
	Queue *p;
	while (q)
	{
		p = q->next;
		delete q;
		q = p;
	}
	q = nullptr;
}

void ReadFile(char *filename, Queue *&q)
{
	if (q)
		clear(q);
	ifstream fin(filename, ios::in, ios::binary);
	if (!fin.is_open())
	{
		cout << "Could not open file." << endl;
		return;
	}

	Queue *p = nullptr;
	while (!fin.eof())
	{

		if (!p)
		{
			p = new Queue;
			fin >> p->data;
			q = p;
		}
		else
		{
			p->next = new Queue;
			fin >> p->next->data;
			p->next->prev = p;
			p = p->next;
		}
	}
}

Queue *copy(Queue *q)
{
	if (!q)
		return nullptr;
	Queue *head = new Queue;
	Queue *p = head;
	while (q)
	{
		p->next = new Queue(q->data);
		p->next->prev = p;
		p = p->next;
		q = q->next;
	}
	head = head->next;
	delete head->prev;
	head->prev = nullptr;
	return head;
}

void push(int data, Queue *&q)
{
	if (q)
	{
		Queue *p = q;
		while (p->next)
			p = p->next;
		p->next = new Queue(data, p, nullptr);
	}
	else q = new Queue(data, nullptr, nullptr);
}



int pop(Queue *&q)
{
	int res;
	if (q)
	{
		Queue *prev = nullptr, *p = q;
		while (p->next)
			p = p->next;

		res = p->data;
		prev = p->prev;
		delete p;
		if (prev)
			prev->next = nullptr;
		else q = nullptr;
		return res;
	}
	else return NULL;
}
		
bool remove(int data, Queue *&q)
{
	if (!q)
		return 0;
	Queue *prev = nullptr, *p = q;
	while (p)
	{
		if (p->data == data)
		{
			if (prev)
				prev->next = p->next;
			else q = p->next;
			if (p->next)
				p->next->prev = prev;
			delete p;
			return 1;
		}
		prev = p;
		p = p->next;
	}
	return 0;
}

Queue *get_tail(Queue *q)
{
	if (!q)
		return nullptr;
	while (q->next)
		q = q->next;
	return q;
}


void swap(Queue *l, Queue *r)
{
	l->data = r->data - l->data;
	r->data -= l->data;
	l->data += r->data;
}

bool is_sorted(Queue *l, Queue *r)
{
	bool res = 1;
	while (l != r)
	{
		if (l->data > l->next->data)
			res = 0;
		l = l->next;
	}
		
	return res;
}

Queue *partition(Queue *l, Queue *r)
{
	int pivot = l == r->prev ? l->data : (l->data + r->data) / 2;
	while (l != r)
	{
		while (r->data >= pivot && l!=r)
			r = r->prev;
		while (l->data < pivot && l!=r)
			l = l->next;
		
		if (l->data > r->data)
			swap(l, r);
	}
	return l;
}

void quicksort(Queue *l, Queue *r)
{
	if (l == r)
		return;
	if (is_sorted(l, r))
		return;
	Queue *m = partition(l, r);
	quicksort(l, m);
	quicksort(m->next, r);
}

void insertion_sort(Queue *q)
{
	Queue *p;
	if (!q)
		return;
	while (q)
	{
		p = q->prev;
		while (p && p->prev && p->prev->data > q->data)
			p = p->prev;
		if (p && p->data > q->data)
			swap(p, q);
		else q = q->next;
	}
}

int main()
{
	Queue *q = nullptr, *p = nullptr;
	clock_t t;

	char *filename = "C:\\Users\\morga\\Documents\\Visual Studio 2015\\Projects\\2course\\tasks\\Task7\\input.txt";

	int choise, N, seed, data;
	while (true)
	{
		cout << "READFILE - 0, PUSH - 1, POP - 2, REMOVE - 3, QUICKSORT - 4, INSERTION SORT - 5, COMPARE SORT - 6, PRINT - 7, CLEAR - 8, RANDOM - 9, EXIT - any other" << endl;
		if (cin >> choise)
			if (choise == 0)
				ReadFile(filename, q);
			else if (choise == 1)
			{
				cout << "Enter data: " << endl;
				if (cin >> data)
					push(data, q);
				else break;
			}
			else if (choise == 2)
			{
				cout << pop(q) << endl;
			}
			else if (choise == 3)
			{
				cout << "Enter data: " << endl;
				if (cin >> data)
					remove(data, q);
				else break;
			}
			else if (choise == 4)
			{
				Queue *tail = get_tail(q);
				t = clock();
				quicksort(q, tail);
				t = clock() - t;

				print(q);
				cout << "Quicksort took " << (double)t / CLOCKS_PER_SEC << endl;
			}

			else if (choise == 5)
			{
				t = clock();
				insertion_sort(q);
				t = clock() - t;

				print(q);
				cout << "Insertion sort took " << (double)t / CLOCKS_PER_SEC << " seconds." << endl;

			}
			else if (choise == 6)
			{
				Queue *p = copy(q);
				print(p);

				cout << endl;

				t = clock();
				insertion_sort(p);
				t = clock() - t;
				print(p);
				cout << "Insertion sort took " << (double)t / CLOCKS_PER_SEC << " seconds." << endl;

				cout << endl;

				Queue *tail = get_tail(q);
				t = clock();
				quicksort(q, tail);
				t = clock() - t;
				print(q);
				cout << "Quicksort took " << (double)t / CLOCKS_PER_SEC << endl;
			}
			else if (choise == 7)
				print(q);
			else if (choise == 8)
			{
				clear(q);
				q = nullptr;
			}
			else if (choise == 9)
			{
				if (q)
					clear(q);
				srand(time(0));
				cout << "Enter N: " << endl;
				if (cin >> N)
					for (int i = 0; i < N; i++)
						push(rand(), q);
				
			}
			else break;


		cin.ignore(cin.rdbuf()->in_avail());
	}
	
	system("pause");
	return 0;
}