#include <iostream>

/////////// SORT AND SWEE

struct BoundingBox {
	int id;
	float x1; // x1 < x2
	float x2;
};

enum IntervalType {
	BEGIN, END
};

struct IntervalLimit {
	IntervalType type;
	int box;
	float* value; // <-- pointer so when we change the box we have the value updated
};

BoundingBox boxes[] = {
	 BoundingBox{0, 0.0f, 1.0f},
	 BoundingBox{1, 0.8f, 1.3f},
	 BoundingBox{2, 1.2f, 2.0f},
	 BoundingBox{3, 3.0f, 3.7f},
	 BoundingBox{4, 3.5f, 4.0f}
};


void printBoxes() {
	for (int i = 0; i < 5; i++) {
		std::cout << "B" << i << ": ";
		for (int j = 0; j < 50; j++) {
			float pos = j / 10.f;
			if (pos < boxes[i].x1 || pos > boxes[i].x2) {
				std::cout << " ";
			}
			else {
				std::cout << "#";
			}
		}
		std::cout << std::endl;
	}
}

void initIntervals(IntervalLimit intervalsLimits[]) {
	for (int i = 0; i < 5; i++) { // FOR EACH BOX...
		intervalsLimits[2 * i] = IntervalLimit{ BEGIN, i, &(boxes[i].x1) };
		intervalsLimits[2 * i + 1] = IntervalLimit{ END, i, &(boxes[i].x2) };
	}
}

void sort(IntervalLimit list[], int n) {
	// Insertion sort
}

void sweep(IntervalLimit list[], int n) {
	bool active[5] = { false, false, false, false, false };

	// activar caixa 3 --> active[3] = true;
	// implementar sweep
	// print collisions
}

void checkPotentialCollisions() {
	IntervalLimit intervalsLimits[2 * 5];
	initIntervals(intervalsLimits);
	// sort
	sort(intervalsLimits, 10);
	// sweep
	sweep(intervalsLimits, 10);
}

void SortAndSweep() {
	checkPotentialCollisions();
	printBoxes();
}