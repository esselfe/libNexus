
VerticalMeter2 {
	char *label;	/* Meter name */
	char *svalue;	/* Textual value */
	char *svalueMin, *svalueMax;
	float value;	/* Real meter unit value */
	float valueMin, valueMax;
	float width, height;
	float x, y, z;
	float color[4];
	float trimColor[4];
};

ramMeter.svalue = malloc (40);
	sprintf (ramMeter.svalue, "%f", ramMeter.valueMin);
	ramMeter.svalueMin = malloc (40);
	sprintf (ramMeter.svalueMin, "%f", ramMeter.valueMin);
	ramMeter.svalueMax = malloc (40);
	sprintf (ramMeter.svalueMax, "%f", ramMeter.valueMin);
	 
	loadMeter.svalue = malloc (40);
	sprintf (loadMeter.svalue, "%f", ramMeter.valueMin);
	loadMeter.svalueMin = malloc (40);
	sprintf (loadMeter.svalueMin, "%f", ramMeter.valueMin);
	loadMeter.svalueMax = malloc (40);
	sprintf (loadMeter.svalueMax, "%f", ramMeter.valueMin);
	
	swapMeter.svalue = malloc (40);
	sprintf (swapMeter.svalue, "%f", ramMeter.valueMin);
	swapMeter.svalueMin = malloc (40);
	sprintf (swapMeter.svalueMin, "%f", swapMeter.valueMin);
	swapMeter.svalueMax = malloc (40);
	sprintf (swapMeter.svalueMax, "%f", ramMeter.valueMin);
	
	highmemMeter.svalue = malloc (40);
	sprintf (highmemMeter.svalue, "%f", ramMeter.valueMin);
	highmemMeter.svalueMin = malloc (40);
	sprintf (highmemMeter.svalueMin, "%f", highmemMeter.valueMin);
	highmemMeter.svalueMax = malloc (40);
	sprintf (highmemMeter.svalueMax, "%f", ramMeter.valueMin);
	