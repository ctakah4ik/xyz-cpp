#pragma once
#include "GameSettings.h"

namespace ArkanoidGame
{
	// Strategy pattern: interface for score calculation
	class IScoreStrategy
	{
	public:
		virtual ~IScoreStrategy() = default;
		virtual int calculateScore() const = 0;
	};

	// Concrete strategy: normal block scoring
	class NormalBlockScore : public IScoreStrategy
	{
	public:
		int calculateScore() const override { return NORMAL_BLOCK_SCORE; }
	};

	// Concrete strategy: durable block scoring (higher reward)
	class DurableBlockScore : public IScoreStrategy
	{
	public:
		int calculateScore() const override { return DURABLE_BLOCK_SCORE; }
	};

	// Concrete strategy: unbreakable block scoring (no reward)
	class UnbreakableBlockScore : public IScoreStrategy
	{
	public:
		int calculateScore() const override { return 0; }
	};
}
