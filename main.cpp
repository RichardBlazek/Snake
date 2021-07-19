#include "objsdl/objsdl.h"
#include "mylibraries/dynarr.h"
#include "mylibraries/random.h"

using namespace std;
using containers::DynArr;

class Snake
{
private:
	DynArr<SDL::Point> body;
	SDL::Point dir;
public:
	Snake(size_t len, SDL::Point headpos, SDL::Point dir):body(len), dir(dir)
	{
        for(size_t i=0;i<len;++i)
		{
            body[i]=headpos-dir*i;
		}
	}
	void Move()
	{
		for(size_t i=body.size()-1;i>0;--i)
		{
			body[i]=body[i-1];
		}
		body[0]+=dir;
	}
	void EatFruit()
	{
		body.push_back(body.back());
	}
	void ChangeDirection(SDL::Point new_dir)
	{
		if(new_dir!=-dir&&abs(dir.x)+abs(dir.y)==1)
		{
			dir=new_dir;
		}
	}
	const DynArr<SDL::Point>& GetBody()
	{
		return body;
	}
};

int main()
{
	SDL::Init _i;
	Random random(time(0));
	constexpr SDL::Point snakesquare(25,25);
	SDL::Window screen("Snake", SDL::Rect(SDL::Window::UndefinedPos, 1000, 500));
	SDL::Renderer rend(screen);
	Snake snake(2, {10,10}, SDL::Point(-1,0));
	SDL::Point fruit(12,12);
	const auto& snakebody=snake.GetBody();
	bool end=false;
    while(!SDL::events::Quit()&&!end)
	{
		rend.Repaint(SDL::Color(255,255,160));
		rend.Draw(SDL::Rect(fruit*snakesquare, snakesquare),SDL::Color(0,255,0));
        for(auto bodypart:snakebody)
		{
			rend.Draw(SDL::Rect(bodypart*snakesquare, snakesquare), SDL::Color(255,0,0));
		}
		rend.Draw({snakebody[0]*snakesquare+snakesquare/2-SDL::Point(2,2), 4,4}, SDL::Color(0,0,0));
		rend.Show();
		for(auto& event:SDL::events::Handler())
		{
			if(event.Type()==SDL::events::Type::Keydown)
			{
				if(event.Keyboard().Key==SDL::Keycode::Down)
				{
					snake.ChangeDirection(SDL::Point(0, 1));
				}
				else if(event.Keyboard().Key==SDL::Keycode::Up)
				{
					snake.ChangeDirection(SDL::Point(0, -1));
				}
				else if(event.Keyboard().Key==SDL::Keycode::Left)
				{
					snake.ChangeDirection(SDL::Point(-1, 0));
				}
				else if(event.Keyboard().Key==SDL::Keycode::Right)
				{
					snake.ChangeDirection(SDL::Point(1, 0));
				}
				break;
			}
		}
		snake.Move();
		if(snakebody[0].x>=40)
		{
			snakebody[0].x-=40;
		}
		if(snakebody[0].x<0)
		{
			snakebody[0].x+=40;
		}
		if(snakebody[0].y>=20)
		{
			snakebody[0].y-=20;
		}
		if(snakebody[0].y<0)
		{
			snakebody[0].y+=21;
		}
		if(snakebody[0]==fruit)
		{
			snake.EatFruit();
			fruit.x=random.Next(35);
			fruit.y=random.Next(20);
		}
		for(size_t i=1;i<snakebody.size();++i)
		{
			if(snakebody[i]==snakebody[0])
			{
				end=true;
				break;
			}
		}
		SDL::Wait(200);
	}
	SDL::MessageBox::Show("You lost", "Score: "+to_string(snakebody.size()-2));
    return 0;
}