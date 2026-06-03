/* Sosnovskaya Ekaterina, 10-3, 01.06.2026 */
#include <windows.h>
#include <string.h>
#define True 1
void main(void)
{
  char s[100], Info[] = "cgsg pml 30", weu[100];
  int a = 0, b = 100, m = (a+b)/2, answer, result;

  wsprintf(s, "hello. now we play the game");
  if(MessageBox(NULL, s, weu, MB_OK | MB_ICONQUESTION) == IDOK)
  {
   wsprintf(s, "if your number more than %i?", m);
    while(True)
    {
      m = (a+b)/2;
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
    }
    wsprintf(s, "your number is %i ", result);
  }
  getchar();
}
/*
void main(void)
{
  printf("hi");
  getchar();
}
  */

