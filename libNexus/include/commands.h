
extern int drawingPrompt;
struct Prompt {
	int drawn;
	char *text;
	char *buffer;
	float cursorPosition;
	float x, y;
	float sizeX, sizeY;
};
extern struct Prompt prompt, *drawnPrompt;

void initCommands (void);
char *prompt2 (char *text);
void parsePrompt (unsigned char key);

