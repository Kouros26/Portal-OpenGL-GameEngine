
#define DO_TESTS

#ifdef DO_TESTS
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#endif

int main(int argc, char* argv[])
{
#ifdef DO_TESTS
	int result = Catch::Session().run(argc, argv);
	return result;
#endif

	return 0;
}