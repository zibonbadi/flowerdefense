/* Hardcoded constants */
#define SCREEN_W 640
#define SCREEN_H 400
#define TITLE "Blub engine"

/* API keywords: structs, enums, etc. */
#define Z_PLANE_LOCKED 0b10000000
#define Z_PLANE_LOOP_X 0b00000001
#define Z_PLANE_LOOP_Y 0b00000010
#define Z_PLANE_ALL 0b01111010
#define Z_PLANE_A 0b00001010
#define Z_PLANE_B 0b00010010
#define Z_PLANE_C 0b00100010
#define Z_PLANE_D 0b01000010

/*
typedef struct {
	float x, y, w, h, angle;
} Z_Transform;
*/

enum exception{
	Z_EXCEPTION,
	Z_INIT_EXCEPTION,
	Z_ENGINE_EXCEPTION,
	Z_GAME_EXCEPTION
};

enum tilebound{
	Z_TILEBOUND_OUTER_RIGHT = 0b00000001,
	Z_TILEBOUND_OUTER_UP = 0b00000010,
	Z_TILEBOUND_OUTER_LEFT = 0b00000100,
	Z_TILEBOUND_OUTER_DOWN = 0b00001000,
	Z_TILEBOUND_INNER_RIGHT = 0b00010000,
	Z_TILEBOUND_INNER_UP = 0b00100000,
	Z_TILEBOUND_INNER_LEFT = 0b01000000,
	Z_TILEBOUND_INNER_DOWN = 0b10000000,
};

enum imgfile {
	Z_IMGFILE_RAW,
	Z_IMGFILE_BMP,
	Z_IMGFILE_PNG,
	Z_IMGFILE_GIF,
	Z_IMGFILE_TIFF
};
