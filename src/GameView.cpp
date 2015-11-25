#include "GameView.hpp"

namespace lava
{
	GameView::GameView(sf::RenderWindow* window, Level* level, Player* player, sf::View view, sf::Texture *lavaTexture, sf::Texture *backgroundTexture, lava::eventManager *manager) :
	isWait(false),
	isPlaying(false),
	isGameover(false)
	//lava(sf::Vector2f(2400, 2000))
	{
		this->window = window;
		this->level = level;
		this->player = player;
        this->view = view;
		this->manager = manager;
		background.setTexture(*backgroundTexture);

		lavaSprite.setTexture(*lavaTexture);
		lavaSprite.setTextureRect(sf::IntRect(0, 0, 2400, 2000));
		lavaSprite.setScale(1, 1.5f);

		font.loadFromFile("pixel_font.ttf");
		text.setFont(font);
		text.setColor(sf::Color::White);
		text.setString("0");
		text.setCharacterSize(50);
		text.setPosition(sf::Vector2f(300, 200));
		EventDelegate example(std::bind(&lava::GameView::respond, this, std::placeholders::_1), (int)this);
		this->manager->registerEvent(example, gameOver);

    }

    GameGUI gameGUI(800, 600);
    
    void GameView::setStart()
    {
        gameGUI.draw(window);
    }
    
    void GameView::setInstructionMessage()
    {
        sf::Texture texture;
        texture.loadFromFile("graph/instruction.jpg");
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, window->getSize().x, window->getSize().y));
        window->draw(sprite);
    }
    
    void GameView::setInstruction()
    {
        setInstructionMessage();
	}
    
    void GameView::setPauseMessage()
    {
        sf::Text pauseMessage("          PAUSE\n\n\npress P to continue", gameGUI.font, 30);
        pauseMessage.setPosition(300, 200);
        pauseMessage.setColor(sf::Color::Red);
        window->draw(pauseMessage);
    }
    
    void GameView::setPause()
    {
        setPauseMessage();
    }
    
    void GameView::setGameoverMessage()
    {
        sf::Text gameoverMessage("      GAME OVER\n\npress [Enter] to restart\n    press Esc to quit", gameGUI.font, 30);

        gameoverMessage.setPosition(300, 200);
        gameoverMessage.setColor(sf::Color::Red);
        window->draw(gameoverMessage);
    }
    
    void GameView::setGameover()
    {
        setGameoverMessage();
    }

    void GameView::update(sf::Clock clock)
	{
        processInput(clock);
		sf::View view;
		view.reset(sf::FloatRect(0, 0, 800, 600));

        window->clear(sf::Color::Black);
        if(isPlaying)
        {
            if(isWait)
            {
				window->setView(view);
                setPause();
            }
            else
            {
                draw();
            }
        }
        else
        {
            if(isGameover)
            {
				window->setView(view);
                setGameover();
            }
            else
            {
				window->setView(view);
                setStart();
                
                if(isWait)
                {
                    window->setView(view);
                    setInstruction();
                }
                else
                {
                    window->setView(view);
                    setStart();
                }
            }
        }
        window->display();
	}
	
    void GameView::processInput(sf::Clock clock)
	{
		sf::Event Event;
		while(window->pollEvent(Event))
		{
			// exit
            if((Event.type == sf::Event::Closed) || ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)))
            {
				window->close();
            }

            if((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Up))
            {
                if(!isPlaying)
                {
                    gameGUI.MoveUp();
                }
            }
            
            if((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Down))
            {
                if(!isPlaying)
                {
                    gameGUI.MoveDown();
                }
            }

            if((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Return))
            {
                if(!isPlaying)
                {
                    switch(gameGUI.GetPressedItem())
                    {
                        case 0:
                            isPlaying = true;
                            isGameover = false;
                            clock.restart();
                            break;
                            
                        case 1:
                            isWait = true;
                            break;
                            
                        case 2:
                            window->close();
                            break;
                    }
                }
            }
            
            if((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::M))
            {
                if(!isPlaying)
                {
                    isWait = false;
                    isPlaying = true;
                    isGameover = false;
                    clock.restart();
                }
            }
            
            if((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::P))
            {
                isWait = !isWait;
            }
            
            if((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::R))
            {
                if(isPlaying)
                {
                    isPlaying = false;
                }
            }
            
            if((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Q))
            {
                if(!isGameover && isPlaying)
                {
                    isGameover = true;
                    isPlaying = false;
                }
            }
			
			// key press events
			if(Event.type == sf::Event::KeyPressed)
			{
				switch(Event.key.code)
				{
					case sf::Keyboard::Space:
						player->charging = true;
						break;
					case sf::Keyboard::D:
					
						player->faceLeft = false;
						player->moveLeft = true;
						break;
					case sf::Keyboard::A:
						player->moveRight = true;
						player->faceLeft = true;
						break;
				}
			}
			
			// key release events
			if(Event.type == sf::Event::KeyReleased)
			{
				switch(Event.key.code)
				{
					case sf::Keyboard::Space:
						player->charging = false;
						player->jump();
						break;
					case sf::Keyboard::D:
						player->moveLeft = false;
						break;
					case sf::Keyboard::A:
						player->moveRight = false; 
						break;
				}
			}
		}
	}
	
	void GameView::draw()
	{
		window->clear(sf::Color::Black);
		sf::Vector2f position(0, 0);
		position.y = player->getY() + 20 - (250);

		//draw background 
		background.setPosition(sf::Vector2f(position.x, position.y));
		background.setTextureRect(sf::IntRect(0, 0, window->getSize().x, window->getSize().y));
		window->draw(background);

		//std::cout << text.getCharacterSize() << "\n";
		// draw platforms
		for(int i=0; i < level->getPlatforms()->size(); i++)
		{
			Platform* platform = level->getPlatforms()->at(i);
			platform->render(window);
		}
		// draw player

		player->render(window);


		// draw lava
		//std::cout << "Printing Lava\n";
        lavaSprite.setPosition(sf::Vector2f(-600, level->getLavaY()));
        window->draw(lavaSprite);

		text.setPosition(sf::Vector2f(position.x + 20, position.y - 10));
		std::string scores = static_cast<std::ostringstream*>(&(std::ostringstream() << player->score))->str();
		text.setString(scores);
		window->draw(text);

		view.reset(sf::FloatRect(position.x, position.y, 800, 600));

        window->setView(view);
    }

	void GameView::respond(const EventInterface& events){
		if (events.getEventType() == GameOverEvent::eventId){
			isGameover = true;
			isPlaying = false;
		}
		else{
			std::cout << "UNKOWN EVENT \n";
		}
	}
}
