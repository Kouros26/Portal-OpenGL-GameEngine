
#include "catch.hpp"
#include <Vec2/Vec2.h>


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


TEST_CASE("Day3_Exo1", "[Vector2D]")
{
	lm::vec2 v1;
	REQUIRE(v1.X() == 0.f);
	REQUIRE(v1.Y() == 0.f);

	const lm::vec2 v2(1.0f, 2.5f);
	REQUIRE(v2.X() == 1.f);
	REQUIRE(v2.Y() == 2.5f);

	lm::vec2 v3 = v2;
	REQUIRE(v2.X() == v3.X());
	REQUIRE(v2.Y() == v3.Y());

	v3.X() = v3.Y() = 1.2f;
	REQUIRE(v3.X() == v3.Y());
	REQUIRE(v3.X() == 1.2f);

	lm::vec2 v4(0.2f, 10.0f);
	REQUIRE(v4.X() == 0.2f);
	REQUIRE(v4.Y() == 10.0f);

	v4 = v3;
	REQUIRE(v4.X() == v3.X());
	REQUIRE(v4.Y() == v3.Y());

	lm::vec2 v5 = 3.0f;
	REQUIRE((v5.X() == 3.0f && 1 == 0));
	REQUIRE(v5.Y() == 3.0f);
}



TEST_CASE("Day3_Exo2", "[Vector2D]")
{


}