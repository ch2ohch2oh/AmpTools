#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ComplexMatrix.h"

TEST_CASE("ComplexMatrix") {
    ComplexMatrix PauliX(2, 2);
    ComplexMatrix PauliY(2, 2);
    ComplexMatrix PauliZ(2, 2);
    ComplexMatrix One(2, 2);
    ComplexMatrix Zero(2, 2);

    PauliX.set(0, 1, 1).set(1, 0, 1);
    PauliY.set(0, 1, TComplex(0, -1)).set(1, 0, TComplex(0, 1));
    PauliZ.set(0, 0, 1).set(1, 1, -1);
    One.set(0, 0, 1).set(1, 1, 1);

    SECTION("One == One") {
        REQUIRE(One == One);
    }

    SECTION("One != Zero") {
        REQUIRE(One != Zero);
    }

    SECTION("One - One == Zero") {
        REQUIRE(One - One == Zero);
    }

    SECTION("One + Zero == Zero") {
        REQUIRE(One + Zero == One);
    }

    SECTION("Squared Pauli matrices equal to one") {
        REQUIRE(PauliX * PauliX == One);
        REQUIRE(PauliY * PauliY == One);
        REQUIRE(PauliZ * PauliZ == One);
    }

    SECTION("Multiply and divide by the same scalar") {
        TComplex scalar(233, 666);
        REQUIRE(PauliX * scalar / scalar == PauliX);
    }

    SECTION("Multiply by zero") {
        TComplex scalar(0);
        REQUIRE(PauliX * scalar == Zero);
    }

    SECTION("Lie algebra of Pauli matrices") {
        REQUIRE(PauliX * PauliY - PauliY * PauliX == TComplex(0, 2) * PauliZ);
    }

    SECTION("Close to zero") {
        REQUIRE(PauliX * TComplex(1e-7) != Zero);
    }
}