#include <gtest/gtest.h>

#include "pkg/pkg.h"

TEST( TemplateTest, Test1 )
{
	EXPECT_EQ( pkg::getFibonacci( 5 ), 5 );
}
