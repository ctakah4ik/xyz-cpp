#pragma once

namespace SnakeGame
{
	struct Vector2D
	{
		int x = 0;
		int y = 0;

		bool operator==(const Vector2D& other) const
		{
			return x == other.x && y == other.y;
		}

		bool operator!=(const Vector2D& other) const
		{
			return !(*this == other);
		}
	};

	typedef Vector2D Position2D;

}
