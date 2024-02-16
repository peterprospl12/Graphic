/******************************************************************
 Grafika komputerowa, �rodowisko MS Windows - program  przyk�adowy
 *****************************************************************/

#include <windows.h>
#include <gdiplus.h>
#include <math.h>
using namespace Gdiplus;

struct move {
	int x = 0;
	int y = 0;
	int angle = 0;
} moveXY;

//deklaracja funkcji obslugi okna
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//funkcja Main - dla Windows
 int WINAPI WinMain(HINSTANCE hInstance,
               HINSTANCE hPrevInstance,
               LPSTR     lpCmdLine,
               int       nCmdShow)
{
	MSG meldunek;		  //innymi slowy "komunikat"
	WNDCLASS nasza_klasa; //klasa g��wnego okna aplikacji
	HWND okno;
	static char nazwa_klasy[] = "Podstawowa";
	
	GdiplusStartupInput gdiplusParametry;// parametry GDI+; domy�lny konstruktor wype�nia struktur� odpowiednimi warto�ciami
	ULONG_PTR	gdiplusToken;			// tzw. token GDI+; warto�� uzyskiwana przy inicjowaniu i przekazywana do funkcji GdiplusShutdown
   
	// Inicjujemy GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusParametry, NULL);

	//Definiujemy klase g��wnego okna aplikacji
	//Okreslamy tu wlasciwosci okna, szczegoly wygladu oraz
	//adres funkcji przetwarzajacej komunikaty
	nasza_klasa.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	nasza_klasa.lpfnWndProc   = WndProc; //adres funkcji realizuj�cej przetwarzanie meldunk�w 
 	nasza_klasa.cbClsExtra    = 0 ;
	nasza_klasa.cbWndExtra    = 0 ;
	nasza_klasa.hInstance     = hInstance; //identyfikator procesu przekazany przez MS Windows podczas uruchamiania programu
	nasza_klasa.hIcon         = 0;
	nasza_klasa.hCursor       = LoadCursor(0, IDC_ARROW);
	nasza_klasa.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
	nasza_klasa.lpszMenuName  = "Menu" ;
	nasza_klasa.lpszClassName = nazwa_klasy;

    //teraz rejestrujemy klas� okna g��wnego
    RegisterClass (&nasza_klasa);
	
	/*tworzymy okno g��wne
	okno b�dzie mia�o zmienne rozmiary, listw� z tytu�em, menu systemowym
	i przyciskami do zwijania do ikony i rozwijania na ca�y ekran, po utworzeniu
	b�dzie widoczne na ekranie */
 	okno = CreateWindow(nazwa_klasy, "Grafika komputerowa", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	
	
	/* wyb�r rozmiaru i usytuowania okna pozostawiamy systemowi MS Windows */
   	ShowWindow (okno, nCmdShow) ;
    
	//odswiezamy zawartosc okna
	UpdateWindow (okno) ;

	// G��WNA P�TLA PROGRAMU
	while (GetMessage(&meldunek, NULL, 0, 0))
     /* pobranie komunikatu z kolejki; funkcja GetMessage zwraca FALSE tylko dla
	 komunikatu WM_QUIT; dla wszystkich pozosta�ych komunikat�w zwraca warto�� TRUE */
	{
		TranslateMessage(&meldunek); // wst�pna obr�bka komunikatu
		DispatchMessage(&meldunek);  // przekazanie komunikatu w�a�ciwemu adresatowi (czyli funkcji obslugujacej odpowiednie okno)
	}

	GdiplusShutdown(gdiplusToken);
	
	return (int)meldunek.wParam;
}

/********************************************************************
FUNKCJA OKNA realizujaca przetwarzanie meldunk�w kierowanych do okna aplikacji*/
LRESULT CALLBACK WndProc (HWND okno, UINT kod_meldunku, WPARAM wParam, LPARAM lParam)
{
	HMENU mPlik, mInfo, mGlowne;
    	
/* PONI�SZA INSTRUKCJA DEFINIUJE REAKCJE APLIKACJI NA POSZCZEG�LNE MELDUNKI */
	switch (kod_meldunku) 
	{
	case WM_CREATE:  //meldunek wysy�any w momencie tworzenia okna
		mPlik = CreateMenu();
		AppendMenu(mPlik, MF_STRING, 100, "&Zapiszcz...");
		AppendMenu(mPlik, MF_SEPARATOR, 0, "");
		AppendMenu(mPlik, MF_STRING, 101, "&Koniec");
		mInfo = CreateMenu();
		AppendMenu(mInfo, MF_STRING, 200, "&Autor...");
		mGlowne = CreateMenu();
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mPlik, "&Plik");
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mInfo, "&Informacja");
		SetMenu(okno, mGlowne);
		DrawMenuBar(okno);

	case WM_COMMAND: //reakcje na wyb�r opcji z menu
		switch (wParam)
		{
		case 100: if(MessageBox(okno, "Zapiszcze�?", "Pisk", MB_YESNO) == IDYES)
					MessageBeep(0);
                  break;
		case 101: DestroyWindow(okno); //wysylamy meldunek WM_DESTROY
        		  break;
		case 200: MessageBox(okno, "Imi� i nazwisko: Piotr Sulewski \nNumer indeksu: 192594 ", "Autor", MB_OK);
		}
		return 0;
	
	case WM_LBUTTONDOWN: //reakcja na lewy przycisk myszki
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			return 0;
		}
	case WM_KEYDOWN:
	{
		if (wParam == VK_UP) {
			moveXY.y -= 5;
		}

		if (wParam == VK_DOWN) {
			moveXY.y += 5;
		}

		if (wParam == VK_LEFT)
		{
			moveXY.angle += 5;
		}

		if (wParam == VK_RIGHT) {
			moveXY.angle -= 5;
		}
		InvalidateRect(okno, 0, TRUE);
		return 0;
	}
	
	
	case WM_PAINT:
		{
			PAINTSTRUCT paint;
			HDC kontekst;

			kontekst = BeginPaint(okno, &paint);
		
			// MIEJSCE NA KOD GDI

			HPEN pioro = CreatePen(PS_SOLID, 10, RGB(255,0,0));
			
			DeleteObject(pioro);

			// utworzenie obiektu umo�liwiaj�cego rysowanie przy u�yciu GDI+
			// (od tego momentu nie mo�na u�ywa� funkcji GDI
			Graphics grafika(kontekst);
			
			// MIEJSCE NA KOD GDI+


			// utworzenie czcionki i wypisanie tekstu na ekranie
			FontFamily  fontFamily(L"Times New Roman");
			Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
			PointF      pointF(100.0f, 400.0f);
			SolidBrush  solidBrush(Color(255, 0, 0, 255));
			Pen blackPen(Color(255, 0, 0, 0), 3);

			

			//              x      y
			PointF rectangle1(200.0f, (200.0f + moveXY.y)); // lewy gorny
			PointF rectangle2(300.0f, (200.0f + moveXY.y)); // prawy gorny
			PointF rectangle3(300.0f, (400.0f + moveXY.y)); // prawy dolny
			PointF rectangle4(200.0f, (400.0f + moveXY.y)); // lewy dolny
			PointF rectangle[4] = { rectangle1, rectangle2, rectangle3, rectangle4 };
			PointF* pRectangle = rectangle;

			// right wind
			PointF RWind1(300.0f, 400.0f + moveXY.y); // lewy dolny
			PointF RWind2(350.0f, 450.0f + moveXY.y); // prawy dolny
			PointF RWind3(350.0f, 400.0f + moveXY.y); // prawy g�rny
			PointF RWind4(300.0f, 350.0f + moveXY.y); // lewy gorny
			PointF RWind[4] = { RWind1, RWind2, RWind3, RWind4 };
			PointF* pRWind = RWind;

			// left wind
			PointF LWind1(200.0f, 400.0f + moveXY.y); // prawy dolny
			PointF LWind2(150.0f, 450.0f + moveXY.y); // lewy dolny
			PointF LWind3(150.0f, 400.0f + moveXY.y); // lewy g�rny
			PointF LWind4(200.0f, 350.0f + moveXY.y); // prawy gorny
			PointF LWind[4] = { LWind1, LWind2, LWind3, LWind4 };
			PointF* pLWind = LWind;
			
			// base
			PointF Base1(220.0f, 400.0f + moveXY.y); // lewy gorny
			PointF Base2(190.0f, 450.0f + moveXY.y); // lewy dolny
			PointF Base3(310.0f, 450.0f + moveXY.y); // prawy dolny
			PointF Base4(280.0f, 400.0f + moveXY.y); // prawy gorny
			PointF Base[4] = { Base1, Base2, Base3, Base4 };
			PointF* pBase = Base;

			// header

			SolidBrush  czerwony(Color(255, 255, 0, 0));
			SolidBrush zielony(Color(255, 0, 255, 0));
			SolidBrush niebieski(Color(255, 0, 0, 255));


			//grafika.DrawPolygon(&blackPen, pRectangle, 4);
			grafika.FillPolygon(&czerwony, pRectangle, 4);
			
			//grafika.DrawPolygon(&blackPen, pRWind, 4);
			grafika.FillPolygon(&niebieski, pRWind, 4);


			//grafika.DrawPolygon(&blackPen, pLWind, 4);
			grafika.FillPolygon(&niebieski, pLWind, 4);

			//grafika.DrawPolygon(&blackPen, pBase, 4);
			grafika.FillPolygon(&zielony, pBase, 4);

			//grafika.DrawArc(&blackPen, 200, 160, 100, 80, 180, 180);
			grafika.FillPie(&niebieski, 200, 160 + moveXY.y, 100, 80, 180, 180);

			//grafika.DrawEllipse(&blackPen, 225, 220, 50, 50);
			grafika.FillEllipse(&zielony, 225, 220 + moveXY.y, 50, 50);


			EndPaint(okno, &paint);

			return 0;
		}
  	
	case WM_DESTROY: //obowi�zkowa obs�uga meldunku o zamkni�ciu okna
		PostQuitMessage (0) ;
		return 0;
    
	default: //standardowa obs�uga pozosta�ych meldunk�w
		return DefWindowProc(okno, kod_meldunku, wParam, lParam);
	}
}
