#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#ifndef SDL_h_
#include <SDL2/SDL.h>
#endif /* ifndef SDL_h_ */
#include <stdio.h>

//Коды
#define TRUE 1
#define FALSE 0

#define NOERR	0
#define SDLERR	-1
#define NOWINDOW -4
#define NORENDERER -5

//Параметры окна
#define TITLE "Лабораторная работа №3. Задание 2"
#define BGCOLOUR 0xE3, 0xA0, 0xA0
#define FGCOLOUR 0x40, 0x40, 0x80

int DrawDiagonals(SDL_Renderer* rend, int stepSize, int hsize, int vsize, int drawPeriod/* , Uint8 r, Uint8 g, Uint8 b */);
int PrintErrorDestroyAllAndExit(SDL_Renderer* rend, SDL_Window* win);
int DestroyAllAndExit(SDL_Renderer* rend, SDL_Window* win);
int DrawBackground(SDL_Renderer* rend, Uint8 r, Uint8 g, Uint8 b);
int CreateAndCheckSpace(SDL_Window** win, SDL_Renderer** rend, int h, int w);
int GetLineDistance(void);
int GetDrawPeriod(void);
void GetWindowSize(int* w, int* h);

int main(int argc, char** argv)
{
	int Window_HSize, Window_VSize;
	//Параметры окна
	GetWindowSize(&Window_HSize, &Window_VSize);
	int lineSpace = GetLineDistance();
	int drawPeriod = GetDrawPeriod();

	
	//Создание рабочего пространства
	SDL_Window* win = NULL;
	SDL_Renderer* rend = NULL;
	if (CreateAndCheckSpace(&win, &rend, Window_HSize, Window_VSize))
		return SDLERR;

	//Заливка фона, завершение работы если не удалось
	if (DrawBackground(rend, BGCOLOUR))
		return PrintErrorDestroyAllAndExit(rend, win);
	SDL_Delay(200);

	//Рисование линий, завершение работы если не удалось
	SDL_SetRenderDrawColor(rend, FGCOLOUR, 0xFF);
	if (DrawDiagonals (rend, lineSpace, Window_HSize, Window_VSize, drawPeriod/* , FGCOLOUR) */))
		return PrintErrorDestroyAllAndExit(rend, win);

	SDL_Event ev;
	int runs = 1;
	while(runs)
		while (SDL_PollEvent(&ev))
			if (ev.type == SDL_KEYUP)
				if(ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					runs = 0;
	//Штатное завершение работы
	return DestroyAllAndExit(rend,win);
}

/*Рисование диагоналей на расстоянии stepSize друг от друга рисовальщиком rend 
в прямоугольнике размером hsize на visze, цвета r g b.
При корректном выполнении осуществляется возврат 0, в противном случае -- код ошибки*/
int DrawDiagonals(SDL_Renderer* rend, int stepSize,int hsize, int vsize, 
					int drawPeriod/* ,Uint8 r, Uint8 g, Uint8 b */)
{
	//Установка цвета
	//SDL_SetRenderDrawColor(rend, r, g, b, 0xFF);

	for (int i = -1 * hsize; i < hsize; i += stepSize)
	{
		if (SDL_RenderDrawLine(rend, i, 0, i + hsize, vsize))
			return SDLERR;
		SDL_RenderPresent(rend);
		SDL_Delay(drawPeriod);
	}

	return NOERR;
}

/*Вызов SDL, создание окна и рисовальщика.
Если что-то не удалось выполнить, вывод ошибки и завершение работы*/
int CreateAndCheckSpace(SDL_Window** win, SDL_Renderer** rend, int h, int w)
{
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Ошибка SDL: %s", SDL_GetError());
		return SDLERR;
	}
	if (NULL == (*win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		h, w, SDL_WINDOW_SHOWN)))
		return PrintErrorDestroyAllAndExit(*rend, *win);
	if (NULL == (*rend = SDL_CreateRenderer(*win, -1, SDL_RENDERER_SOFTWARE)))
		return PrintErrorDestroyAllAndExit(*rend, *win);
	/*else*/ return NOERR;
}

/*Функция рисования фона рисовальщика rend, цветом r g b
При корректном выполнении осуществляется возврат 0, в противном случае -- код ошибки*/
int DrawBackground(SDL_Renderer* rend, Uint8 r, Uint8 g, Uint8 b)
{
	if (SDL_SetRenderDrawColor(rend, r, g, b, 0xFF) ||
		SDL_RenderClear(rend))
	{
		return SDLERR;
	}
	/*else*/
	SDL_RenderPresent(rend);
	return NOERR;
}

/*Удаление рисовальщика rend, окна win, завершение работы SDL*/
int DestroyAllAndExit(SDL_Renderer* rend, SDL_Window* win)
{
	int errcode = 0;
	if (rend)
		SDL_DestroyRenderer(rend);
	else 
		errcode = NORENDERER;
	if (win)
		SDL_DestroyWindow(win);
	else
		errcode = NOWINDOW;
	SDL_Quit();
	return errcode;
}

/*Вывод сообщения об ошибке, удаление рисовальщика rend, окна win, завершение работы SDL*/
int PrintErrorDestroyAllAndExit(SDL_Renderer* rend, SDL_Window* win)
{
	printf("Ошибка SDL: %s", SDL_GetError());
	return DestroyAllAndExit(rend, win);
}

/*Получение размера окна шириной w и высотой h*/
void GetWindowSize(int * w, int * h)
{
	do
	{
		printf("Введите размер окна по горизонтали: ");
		scanf("%d", w);
		if (*w > 0)
			break;
		/*else*/puts("Неправильный размер!");
	} while (TRUE);
	do
	{
		printf("Введите размер окна по вертикали: ");
		scanf("%d", h);
		if (*h > 0)
			return;
		/*else*/puts("Неправильный размер!");
	} while (TRUE);
}

/*Получение раcстояния d между линиями*/
int GetLineDistance(void)
{
	int d;
	do
	{
		printf("Введите размер расстояния между линиями: ");
		scanf("%d", &d);
		if (d > 0)
			return d;
		/*else*/puts("Неправильный размер!");
	} while (TRUE);
}

/*Получение периода t рисования линий*/
int GetDrawPeriod(void)
{
	int t;
	do
	{
		printf("Введите период рисования линий: ");
		scanf("%d", &t);
		if (t > 0)
			return t;
		/*else*/puts("Неправильная величина!");
	} while (TRUE);
}
