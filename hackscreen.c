#include <stdio.h>
#include <ncurses.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>


struct column {
	char *text;
	bool is_zero;
};

struct column initialize_column(struct column col);
struct column update_column(struct column col, int index);
void print_column(struct column col, int x, WINDOW *screen);

char key;
int max_x, max_y;
bool running;

int main() {
	initscr();
	start_color();
	timeout(100);
	cbreak();
	noecho();
	noqiflush();
	keypad(stdscr, true);


	getmaxyx(stdscr, max_y, max_x);
	running = true;
	
	struct column columns[max_x];

	// Set every column with random values
	for (int i = 0; i < max_x; i++) {
		columns[i] = initialize_column(columns[i]);
	}

	// Application Loop
	while (running) {
		// Set the color to green
		init_pair(1, COLOR_GREEN, COLOR_BLACK);
		attron(COLOR_PAIR(1));

		for (int i = 0; i < max_x; i++) {
			print_column(columns[i], i, stdscr);
			refresh();
			columns[i] = update_column(columns[i], max_y - 1);
		}

		key = getch();
		if (key == ERR){
			continue;
		} else if (key == 'x') {
			attroff(COLOR_PAIR(1));
			endwin();
			running = false;
			break;
		}
	}
	return 0;
}

struct column initialize_column(struct column col) {
	col.text = malloc(sizeof(char) * max_y);
	col.text[0] = rand() % 2 == 0 ? '0' : '1';
	col.is_zero = rand() % 2 == 0;

	for (int i = 1; i < max_y; i++) {
		if (col.text[i - 1] == '0') {
			col.text[i] = rand() % 16 == 0 ? '1' : '0';
		} else if (col.text[i - 1] == '1') {
			col.text[i] = rand() % 16 == 0 ? col.is_zero ? '0' : ' ' : '1';
		} else {
			col.text[i] = rand() % 16 == 0 ? '1' : ' ';
		}
	}
	return col;
}
		
struct column update_column(struct column col, int index) {
	if (index != 0) {
		col.text[index] = col.text[index - 1];
		return update_column(col, index - 1);
	} else {
		if (col.text[index + 1] == '0') {
			col.text[index] = rand() % 16 == 0 ? '1' : '0';
			col.is_zero = !col.is_zero;
		} else if (col.text[index + 1] == '1') {
			col.text[index] = rand() % 16 == 0 ? col.is_zero ? '0' : ' ' : '1';
		} else {
			col.text[index] = rand() % 16 == 0 ? '1' : ' ';
			col.is_zero = !col.is_zero;
		}
		return(col);
	}
}

void print_column(struct column col, int x, WINDOW *screen) {
	for (int i = 0; i < max_y; i++) {
		mvwprintw(screen, i, x, "%c", col.text[i]);
	}
}
