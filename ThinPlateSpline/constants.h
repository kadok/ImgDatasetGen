#ifndef CONSTANTS
#define CONSTANTS

static const int N_SAMPLES_1D = 10;

#ifndef PI
static const double PI = 3.14159265;
#endif

#ifndef PIover180
static const double PIover180 = 3.14159265/180;
#endif

static const float C_SCALE_MIN = 0.9;
static const float C_SCALE_MAX = 1.1;

//in grades:
static const int C_ROT_MIN = -15;
static const int C_ROT_MAX = 15;

static const int C_TRANS_MIN = -10;
static const int C_TRANS_MAX = 10;

static const int C_NLAYERS = 3;

#endif // CONSTANTS
