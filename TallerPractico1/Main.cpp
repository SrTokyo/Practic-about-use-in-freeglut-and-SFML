#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <math.h>

using namespace sf;
using namespace std;
bool nofunciona = false;
//translacion
void traslation(Vector2f position, VertexArray& vertex)
{
	for (int i = 0; i < vertex.getVertexCount(); i++)
	{
		vertex[i].position += position;
	}
}
//rotación
void rotation(float angle, Vector2f punto, VertexArray& vertex)
{
	float radians = angle * (float)3.1416f / 180.0f;
	for (int i = 0; i < vertex.getVertexCount(); i++)
	{
		vertex[i].position.x = punto.x + (vertex[i].position.x - punto.x) * cos(radians) - (vertex[i].position.y - punto.y) * sin(radians) ;
		vertex[i].position.y = punto.y + (vertex[i].position.x - punto.x) * sin(radians) + (vertex[i].position.y - punto.y) * cos(radians) ;
	}
}
//scaling
void scaling(Vector2f s, Vector2f f, VertexArray& vertex)
{
	for (int i = 0; i < vertex.getVertexCount(); i++)
	{
		vertex[i].position.x = vertex[i].position.x *s.x + f.x*(1 - s.x);
		vertex[i].position.y = vertex[i].position.y * s.y + f.y * (1 - s.y);
	}
}





typedef float Matrix3x3[3][3];
Matrix3x3 matComposite;
const double pi = 3.14159;

// Construct the 3 x 3 identity matrix. 
void matrix3x3SetIdentity(Matrix3x3 matIdent3x3)
{
	int row, col;
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			matIdent3x3[row][col] = (row == col);
}

// Premultiply matrix m1 times matrix m2, store result in m2. 
void matrix3x3PreMultiply(Matrix3x3 m1, Matrix3x3 m2)
{
	int row, col;
	Matrix3x3 matTemp;
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			matTemp[row][col] = m1[row][0] * m2[0][col] + m1[row][1] * m2[1][col] + m1[row][2] * m2[2][col];
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			m2[row][col] = matTemp[row][col];
}

void translate2D(Vector2f punto, bool inversa)
{
	float valx = punto.x;
	float valy = punto.y;
	if (inversa)
	{
		float valx = -punto.x;
		float valy = -punto.y;
	}
	Matrix3x3 matTransl;
	// Initialize translation matrix to identity. 
	matrix3x3SetIdentity(matTransl);
	matTransl[0][2] = valx;
	matTransl[1][2] = valy;
	// Concatenate matTransl with the composite matrix. 
	matrix3x3PreMultiply(matTransl, matComposite);
}
void rotate2D(Vector2f punto, float theta)
{
	Matrix3x3 matRot;
	// Initialize rotation matrix to identity. 
	matrix3x3SetIdentity(matRot);
	matRot[0][0] = cos(theta);
	matRot[0][1] = -sin(theta);
	matRot[0][2] = punto.x * (1 - cos(theta)) + punto.y * sin(theta);
	matRot[1][0] = sin(theta);
	matRot[1][1] = cos(theta);
	matRot[1][2] = punto.y * (1 - cos(theta)) - punto.x * sin(theta);
	// Concatenate matRot with the composite matrix. 
	matrix3x3PreMultiply(matRot, matComposite);
}


void translaR3(float angle, Vector2f pivot, VertexArray& vertex, Vector2f transla)
{
	translate2D(transla, false);

	for (int i = 0; i < vertex.getVertexCount(); i++)
	{
		vertex[i].position.x += matComposite[0][2];

		vertex[i].position.y += matComposite[1][2];
	}

	float radians = angle * (float)3.1416f / 180.0f;

	rotate2D(pivot, radians);

	for (int i = 0; i < vertex.getVertexCount(); i++)
	{
		vertex[i].position.x = pivot.x + (vertex[i].position.x - pivot.x) * matComposite[0][0] + (vertex[i].position.y - pivot.y) * matComposite[0][1] + (0 * matComposite[0][2]);

		// multiplico x 0 el pultimo componente ya que esta trabajando vertex array y no tiene componente en Z por lo que es 0

		vertex[i].position.y = pivot.y + (vertex[i].position.x - pivot.x) * matComposite[1][0] + (vertex[i].position.y - pivot.y) * matComposite[1][1] + (0 * matComposite[1][2]);
	}

	matrix3x3SetIdentity(matComposite);
}

void rotateMat(float angle, Vector2f pivot, VertexArray& vertex, Vector2f transla)
{
	translate2D(transla, false);

	float radians = angle * (float)3.1416f / 180.0f;

	rotate2D(pivot, radians);

	translate2D(transla, true);

	for (int i = 0; i < vertex.getVertexCount(); i++)
	{
		vertex[i].position.x = pivot.x + (vertex[i].position.x - pivot.x) * matComposite[0][0] + (vertex[i].position.y - pivot.y) * matComposite[0][1] + (0 * matComposite[0][2]);

		// multiplico x 0 el pultimo componente ya que esta trabajando vertex array y no tiene componente en Z por lo que es 0

		vertex[i].position.y = pivot.y + (vertex[i].position.x - pivot.x) * matComposite[1][0] + (vertex[i].position.y - pivot.y) * matComposite[1][1] + (0 * matComposite[1][2]);
	}

	matrix3x3SetIdentity(matComposite);
}




int main(int argc, char** argv)
{
	matrix3x3SetIdentity(matComposite);
	srand(time(NULL));
	
	RenderWindow window(VideoMode(1600, 1000), "GAME 1");

	ancla:

	window.setFramerateLimit(60);

	Event event;

	int cont = 0;

	Font font;

	if (!font.loadFromFile("Fontesitas/Lovely Boys Font by 7NTypes_D.otf"))
		return EXIT_FAILURE;

	Text textMenu("presiona  K  para empezar", font, 50);

	textMenu.setPosition(Vector2f(550, 450));

	Vector2f vertexD(10.0f, 0.0f);
	Vector2f vertexA(0.0f, 10.0f);
	
	Texture textureBackground;
	textureBackground.loadFromFile("imagencita/wall.jpg");

	Texture texturecharacter;
	texturecharacter.loadFromFile("imagencita/blackmage.png");

	Texture textureBook;
	textureBook.loadFromFile("imagencita/Speed.png");

	Texture textureAxe;
	textureAxe.loadFromFile("imagencita/Axee.png");

	Texture textureMacaco;
	textureMacaco.loadFromFile("imagencita/Macaco.jpg");

	Sprite spriteBackground;

	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	Sprite spriteMacaco;

	spriteMacaco.setTexture(textureMacaco);
	spriteMacaco.setPosition(750, 400);
	
	VertexArray progress(sf::Quads, 4);

	progress[0].position = sf::Vector2f(10.0f, 10.0f);
	progress[1].position = sf::Vector2f(110.0f, 10.0f);
	progress[2].position = sf::Vector2f(110.0f, 110.0f);
	progress[3].position = sf::Vector2f(10.0f, 110.0f);
	
	progress[0].texCoords = sf::Vector2f(0.f, 0.f);
	progress[1].texCoords = sf::Vector2f(200.f, 0.f);
	progress[2].texCoords = sf::Vector2f(200.f, 190.f);
	progress[3].texCoords = sf::Vector2f(0.f, 190.f);
	
	bool menu = true;
	int spd1 = rand() % 15;
	spd1 = (spd1 * 100);
	
	int spd2 = rand() % 10;
	spd2 = spd2 * 100;
	
	VertexArray speed(sf::Quads, 4);

	speed[0].position = sf::Vector2f((float)spd1, (float)spd2);
	speed[1].position = sf::Vector2f((float)(spd1 + 100.0f), (float)spd2 );
	speed[2].position = sf::Vector2f((float)(spd1 + 100.0f), (float)(spd2 + 100.0f));
	speed[3].position = sf::Vector2f((float)spd1 , (float)(spd2 + 100.0f));

	speed[0].texCoords = sf::Vector2f(0.f, 0.f);
	speed[1].texCoords = sf::Vector2f(682.f, 0.f);
	speed[2].texCoords = sf::Vector2f(682.f, 665.f);
	speed[3].texCoords = sf::Vector2f(0.f, 665.f);

	VertexArray enemy(sf::Quads, 4);

	enemy[0].position = sf::Vector2f(1300.0f, 750.0f);
	enemy[1].position = sf::Vector2f(1450.0f, 750.0f);
	enemy[2].position = sf::Vector2f(1450.0f, 900.0f);
	enemy[3].position = sf::Vector2f(1300.0f, 900.0f);

	enemy[0].texCoords = sf::Vector2f(0.f, 0.f);
	enemy[1].texCoords = sf::Vector2f(952.f, 0.f);
	enemy[2].texCoords = sf::Vector2f(952.f, 1000.f);
	enemy[3].texCoords = sf::Vector2f(0.f, 1000.f);

	bool perdio = false;
	bool disparo1 = false;
	bool disparo2 = false;
	bool disparo3 = false;
	bool disparo4 = false;

	VertexArray bala1(sf::Lines, 2);
	VertexArray bala2(sf::Lines, 2);
	VertexArray bala3(sf::Lines, 2);
	VertexArray bala4(sf::Lines, 2);
	
	while (window.isOpen())
	{
		
		String pato = to_string(cont);

		Text textA(pato, font, 50);

		textA.setPosition(Vector2f(1450, 10));
		
		

		while (window.pollEvent(event))
		{
			if (event.type == Event::EventType::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{

				if (event.key.code == Keyboard::Escape)
				{
					window.close();
				
				}
				if (event.key.code == Keyboard::Right)
				{
					
					bala1[0].position = sf::Vector2f(progress[1].position.x, ((progress[2].position.y - progress[1].position.y) / 2 + progress[1].position.y));
					bala1[1].position = sf::Vector2f(progress[1].position.x+20, ((progress[2].position.y - progress[1].position.y) / 2+ progress[1].position.y));
					disparo1 = true;
				}
				if (event.key.code == Keyboard::Left)
				{

					bala2[0].position = sf::Vector2f(progress[0].position.x, ((progress[2].position.y - progress[1].position.y) / 2 + progress[1].position.y));
					bala2[1].position = sf::Vector2f(progress[0].position.x - 20, ((progress[2].position.y - progress[1].position.y) / 2 + progress[1].position.y));
					disparo2 = true;
				}
				if (event.key.code == Keyboard::Up)
				{

					bala3[0].position = sf::Vector2f(((progress[1].position.x - progress[0].position.x) / 2 + progress[0].position.x), progress[0].position.y);
					bala3[1].position = sf::Vector2f(((progress[1].position.x - progress[0].position.x) / 2 + progress[0].position.x), progress[0].position.y - 20);
					disparo3 = true;
				}
				if (event.key.code == Keyboard::Down)
				{

					bala4[0].position = sf::Vector2f(((progress[1].position.x - progress[0].position.x) / 2 + progress[0].position.x), progress[2].position.y);
					bala4[1].position = sf::Vector2f(((progress[1].position.x - progress[0].position.x) / 2 + progress[0].position.x), progress[2].position.y + 20);
					disparo4 = true;
				}
				if (event.key.code == Keyboard::K)
				{
					menu = false;

				}

			}

		}



		Vector2f enemS(1.0f, 1.0f);

		Vector2f enem;

		rotateMat(1.0f, enemy[3].position, enemy, Vector2f(0.0f, 0.0f) );

		scaling(Vector2f(1.0001f, 1.0001f), enemy[3].position, enemy);

		enem.x = progress[0].position.x - enemy[3].position.x;

		enem.y = progress[0].position.y - enemy[3].position.y;

		float norm = sqrt(enem.x * enem.x + enem.y * enem.y);

		if (norm)
		{
			enem.x *= (enemS.x / norm);
			enem.y *= (enemS.y / norm);
		}

		traslation(enem, enemy);



		for (int i = 0; i < enemy.getVertexCount(); i++)
		{
				int hitX = (enemy[i].position.x - progress[i].position.x);
				int hity = (enemy[i].position.y - progress[i].position.y);
				if ((hitX + hity) == 0)
				{
					perdio = true;

				}
			
		}

		for (int i = 0; i < speed.getVertexCount(); i++)
		{
			for (int j = 0; j < progress.getVertexCount(); j++)
			{
				int hitX = (speed[i].position.x - progress[j].position.x);
				int hity = (speed[i].position.y - progress[j].position.y);
				if ((hitX + hity) == 0)
				{
					int spd1t = rand() % 15;
					spd1t = (spd1t * 100);

					int spd2t = rand() % 10;
					spd2t = spd2t * 100;

					speed[0].position = sf::Vector2f((float)spd1t, (float)spd2t);
					speed[1].position = sf::Vector2f((float)(spd1t + 100.0f), (float)spd2t);
					speed[2].position = sf::Vector2f((float)(spd1t + 100.0f), (float)(spd2t + 100.0f));
					speed[3].position = sf::Vector2f((float)spd1t, (float)(spd2t + 100.0f));
					cont++;
				}
			}
		}

		for (int i = 0; i < enemy.getVertexCount(); i++)
		{
			for (int j = 0; j < bala1.getVertexCount(); j++)
			{
				int hitX = (enemy[i].position.x - bala1[j].position.x);
				int hity = (enemy[i].position.y - bala1[j].position.y);
				if ((hitX + hity) == 0)
				{
					enemy[0].position = sf::Vector2f(1300.0f, 750.0f);
					enemy[1].position = sf::Vector2f(1450.0f, 750.0f);
					enemy[2].position = sf::Vector2f(1450.0f, 900.0f);
					enemy[3].position = sf::Vector2f(1300.0f, 900.0f);

				}
			}
		}

		for (int i = 0; i < enemy.getVertexCount(); i++)
		{
			for (int j = 0; j < bala2.getVertexCount(); j++)
			{
				int hitX = (enemy[i].position.x - bala2[j].position.x);
				int hity = (enemy[i].position.y - bala2[j].position.y);
				if ((hitX + hity) == 0)
				{
					enemy[0].position = sf::Vector2f(1300.0f, 750.0f);
					enemy[1].position = sf::Vector2f(1450.0f, 750.0f);
					enemy[2].position = sf::Vector2f(1450.0f, 900.0f);
					enemy[3].position = sf::Vector2f(1300.0f, 900.0f);

				}
			}
		}

		for (int i = 0; i < enemy.getVertexCount(); i++)
		{
			for (int j = 0; j < bala3.getVertexCount(); j++)
			{
				int hitX = (enemy[i].position.x - bala3[j].position.x);
				int hity = (enemy[i].position.y - bala3[j].position.y);
				if ((hitX + hity) == 0)
				{
					enemy[0].position = sf::Vector2f(1300.0f, 750.0f);
					enemy[1].position = sf::Vector2f(1450.0f, 750.0f);
					enemy[2].position = sf::Vector2f(1450.0f, 900.0f);
					enemy[3].position = sf::Vector2f(1300.0f, 900.0f);

				}
			}
		}

		for (int i = 0; i < enemy.getVertexCount(); i++)
		{
			for (int j = 0; j < bala4.getVertexCount(); j++)
			{
				int hitX = (enemy[i].position.x - bala4[j].position.x);
				int hity = (enemy[i].position.y - bala4[j].position.y);
				if ((hitX + hity) == 0)
				{
					enemy[0].position = sf::Vector2f(1300.0f, 750.0f);
					enemy[1].position = sf::Vector2f(1450.0f, 750.0f);
					enemy[2].position = sf::Vector2f(1450.0f, 900.0f);
					enemy[3].position = sf::Vector2f(1300.0f, 900.0f);

				}
			}
		}



		if (disparo1)
		{
			if (nofunciona)
			{
				traslation(Vector2f(5.0f, 0.0f), bala1);
			}
			else {
				translaR3(2.0f, bala1[0].position, bala1, Vector2f(5.0f, 0.0f));
			}
		}

		if (disparo2)
		{
			if (nofunciona)
			{
				traslation(Vector2f(-5.0f, 0.0f), bala2);
			}
			else {
				translaR3(2.0f, bala2[0].position, bala2, Vector2f(-5.0f, 0.0f));
			}
		}

		if (disparo3)
		{
			if (nofunciona)
			{
				traslation(Vector2f(0.0f, -5.0f), bala3);
			}
			else {
				translaR3(2.0f, bala3[0].position, bala3, Vector2f(0.0f, -5.0f));
			}
		}

		if (disparo4)
		{
			if (nofunciona)
			{
				traslation(Vector2f(0.0f, 5.0f), bala4);
			}
			else {
				translaR3(2.0f, bala4[0].position, bala4, Vector2f(0.0f, 5.0f));
			}
		}
			
		
		

		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			traslation(vertexD, progress);
		}
		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			traslation(-vertexD, progress);
		}
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			traslation(-vertexA, progress);
		}
		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			traslation(vertexA, progress);
		}

		

		window.clear();
		if (perdio)
		{
			window.draw(spriteBackground);
				window.draw(spriteMacaco);
				window.display();
				Sleep(2000);
				goto ancla;
		}
		else {
			if (menu)
			{
				window.draw(spriteBackground);
				window.draw(textMenu);
				
			}
			else {
				window.draw(spriteBackground);
				window.draw(textA);
				window.draw(progress, &texturecharacter);
				window.draw(speed, &textureBook);
				window.draw(enemy, &textureAxe);
				window.draw(bala1);
				window.draw(bala2);
				window.draw(bala3);
				window.draw(bala4);
			}
		}
		window.display();
	}
	return 0;
}