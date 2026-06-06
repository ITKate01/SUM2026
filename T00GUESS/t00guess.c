/* Sosnovskaya Ekaterina, 10-3, 01.06.2026 */
#include <windows.h>
#include <stdio.h>
#define True 1

void main(void)
{
  char s[256], Info[] = "CGSG Summer practice", weu[100] = "PLAY: guess practice";
  int a = 0, b = 100, m, answer, result = -1;
  
  wsprintf(s, "Guess number from 0 to 100.\nPlease tap the button");
  MessageBox(NULL, s, weu, MB_OK | MB_ICONINFORMATION);
  while(True)
  {
    m = (a + b) / 2;

    wsprintf(s, "Is your number more than %d?", m);
    answer = MessageBox(NULL, s, Info, MB_YESNOCANCEL);
    if(answer == IDYES)
    {
      a = m + 1;
    }
    else if(answer == IDNO)
    {
      b = m - 1;
    }
    else if(answer == IDCANCEL)
    {
      result = m;
      break;
    }
    
    if(a > b)
    {
      wsprintf(s, "ACHTUUUUUUUUUUUUUUUUUNG! Dein Programm verrect, ist es vorbei. You answered incorrectly(((\nNumber is not find.");
      MessageBox(NULL, s, Info, MB_OK | MB_ICONERROR);
      return;
    }
  }
  wsprintf(s, "Your number: %d! danke furs Spielen.", result);
  MessageBox(NULL, s, weu, MB_OK | MB_ICONINFORMATION);
  
  getchar();
}
