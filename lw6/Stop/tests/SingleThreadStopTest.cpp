#include "../StopSource.h"
#include "../StopToken.h"
#include <gtest/gtest.h>

TEST(StopToken, DefaultTokenDoesNotRequestStop)
{
	const StopToken token;

	EXPECT_FALSE(token.StopRequested());
}

TEST(StopSource, NewTokenDoesNotRequestStop)
{
	const StopSource source;
	const StopToken token = source.GetToken();

	EXPECT_FALSE(token.StopRequested());
}

TEST(StopSource, RequestStopChangesTokenState)
{
	const StopSource source;
	const StopToken token = source.GetToken();

	source.RequestStop();

	EXPECT_TRUE(token.StopRequested());
}

TEST(StopSource, SeveralTokensShareSameState)
{
	const StopSource source;

	const StopToken token1 = source.GetToken();
	const StopToken token2 = source.GetToken();

	EXPECT_FALSE(token1.StopRequested());
	EXPECT_FALSE(token2.StopRequested());

	source.RequestStop();

	EXPECT_TRUE(token1.StopRequested());
	EXPECT_TRUE(token2.StopRequested());
}

TEST(StopToken, CopiedTokenSharesSameState)
{
	const StopSource source;

	const StopToken token1 = source.GetToken();
	const StopToken& token2 = token1;

	source.RequestStop();

	EXPECT_TRUE(token1.StopRequested());
	EXPECT_TRUE(token2.StopRequested());
}

TEST(StopToken, MovedTokenKeepsState)
{
	const StopSource source;

	StopToken token1 = source.GetToken();
	const StopToken token2 = std::move(token1);

	source.RequestStop();

	EXPECT_TRUE(token2.StopRequested());
}

TEST(StopToken, TokenLivesLongerThanSource)
{
	StopToken token;

	{
		const StopSource source;
		token = source.GetToken();

		EXPECT_FALSE(token.StopRequested());

		source.RequestStop();

		EXPECT_TRUE(token.StopRequested());
	}

	EXPECT_TRUE(token.StopRequested());
}