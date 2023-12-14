#include <stdio.h>

#include "libNexus.h"

const char *INVENTORY_VERSION = "0.0.0.1";

int main (int argc, char **argv) {
	printf ("Inventory v%s\n", INVENTORY_VERSION);

	NLinit (&argc, argv, "Inventory");

	return 0;
}

