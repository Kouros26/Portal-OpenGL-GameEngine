#ifdef _DEBUG
#define ASSERT(cond)\
if (!cond)\
	__debugbreak();
#else
#define ASSERT(cond)\
if (!cond)\
	exit(EXIT_FAILURE);
#endif