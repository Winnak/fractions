/**
 * MIT LICENSE
 *
 * Copyright 2018 Erik Høyrup Jørgensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "../fractions.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE( "Factions are created", "[creation]" ) {
    CHECK( Fraction<int8_t, int8_t>{1, 2}.float32() == 0.5f );
    CHECK( Fraction<int16_t, int16_t>{1, 2}.float32() == 0.5f );
    CHECK( Fraction<int64_t, int64_t>{1, 2}.float32() == 0.5f );
    CHECK( Fraction<int32_t, int32_t>{1, 2}.float32() == 0.5f );

    CHECK( Fraction<int8_t, int8_t>{3, 8}.float32() == 0.375f );
    CHECK( Fraction<int16_t, int16_t>{2, 4}.float32() == 0.5f );
    CHECK( Fraction<int64_t, int64_t>{9, 1}.float32() == 9.0f );
    CHECK( Fraction<int32_t, int32_t>{0, 1}.float32() == 0.0f );

    CHECK( frac8{6, 8}.float64() == 0.75 );
    CHECK( frac16{3, 4}.float64() == 0.75 );
    CHECK( frac32{9, 2}.float64() == 4.5 );
    CHECK( frac64{0, 100}.float64() == 0 );
}


TEST_CASE( "Fractions are reduced", "[reduction]" ) {
    frac64 a{12, 6};
    a.reduce();
    CHECK( a.float64() == 2 );

    frac8 b{-12, 6};
    b.reduce();
    CHECK( b.float64() == -2 );

    CHECK( frac8::reduce({2, 8}).float64() == 0.25 );
}

TEST_CASE( "Fractions can do arithmetic", "[arithmetic]" ) {
    CHECK( frac8::add({2, 4}, {1, 4}).float32() == 0.75f );
    CHECK( frac16::sub({2, 4}, {1, 4}).float32() == 0.25f );
    CHECK( frac32::mul({1, 2}, {1, 4}).float32() == 0.125f );
    CHECK( frac64::div({1, 2}, {1, 4}).float64() == 2.0 );
}

TEST_CASE( "Fractions have overloaded operators", "[arithmetic]" ) {
    CHECK( (frac8{2, 4} + frac8{1, 4}).float32() == 0.75f );
    CHECK( (frac16{2, 4} - frac16{1, 4}).float32() == 0.25f );
    CHECK( (frac32{1, 2} * frac32{1, 4}).float32() == 0.125f );
    CHECK( (frac64{1, 2} / frac64{1, 4}).float64() == 2.0 );

    CHECK( (frac8{1, 3} == frac8{2, 3}) == false );
    CHECK( (frac8{1, 3} == frac8{1, 3}) == true );
    CHECK( (frac8{1, 3} == frac8{2, 6}) == true );

    CHECK( ((-frac32{1, 3}) == frac32{-1, 3}) == true );
    CHECK( ((-frac32{1, 3}) == frac32{1, -3}) == true );

    CHECK( (frac16{2, 9} != frac16{2, 9}) == false );
    CHECK( (frac16{2, 9} != frac16{1, 3}) == true );
    CHECK( (frac16{2, 9} != frac16{4, 18}) == false );

    CHECK( (frac32{2, 9} > frac32{2, 9}) == false );
    CHECK( (frac32{2, 9} >= frac32{2, 9}) == true );
    CHECK( (frac32{2, 9} > frac32{1, 9}) == true );
    CHECK( (frac32{2, 9} > frac32{3, 9}) == false );

    CHECK( (frac64{2, 9} < frac64{2, 9}) == false );
    CHECK( (frac64{2, 9} <= frac64{2, 9}) == true );
    CHECK( (frac64{2, 9} < frac64{1, 9}) == false );
    CHECK( (frac64{2, 9} < frac64{3, 9}) == true );


    frac32 a{3, 4};
    REQUIRE( (a += {1, 2}).float32() == 5/4.0f );
    REQUIRE( (a -= {1, 2}).float32() == 3/4.0f );
    REQUIRE( (a *= {1, 2}).float32() == 3/8.0f );
    REQUIRE( (a /= {1, 2}).float32() == 3/4.0f );
    REQUIRE( (++a).float32() == 7/4.0f );
    REQUIRE( (--a).float32() == -1/4.0f );

    // unary test:
    frac64 b{12, 6};
    CHECK( (-b).float64() == -2 );
    CHECK( b.float64() == 2 );
}

TEST_CASE( "Fractions handle 0", "[zero]" ) {
    CHECK( (frac32{2, 9} == frac32{0, 0}) == false );
    CHECK( (frac32{0, 0} == frac32{0, 0}) == true );
    CHECK( (frac32{0, 0} != frac32{0, 0}) == false );
    CHECK( (frac32{0, 0} != frac32{2, 9}) == true );
    CHECK( (frac32{0, 0} <  frac32{1, 2}) == true );
    CHECK( (frac32{1, 2} <  frac32{0, 0}) == false );
    CHECK( (frac32{0, 0} <  frac32{0, 0}) == false );
    CHECK( (frac32{0, 0} <= frac32{1, 2}) == true );
    CHECK( (frac32{1, 2} <= frac32{0, 0}) == false );
    CHECK( (frac32{0, 0} <= frac32{0, 0}) == true );
    CHECK( (frac32{0, 0} >  frac32{1, 2}) == false );
    CHECK( (frac32{1, 2} >  frac32{0, 0}) == true );
    CHECK( (frac32{0, 0} >  frac32{0, 0}) == false );
    CHECK( (frac32{0, 0} >= frac32{1, 2}) == false );
    CHECK( (frac32{1, 2} >= frac32{0, 0}) == true );
    CHECK( (frac32{0, 0} >= frac32{0, 0}) == true );
}

TEST_CASE( "Fractions supports arithmetic with other data types", "[other_data]" ) {
    CHECK( (frac32{1, 2} * 3).float32() == 1.5f );
    CHECK( (frac32{1, 2} + 2).float32() == 2.5f );
    CHECK( (frac32{1, 2} - 3).float32() == -2.5f );
    CHECK( (frac32{1, 2} / 2).float32() == 0.25f );
}

TEST_CASE( "Fractions can be created from floats" "[from_float]" ) {
    CHECK( (frac8::from_float(0.5f) == frac8{1, 2}) == true );
    CHECK( (frac8::from_float(0.5f)).float32() == 0.5f );
    CHECK( (frac16::from_float(0.25f) == frac16{1, 4}) == true );
    CHECK( (frac32::from_float(1.25f) == frac32{5, 4}) == true );
    CHECK( (frac64::from_float(1.555f) == frac64{311, 200}) == true );

    CHECK( (frac32::from_float(1.75) == frac32{7, 4}) == true);
    CHECK( (frac32::from_float(0.5f) == frac32{1, 2}) == true);
    CHECK( (frac32::from_float(0.0f) == frac32{0, 1}) == true);
    CHECK( (frac32::from_float(-1.0) == frac32{-1, 1}) == true);
    CHECK( (frac32::from_float(1.0f) == frac32{1, 1}) == true);
    CHECK( (frac32::from_float(2.0) == frac32{2, 1}) == true);
    CHECK( (frac32::from_float(-0.2f) == frac32{-1, 5}) == true);

}