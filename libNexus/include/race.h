
struct Car {
	char *name;
	float speed;
	short rpm;
	short hp;
	short weight;
	float posx, posz;
};

extern struct Car firstCar;

void moveCar (void);

