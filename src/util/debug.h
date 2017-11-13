#include <cassert>
#ifdef DEBUG
#define STRING(X) #X
#define TOSTRING(X) STRING(X)
#define GLCHECK glcheck("@" __FILE__ ":" TOSTRING(__LINE__));
void glcheck(const char* loc);
#else
#define GLCHECK
#endif
