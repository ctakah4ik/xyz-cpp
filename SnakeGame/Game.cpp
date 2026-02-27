#include "Game.h"
#include <cassert>
#include <string>

namespace SnakeGame
{
	static void LoadTexture(sf::Texture& texture, const std::string& file)
	{
		bool ok = texture.loadFromFile(RESOURCES_PATH + file);
		assert(ok && "Failed to load texture");
	}

	static void LoadSFX(sf::SoundBuffer& buffer, sf::Sound& sound, const std::string& file)
	{
		bool ok = buffer.loadFromFile(RESOURCES_PATH + file);
		assert(ok && "Failed to load sound");
		sound.setBuffer(buffer);
	}

	void PlaySFX(const Game& game, sf::Sound& sound)
	{
		if (game.soundEnabled)
			sound.play();
	}

	void StartMusic(Game& game)
	{
		if (game.musicEnabled)
		{
			game.sessionMusic.setLoop(true);
			game.sessionMusic.play();
		}
	}

	void PauseMusic(Game& game)
	{
		if (game.sessionMusic.getStatus() == sf::SoundSource::Playing)
			game.sessionMusic.pause();
	}

	void StopMusic(Game& game)
	{
		game.sessionMusic.stop();
	}

	void InitGame(Game& game)
	{
		bool fontOk = game.font.loadFromFile(RESOURCES_PATH + FONT_FILE);
		assert(fontOk && "Failed to load font");

		// Load sounds
		LoadSFX(game.buttonBuffer, game.buttonSound, SFX_BUTTON);
		LoadSFX(game.eatBuffer,    game.eatSound,    SFX_EAT);
		LoadSFX(game.startBuffer,  game.startSound,  SFX_START);
		LoadSFX(game.finishBuffer, game.finishSound, SFX_FINISH);
		bool musicOk = game.sessionMusic.openFromFile(RESOURCES_PATH + MUSIC_SESSION);
		assert(musicOk && "Failed to load music");

		// Load textures
		LoadTexture(game.appleTexture, APPLE_TEXTURE);
		LoadTexture(game.headTextures[(int)Direction::Right], HEAD_RIGHT_TEXTURE);
		LoadTexture(game.headTextures[(int)Direction::Up], HEAD_UP_TEXTURE);
		LoadTexture(game.headTextures[(int)Direction::Left], HEAD_LEFT_TEXTURE);
		LoadTexture(game.headTextures[(int)Direction::Down], HEAD_DOWN_TEXTURE);
		LoadTexture(game.bodyHorizontal, BODY_HORIZONTAL_TEXTURE);
		LoadTexture(game.bodyVertical, BODY_VERTICAL_TEXTURE);
		LoadTexture(game.bodyTopLeft, BODY_TOPLEFT_TEXTURE);
		LoadTexture(game.bodyTopRight, BODY_TOPRIGHT_TEXTURE);
		LoadTexture(game.bodyBottomLeft, BODY_BOTTOMLEFT_TEXTURE);
		LoadTexture(game.bodyBottomRight, BODY_BOTTOMRIGHT_TEXTURE);
		LoadTexture(game.tailTextures[(int)Direction::Right], TAIL_RIGHT_TEXTURE);
		LoadTexture(game.tailTextures[(int)Direction::Up], TAIL_UP_TEXTURE);
		LoadTexture(game.tailTextures[(int)Direction::Left], TAIL_LEFT_TEXTURE);
		LoadTexture(game.tailTextures[(int)Direction::Down], TAIL_DOWN_TEXTURE);

		game.background.setSize(sf::Vector2f((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
		game.background.setFillColor(sf::Color(30, 30, 30));

		InitLeaderboard(game.leaderboard);
		LoadLeaderboard(game.leaderboard);

		game.state = GameState::MainMenu;
		game.menuSelectedItem = 0;
		game.difficultyLevel = 0;
	}

	SnakeTextures BuildSnakeTextures(const Game& game)
	{
		SnakeTextures t;
		for (int i = 0; i < 4; ++i)
		{
			t.head[i] = &game.headTextures[i];
			t.tail[i] = &game.tailTextures[i];
		}
		t.bodyH = &game.bodyHorizontal;
		t.bodyV = &game.bodyVertical;
		t.bodyTL = &game.bodyTopLeft;
		t.bodyTR = &game.bodyTopRight;
		t.bodyBL = &game.bodyBottomLeft;
		t.bodyBR = &game.bodyBottomRight;
		return t;
	}

	void StartSession(Game& game)
	{
		StopMusic(game);
		float speed = DIFFICULTY_LEVELS[game.difficultyLevel].speed;
		InitSnake(game.snake, speed);
		SpawnApple(game.apple, game.snake);
		game.score = 0;
		game.countdownTimer = SESSION_START_DELAY;
		game.state = GameState::Countdown;
		PlaySFX(game, game.startSound);
	}

	static void OnSnakeDeath(Game& game)
	{
		game.snake.alive = false;
		game.nameEntryAsked = false;
		game.nameEntryChoice = 1; // default "No"
		StopMusic(game);
		PlaySFX(game, game.finishSound);
		game.state = GameState::GameOverPopup;
	}

	void UpdateGame(Game& game, float deltaTime)
	{
		if (game.state == GameState::Countdown)
		{
			game.countdownTimer -= deltaTime;
			if (game.countdownTimer <= 0.f)
			{
				game.state = GameState::Playing;
				StartMusic(game);
			}
			return;
		}

		if (game.state != GameState::Playing)
			return;

		Snake& snake = game.snake;
		if (snake.alive && snake.moveTimer + deltaTime >= snake.moveInterval)
		{
			Position2D nextHead = snake.segments[0].position;
			switch (snake.nextDirection)
			{
			case Direction::Right: 
				nextHead.x++; 
				break;
			case Direction::Left:  
				nextHead.x--; 
				break;
			case Direction::Up:    
				nextHead.y--; 
				break;
			case Direction::Down:  
				nextHead.y++; 
				break;
			}
			if (nextHead.x <= 0 || nextHead.x >= GRID_WIDTH - 1 ||
				nextHead.y <= 0 || nextHead.y >= GRID_HEIGHT - 1)
			{
				snake.moveTimer -= snake.moveInterval; // consume tick, don't move
				OnSnakeDeath(game);
				return;
			}
		}

		bool moved = UpdateSnake(game.snake, deltaTime);

		if (!moved)
			return;

		// Check self collision
		if (IsSnakeCollidingSelf(game.snake))
		{
			OnSnakeDeath(game);
			return;
		}

		// Check apple
		if (GetHead(game.snake).position == game.apple.position)
		{
			int points = DIFFICULTY_LEVELS[game.difficultyLevel].pointsPerApple;
			game.score += points;
			GrowSnake(game.snake, GROWTH_PER_APPLE);
			SpawnApple(game.apple, game.snake);
			PlaySFX(game, game.eatSound);
		}
	}
}
