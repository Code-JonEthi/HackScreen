#include <ncurses.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>


struct column {
	char *text;
	bool is_zero;
};

struct column initialize_column(struct column col);
struct column update_column(struct column col, int index);
void print_column(struct column col, int x, WINDOW *screen);
void free_columns(struct column columns[]);

char key;
int max_x, max_y;
int width;
int height;
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
	width = ceil(max_x / 2);
	height = max_y;
	running = true;
	int x;
	
	struct column columns[width];

	// Set every column with random values
	for (int i = 0; i < width; i++) {
		columns[i] = initialize_column(columns[i]);
	}

	// Application Loop
	while (running) {
		// Set the color to green
		init_pair(1, COLOR_GREEN, COLOR_BLACK);
		attron(COLOR_PAIR(1));
	
		x = 1;
		for (int i = 0; i < width; i++) {
			print_column(columns[i], x, stdscr);
			refresh();
			columns[i] = update_column(columns[i], height - 1);
			x += 2;
		}

		key = getch();
		if (key == ERR){
			continue;
		} else if (key == 'x') {
			attroff(COLOR_PAIR(1));
			endwin();
			free_columns(columns);
			running = false;
			break;
		}
	}
	return 0;
}

struct column initialize_column(struct column col) {
	col.text = malloc(sizeof(char) * height);
	col.text[0] = rand() % 2 == 0 ? '0' : '1';
	col.is_zero = rand() % 2 == 0;

	for (int i = 1; i < height; i++) {
		if (col.text[i - 1] == '0') {
			col.text[i] = rand() % 10 == 0 ? '1' : '0';
		} else if (col.text[i - 1] == '1') {
			col.text[i] = rand() % 10 == 0 ? col.is_zero ? '0' : ' ' : '1';
		} else {
			col.text[i] = rand() % 10 == 0 ? '1' : ' ';
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
			col.text[index] = rand() % 10 == 0 ? '1' : '0';
			col.is_zero = !col.is_zero;
		} else if (col.text[index + 1] == '1') {
			col.text[index] = rand() % 10 == 0 ? col.is_zero ? '0' : ' ' : '1';
		} else {
			col.text[index] = rand() % 10 == 0 ? '1' : ' ';
			col.is_zero = !col.is_zero;
		}
		return(col);
	}
}

void print_column(struct column col, int x, WINDOW *screen) {
	for (int i = 0; i < height; i++) {
		mvwprintw(screen, i, x, "%c", col.text[i]);
	}
}

void free_columns(struct column columns[]) {
	for (int i = 0; i < width; i++) {
		free(columns[i].text);
	}
}

