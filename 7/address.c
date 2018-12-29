#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CMD 20

struct process {
	int base;
	int limit;
	char pid [MAX_CMD];
	struct process * next;
};

/* a sequence of processes sorted by memory base */
struct process * head, * tail;

// functions defined for easy implementation
struct process * new_process (int base, int limit, char * pid, struct process * next);
struct process * first_fit (int memory);
struct process * best_fit (int memory);
struct process * worst_fit (int memory);

// allocator operations
bool request_memory (char * pid, int memory, char mode);
bool release_memory (char * pid);
void show_status (void);
void compaction (void);

// clear the allocated memory of the program
void clear_memory (void);

int main (int argc, char * argv[]) {
	int MAX;
	char cmd[MAX_CMD];
	char pid [MAX_CMD];
	int memory;
	char mode;

	/* 1. init */
	if (argc < 2) {
		printf ("[error] expecting 1 argument, get %d\n", argc-1);
		return 1;
	}
	
	MAX = atoi (argv[1]);
	printf ("initialization option: MAX = %d\n", MAX);
	tail = new_process (MAX, 0, "TAIL", NULL);
	head = new_process (0, 0, "HEAD", tail);

	/* 2. waiting for input */
	while (true) {
		printf ("allocator> ");
		scanf ("%s", cmd);
		//printf ("cmd=%s\n", cmd);
		if (strcmp (cmd, "X") == 0) {
			break;
		} else if (strcmp (cmd, "RQ") == 0) {
		/* memory request */
			scanf ("%s", pid);
			scanf ("%d", &memory);
			while ((mode = getchar()) == ' ')
				;
			//printf ("pid=%s memory=%d mode=%c\n", pid, memory, mode);
			if (request_memory (pid, memory, mode)) {
				printf ("allocation granted.\n");
			} else {
				printf ("allocation rejected.\n");
			}
		} else if (strcmp (cmd, "RL") == 0) {
		/* memory release */
			scanf ("%s", pid);
			if (release_memory (pid)) {
				printf ("release successful.\n");
			} else {
				printf ("release failed.\n");
			}
		} else if (strcmp (cmd, "STAT") == 0) {
		/* show status */
			show_status ();
		} else if (strcmp (cmd, "C") == 0) {
		/* compaction */
			compaction ();
			printf ("compaction done.\n");
		} else {
			printf ("[warning] unknown command %s\n", cmd);
		}
		while (getchar () != '\n')
			;

	}

	printf ("exiting allocator\n");
	clear_memory();

	return 0;
}

struct process * new_process (int base, int limit, char * pid, struct process * next) {
	struct process * ptr = (struct process *) malloc (sizeof (struct process));
	ptr->base = base;
	ptr->limit = limit;
	strcpy (ptr->pid, pid);
	ptr->next = next;
	return ptr;
}

struct process * first_fit (int memory) {
	struct process * ptr = head;
	int vol;
	while (ptr != tail) {
		vol = (ptr->next->base - (ptr->base + ptr->limit));
		if (memory > vol) {
		/* not fit */
			ptr = ptr->next;
		} else {
			return ptr;
		}
	}
	return NULL;
}

struct process * best_fit (int memory) {
	struct process * ptr = head;
	struct process * best_ptr = NULL;
	int vol, min_vol;
	min_vol = tail->base + 1; // unreachable in this case
	
	while (ptr != tail) {
		vol = (ptr->next->base - (ptr->base + ptr->limit));
		if (memory <= vol && vol < min_vol) {
			min_vol = vol;
			best_ptr = ptr;
		}
		ptr = ptr->next;
	}
	
	return best_ptr;
}

struct process * worst_fit (int memory) {
	struct process * ptr = head;
	struct process * worst_ptr = NULL;
	int vol, worst_vol;
	worst_vol = -1;

	while (ptr != tail) {
		vol = (ptr->next->base - (ptr->base + ptr->limit));
		if (vol > worst_vol) {
			worst_vol = vol;
			worst_ptr = ptr;
		}
		ptr = ptr->next;
	}
	
	if (memory <= worst_vol)
		return worst_ptr;
	else
		return NULL;
}

bool request_memory (char * pid, int memory, char mode) {
	struct process * ptr;
	
	switch (mode) {
		case 'F':
			ptr = first_fit (memory);
			break;
		case 'B':
			ptr = best_fit (memory);
			break;
		case 'W':
			ptr = worst_fit (memory);
			break;
		default:
			printf ("[warning] invalid allocation mode %c\n", mode);
			return false;
	}
	
	if (ptr == NULL) {
		printf ("[warning] no available memory partition found.\n");
		return false;
	}

	ptr->next = new_process (ptr->base + ptr->limit, memory, pid, ptr->next);
	
	return true;
}

bool release_memory (char * pid) {
	struct process * ptr = head->next;
	struct process * prev = head;
	bool flag = false; // true if at least one found

	while (ptr != tail) {
		if (strcmp (pid, ptr->pid) == 0) {
		/* process found */
			prev->next = ptr->next; // remove partition
			free (ptr); // release allocated memory
			ptr = prev->next; // note prev need not be changed
			flag = true; // set flag because found
		} else {
		/* not this process, try next */
			prev = ptr;
			ptr = ptr->next;
		}
	}

	return flag;
}

void show_status (void) {
	struct process * ptr = head->next;
	int start = 0;
	while (ptr != NULL) {
		if (ptr->base != start) {
		/* not adjacent partitions, exist unused memory */
			printf ("Addresses [%d:%d] Unused\n", start, ptr->base - 1);
		}
		if (ptr->limit != 0) {
		/* ptr->limit != 0 means this is not TAIL process */
			start = ptr->base + ptr->limit - 1;
			printf ("Addresses [%d:%d] Process %s\n", ptr->base, start, ptr->pid);
			++ start; // increment start, because [base : base+limit-1] is allocated to this process, plus one enters the next partition
		}
		ptr = ptr->next;
	}
}

void compaction (void) {
	/* a very convenient implementation because of the data structure I use */
	struct process * ptr = head;
	
	while (ptr->next != tail) {/* stop one process before TAIL */
	/* set the following process, set base = ptr->base + ptr->limit */
		ptr->next->base = ptr->base + ptr->limit;
		ptr = ptr->next;
	}
}

void clear_memory (void) {
	struct process * ptr = head;
	struct process * tmp;
	while (ptr != NULL) {
		tmp = ptr;
		ptr = ptr->next;
		free (tmp);
	}
}