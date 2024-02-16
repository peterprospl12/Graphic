
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

int red, green, blue, yellow;
unsigned long foreground, background;

typedef struct {
    int x; // Współrzędna x symbolu
    int y; // Współrzędna y symbolu
    int width; // Szerokość symbolu
    int height; // Wysokość symbolu
    int dodx;
    int dody;
    // Dodaj dodatkowe pola, jeśli są potrzebne
    int isSet;
} Symbol;

//*************************************************************************************************************************
//funkcja przydzielania kolorow

int AllocNamedColor(char *name, Display* display, Colormap colormap)
  {
    XColor col;
    XParseColor(display, colormap, name, &col);
    XAllocColor(display, colormap, &col);
    return col.pixel;
  } 

//*************************************************************************************************************************
// inicjalizacja zmiennych globalnych okreslajacych kolory

int init_colors(Display* display, int screen_no, Colormap colormap)
{
  background = WhitePixel(display, screen_no);  //niech tlo bedzie biale
  foreground = BlackPixel(display, screen_no);  //niech ekran bedzie czarny
  red=AllocNamedColor("red", display, colormap);
  green=AllocNamedColor("green", display, colormap);
  blue=AllocNamedColor("blue", display, colormap);
  yellow=AllocNamedColor("yellow", display, colormap);
}

//*************************************************************************************************************************
// Glowna funkcja zawierajaca petle obslugujaca zdarzenia */

int main(int argc, char *argv[])
{
  char            icon_name[] = "Grafika";
  char            title[]     = "Grafika komputerowa";
  Display*        display;    //gdzie bedziemy wysylac dane (do jakiego X servera)
  Window          window;     //nasze okno, gdzie bedziemy dokonywac roznych operacji
  GC              gc;         //tu znajduja sie informacje o parametrach graficznych
  XEvent          event;      //gdzie bedziemy zapisywac pojawiajace sie zdarzenia
  KeySym          key;        //informacja o stanie klawiatury 
  Colormap        colormap;
  int             screen_no;
  XSizeHints      info;       //informacje typu rozmiar i polozenie ok
  
  char            buffer[8];  //gdzie bedziemy zapamietywac znaki z klawiatury
  int             hm_keys;    //licznik klawiszy
  int             to_end;

  display    = XOpenDisplay("");                //otworz polaczenie z X serverem pobierz dane od zmiennej srodowiskowej DISPLAY ("")
  screen_no  = DefaultScreen(display);          //pobierz domyslny ekran dla tego wyswietlacza (0)
  colormap = XDefaultColormap(display, screen_no);
  init_colors(display, screen_no, colormap);

  int add_x = 0;
  int add_y = 0;
   XPoint points[] = {
        //x     y
        {110 , 100 }, // 1 lewy gorny
        {180 , 100 }, // 2 prawy gorny
        {180 , 120 }, // 3
        {130 , 120 },  // 4
        {130 , 150 },
        {180 , 150 },  // 6
        {180 , 220 },
        {160 , 240 }, // 8
        {100 , 240 },
        {80 , 210 }, // 10
        {100 ,210 },
        {110 ,222 },
        {150 ,222 },
        {160 ,210 },
        {160 , 170 },
        {110 , 170 }
    };
  Symbol piatki[30];
  int pointsCounter = sizeof(points) / sizeof(points[0]);
  int piatkicounter = 0;

  //okresl rozmiar i polozenie okna
  info.x = 100;
  info.y = 150;
  info.width = 1000;
  info.height = 800;
  info.flags = PPosition | PSize;

  //majac wyswietlacz, stworz okno - domyslny uchwyt okna
  window = XCreateSimpleWindow(display, DefaultRootWindow(display),info.x, info.y, info.width, info.height, 7/* grubosc ramki */, foreground, background);
  XSetStandardProperties(display, window, title, icon_name, None, argv, argc, &info);
  //utworz kontekst graficzny do zarzadzania parametrami graficznymi (0,0) domyslne wartosci
  gc = XCreateGC(display, window, 0, 0);
  XSetBackground(display, gc, background);
  XSetForeground(display, gc, foreground);

  //okresl zdarzenia jakie nas interesuja, np. nacisniecie klawisza
  XSelectInput(display, window, (KeyPressMask | ExposureMask | ButtonPressMask| ButtonReleaseMask | Button1MotionMask));
  XMapRaised(display, window);  //wyswietl nasze okno na samym wierzchu wszystkich okien
      
  to_end = FALSE;

 /* petla najpierw sprawdza, czy warunek jest spelniony
     i jesli tak, to nastepuje przetwarzanie petli
     a jesli nie, to wyjscie z petli, bez jej przetwarzania */
  while (to_end == FALSE)
  {
    XNextEvent(display, &event);  // czekaj na zdarzenia okreslone wczesniej przez funkcje XSelectInput

    switch(event.type)
    {
      case Expose:
        if (event.xexpose.count == 0)
        {
         //XFillPolygon(display, window, gc, points, pointsCounter, Nonconvex, CoordModeOrigin);
         //XDrawLines(display, window, gc, points, pointsCounter, CoordModeOrigin);
    //XFillArc(display, window, gc, 70, 208, 50, 32, 90 * 64, 270 * 64);        }
    
        break;

      case MotionNotify:
         
        break;

      case ButtonPress:
        if (event.xbutton.button == Button1) {
    add_x = event.xbutton.x - 100;
    add_y = event.xbutton.y - 100;
    
    if(piatkicounter==0){
        for (int i = 0; i < pointsCounter; i++) {
                points[i].x += add_x;
                points[i].y += add_y;
            }

                
           

            piatki[piatkicounter].x = points[piatkicounter].x; 
            piatki[piatkicounter].y = points[piatkicounter].y;
            piatki[piatkicounter].height = 140;
                piatki[piatkicounter].isSet= 1;
            piatki[piatkicounter++].width = 100; 
            


    }


    for(int i=0;i<piatkicounter;i++){
    if(event.xbutton.x >= piatki[i].x && event.xbutton.x <= piatki[i].x + piatki[i].width && event.xbutton.y >= piatki[i].y && event.xbutton.y <= piatki[i].y + piatki[i].height ) {
      piatki[i].isSet = FALSE;
    }
    else{
        for (int i = 0; i < pointsCounter; i++) {
                points[i].x += add_x;
                points[i].y += add_y;
            }

                piatki[piatkicounter].isSet= 1;            


            for (int i = 0; i < pointsCounter; i++) {
                points[i].x -= add_x;
                points[i].y -= add_y;
            }
    }
    XSetForeground(display, gc, foreground);
          XClearWindow(display, window);
    for(int i=0; i< piatkicounter; i++) {
      if(piatki[i].isSet) {
          XFillPolygon(display, window, gc, points, pointsCounter, Nonconvex, CoordModeOrigin);
          XFillArc(display, window, gc, 70 + add_x, 208 + add_y, 50, 32, 90 * 64, 270 * 64);
      }
    }


    
    }
        }


}

        break;

      case ButtonRelease:
       
      break;
      case KeyPress:
        hm_keys = XLookupString(&event.xkey, buffer, 8, &key, 0);
        if (hm_keys == 1)
        {
          if (buffer[0] == 'q') to_end = TRUE;        // koniec programu
          
        }

      default:
        break;
    }
  }

  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);

  return 0;
}
