

#include <iostream>
#include<sstream>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
using namespace std;
using namespace sf;
void updatebranches(int x);
const int numofbranches = 6;
Sprite branches[numofbranches];
enum class side{LEFT,RIGHT,NONE};
side branchposition[numofbranches];
int Best = 0;

int main()
{
	VideoMode vm(1920, 1080);
	RenderWindow window(vm, "chopchop", Style::Fullscreen);
	Texture texturebackground;
	texturebackground.loadFromFile("Graphics/bcg.png");
	Sprite spritebackground;
	spritebackground.setTexture(texturebackground);
	spritebackground.setPosition(0, 0);

	Texture tree;
	tree.loadFromFile("Graphics/bark.png");
	Sprite spritetree;
	spritetree.setTexture(tree);
	spritetree.setPosition(810, 0);

	Texture bee;
	bee.loadFromFile("Graphics/bee.png");
	Sprite spritebee;
	spritebee.setTexture(bee);
	bool isbeeactive=false;
	float beespeed = 0.0f;

	Texture cloud;
	cloud.loadFromFile("Graphics/cloud.png");
	Sprite spritecloud1,spritecloud2;
	spritecloud1.setTexture(cloud);
	spritecloud2.setTexture(cloud);
	Texture branch;
	branch.loadFromFile("Graphics/branch.png");
	for (int i = 0; i < numofbranches; i++) {
		branches[i].setTexture(branch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}
	Texture player;
	player.loadFromFile("Graphics/player.png");
	Sprite spriteplayer;
	spriteplayer.setTexture(player);
	spriteplayer.setPosition(580,720);
	side playerside = side::LEFT;
	Texture rip;
	rip.loadFromFile("Graphics/rip.png");
	Sprite spriterip;
	spriterip.setTexture(rip);
	spriterip.setPosition(2000,860);
	Texture axe;
	axe.loadFromFile("Graphics/axe.png");
	Sprite spriteaxe;
	spriteaxe.setTexture(axe);
	spriteaxe.setPosition(700,830);
	float axepositionright = 1075, axepositionleft = 700;
	Texture log;

	log.loadFromFile("Graphics/log.png");

	Sprite spritelog;

	spritelog.setTexture(log);
	spritelog.setPosition(810,720);

	bool logactive = false,acceptinput=false;

	SoundBuffer chopbuffer, deathbuffer, timeoutbuffer;

	chopbuffer.loadFromFile("Sound/chop.wav");
	deathbuffer.loadFromFile("Sound/death.wav");
	timeoutbuffer.loadFromFile("Sound/timeout.wav");

	Sound chop,death,timeout;
	chop.setBuffer(chopbuffer);
	death.setBuffer(deathbuffer);
	timeout.setBuffer(timeoutbuffer);

	float logspeedx=10.0f, logspeedy = -15.0f;
	bool cloud1active = false,cloud2active=false;
	float cloud1speed = 0.0f,cloud2speed=0.0f;
	bool paused = true;
	int score = 0;
	Text messagetext, scoretext,Besttext;
	Font font;
	font.loadFromFile("Fonts/KOMIKAP_.ttf");
	messagetext.setFont(font);
	scoretext.setFont(font);
	Besttext.setFont(font);
	messagetext.setString("Press Enter to play!!");
	scoretext.setString("Score=0");
	messagetext.setCharacterSize(100);
	scoretext.setCharacterSize(75);
	messagetext.setFillColor(Color::White);
	scoretext.setFillColor(Color::Red);

	Besttext.setCharacterSize(50);
	Besttext.setFillColor(Color::White);
	FloatRect textrect = messagetext.getLocalBounds(); // for initialising the coordinates to rectangle formed by messagetext
	messagetext.setOrigin(textrect.left + textrect.width / 2.0f, textrect.top + textrect.height / 2.0f);
	messagetext.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoretext.setPosition(15, 15);
	Besttext.setPosition(1650, 20);
	
	Clock clock;
	// Adding a time bar
	RectangleShape timebar;   // class of sfml to make rectangle object
	float timebarwidth = 50, timebarstartheight = 400;
	timebar.setSize(Vector2f(timebarwidth, timebarstartheight));
	timebar.setFillColor(Color::Yellow);
	timebar.setPosition(50, 650);

	Time gametimetotal;
	float timeremaining = 10.0f, shrinkheightpersecond = timebarstartheight / timeremaining;
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyReleased && !paused) {
				acceptinput = true;
				spriteaxe.setPosition(2000, spriteaxe.getPosition().y);
			}

		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Enter)) {
			paused = false;
			score = 0;
			timeremaining = 10.0f;
			for (int i = 0; i < numofbranches; i++) {
				branchposition[i] = side::NONE;
			}
			spriterip.setPosition(675,2000);
			spriteplayer.setPosition(580,720);
				acceptinput = true;
		}
		if (acceptinput) {
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				 playerside=side::RIGHT;
				score++;
				timeremaining = 10/ score + 0.8f;
				spriteaxe.setPosition(axepositionright, spriteaxe.getPosition().y);
				spriteplayer.setPosition(1200, 720);
				updatebranches(score);
				spritelog.setPosition(810,720);
				logspeedx = -500;
				logactive = true;
				acceptinput = false;
				chop.play();
			}
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				 playerside = side::LEFT;
				score++;
				timeremaining = 10/ score + 0.8f;
				spriteaxe.setPosition(axepositionleft, spriteaxe.getPosition().y);
				spriteplayer.setPosition(580, 720);
				updatebranches(score);
				spritelog.setPosition(810, 720);
				logspeedx = 500;
				logactive = true;
				acceptinput = false;
				chop.play();
			}
		}
		if (!paused) {
			Time dt = clock.restart();
			timeremaining -= dt.asSeconds()/2.0f;
			timebar.setSize(Vector2f(timebarwidth, shrinkheightpersecond * timeremaining));
			if (timeremaining <= 0.0f) {
				paused = true;
				messagetext.setString("Game Over!");
				FloatRect textrect2 = messagetext.getLocalBounds();
				messagetext.setOrigin(textrect2.left + textrect2.width / 2.0f, textrect2.top + textrect2.height);
		 	    messagetext.setPosition(1920/2.0f,1080/2.0f);   
				timeout.play();
			}
			if (!isbeeactive) {
				srand((int)time(0));
				beespeed = (rand() % 100) + 100;
				srand((int)time(0) * 10);
				float height = (rand() % 400) + 300;
				spritebee.setPosition(2000, height);
				isbeeactive = true;
			}
			else {
				spritebee.setPosition(spritebee.getPosition().x - beespeed * dt.asSeconds(), spritebee.getPosition().y);
				if (spritebee.getPosition().x < -100)
					isbeeactive = false;
			}

			if (!cloud1active) {
				srand((int)time(0) * 10);
				cloud1speed = (rand() % 20) + 70;
				srand((int)time(0) * 10);
				float height = rand() % 150;
				spritecloud1.setPosition(-200, height);
				cloud1active = true;
			}
			else {
				spritecloud1.setPosition(spritecloud1.getPosition().x + cloud1speed * dt.asSeconds(), spritecloud1.getPosition().y);
				if (spritecloud1.getPosition().x > 1920)
					cloud1active = false;
			}
			if (!cloud2active) {
				srand((int)time(0) * 20);
				cloud2speed = (rand() % 20) + 50;
				srand((int)time(0) * 20);
				float height = rand() % 80;
				spritecloud2.setPosition(-200, height);
				cloud2active = true;
			}
			else {
				spritecloud2.setPosition(spritecloud2.getPosition().x + cloud2speed * dt.asSeconds(), spritecloud2.getPosition().y);
				if (spritecloud2.getPosition().x > 1920)
					cloud2active = false;
			}
			stringstream ss,ss1;
			ss << "Score= " << score;
			scoretext.setString(ss.str());
			if (score > Best) {
				Best = score;
			}
			ss1 << "Best= " << Best;
			Besttext.setString(ss1.str());

			for (int i = 0; i < numofbranches; i++) {
				float height = i * 150;
				if (branchposition[i] == side::LEFT) {
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
				}
				else if (branchposition[i] == side::RIGHT) {
					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);
				}
				else {
					branches[i].setPosition(3000, height);
				}
				if (logactive) {
					spritelog.setPosition(spritelog.getPosition().x + logspeedx * dt.asSeconds(), spritelog.getPosition().y + logspeedy * dt.asSeconds());
					if (spritelog.getPosition().x < -100 || spritelog.getPosition().y>2000) {
						logactive = false;
						spritelog.setPosition(810,720);
					}
					if (branchposition[5] == playerside) {
						paused = true;
						acceptinput = false;
						spriterip.setPosition(525,760);
						spriteplayer.setPosition(2000,660);
						messagetext.setString("crushed!!");
						FloatRect textrect = messagetext.getLocalBounds();
						messagetext.setOrigin(textrect.left + textrect.width / 2.0f, textrect.top + textrect.height / 2.0f);
						messagetext.setPosition(1920 / 2.0f, 1080 / 2.0f);
						death.play();
					}
				}

			}

			
		}
		window.clear();
		window.draw(spritebackground);
		window.draw(spritecloud1);
		window.draw(spritecloud2);
		for (int i = 0; i < numofbranches; i++) {
			window.draw(branches[i]);
		}
		window.draw(spritetree);
		window.draw(spriteplayer);
		window.draw(spriteaxe);
		window.draw(spritelog);
		window.draw(spriterip);
		window.draw(spritebee);
		window.draw(scoretext);
		window.draw(Besttext);
		window.draw(timebar);
		if (paused) {
			score = 0;
			window.draw(messagetext);
		}
		window.display();

	}
	return 0;
}
void updatebranches(int x) {
	for (int j = numofbranches - 1; j > 0; j--) {
		branchposition[j] = branchposition[j - 1];
	}
	srand((int)time(0) + x);
	int y = rand() % 5;
	switch (y) {
	case 0:
		branchposition[0] = side::LEFT;
		break;
	case 1:
		branchposition[0] = side::RIGHT;
		break;
	default:
		branchposition[0] = side::NONE;
		break;

	}
}

