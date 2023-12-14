#ifndef MEMORYVIEWER_H
#define MEMORYVIEWER_H 1

#include "objects.h"

// SVM == structVerticalMeter
typedef struct {
//struct SVM {
	unsigned long value, valuePrev;			// Real meter unit value
	float valueMin, valueMax;
	float width, height;
	float x, y, z;
	float color[4];
	float trimColor[4];
	sTextObject *label;		// Meter name
	sTextObject *svalue;		// Textual value
	sTextObject *svalueDiff;
	sTextObject *svalueMin;
	sTextObject *svalueMax;
//};
} VerticalMeter;
//typedef struct SVM VerticalMeter;

extern VerticalMeter ramMeter, swapMeter, usbMeter;//, loadMeter, swapMeter, highmemMeter;

void initMemoryViewer (void);
void showMem (void);

#endif /* MEMORYVIEWER_H */
