/* File handling any textual forms of data, switching on objects type
to parse the stream as source code manip, system commands covering,
internal libNexus functions access...
*/

#define DATA_UNKNOWN			0x00
#define DATA_TYPE_FILE			0x01
#define DATA_TYPE_HISTORY		0x02

/* data buffer holding file data to parse */
typedef struct LexicalNodeData {
	struct LexicalNode *prev, *next;
	int dataType;
	struct BufferTree *bufferTree;	// node's data
} sLexicalNodeData;

/* The parser will execute appropriate function for characters like !*{}[]=-+;,.<> and such */
typedef struct LexicalKeychar {
	char c;
	void (*func)(void);
} sLexicalKeychar;

typedef struct LexicalKeyword {
	struct LexicalKeyword *prev, *next;
	char *name;
} sLexicalKeyword;

typedef struct LexicalLine {
	struct LexicalLine *prev, *next;
	struct LexicalKeyword *firstKeyword, *lastKeyword;
} sLexicalLine;

typedef struct LexicalSheet  {
	struct LexicalSheet *prev, *next;
	struct LexicalLine *firstLine, *lastLine;
} sLexicalSheet;

void NLinitLexic (void);
