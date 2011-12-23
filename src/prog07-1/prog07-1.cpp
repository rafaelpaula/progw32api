//--------------------------------------------------------------------------
// Programa��o Windows: C e Win32 API com �nfase em Multim�dia
// Autor: Andr� Kishimoto
// ISBN 85-906129-1-0
// Copyright (c) 2004-2006, Andr� Kishimoto
//
// prog07-1.cpp - Regi�es
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>

//--------------------------------------------------------------------------
// Defini��es
//--------------------------------------------------------------------------
// Nome da classe da janela 
#define WINDOW_CLASS		"prog07-1"
// T�tulo da janela 
#define WINDOW_TITLE		"Prog 07-1"
// Largura da janela 
#define WINDOW_WIDTH		320
// Altura da janela 
#define WINDOW_HEIGHT		240

//--------------------------------------------------------------------------
// Prot�tipo das fun��es
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

//--------------------------------------------------------------------------
// WinMain() -> Fun��o principal
//--------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
  // Cria a classe da janela e especifica seus atributos
  WNDCLASSEX wcl;
  wcl.cbSize = sizeof(WNDCLASSEX);
  wcl.style = CS_HREDRAW | CS_VREDRAW;
  wcl.lpfnWndProc = (WNDPROC)WindowProc;
  wcl.cbClsExtra = 0;
  wcl.cbWndExtra = 0;
  wcl.hInstance = hInstance;
  wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wcl.lpszMenuName = NULL;
  wcl.lpszClassName = WINDOW_CLASS;
  wcl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  // Registra a classe da janela
  if(RegisterClassEx(&wcl))
  {
    // Cria a janela principal do programa
    HWND hWnd = NULL;
    hWnd = CreateWindowEx(
           NULL,
		   WINDOW_CLASS,
		   WINDOW_TITLE,
		   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		   (GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2,
		   (GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2,
		   WINDOW_WIDTH,
		   WINDOW_HEIGHT,
		   HWND_DESKTOP,
		   NULL,
		   hInstance,
		   NULL);

    // Verifica se a janela foi criada
    if(hWnd)
    {
      // Mostra a janela
      ShowWindow(hWnd, nCmdShow);
      UpdateWindow(hWnd);

	  // Armazena dados da mensagem que ser� obtida
      MSG msg;

      // Loop de mensagens, enquanto mensagem n�o for WM_QUIT,
      // obt�m mensagem da fila de mensagens
      while(GetMessage(&msg, NULL, 0, 0) > 0)
      {
        // Traduz teclas virtuais ou aceleradoras (de atalho)
        TranslateMessage(&msg);

        // Envia mensagem para a fun��o que processa mensagens (WindowProc)
        DispatchMessage(&msg);
      }

      // Retorna ao Windows com valor de msg.wParam
      return((int)msg.wParam);
     }
     // Se a janela n�o foi criada
     else
     {
       // Exibe mensagem de erro e sai do programa
       MessageBox(NULL, "N�o foi poss�vel criar janela.", "Erro!", MB_OK | MB_ICONERROR);

       return(0);
     }
  }
  // Se a classe da janela n�o foi registrada
  else
  {
    // Exibe mensagem de erro e sai do programa
    MessageBox(NULL, "N�o foi poss�vel registrar a classe da janela.", "Erro!", MB_OK | MB_ICONERROR);

    return(0);
  }
	
  // Retorna ao Windows sem passar pelo loop de mensagens
  return(0);
}

//--------------------------------------------------------------------------
// WindowProc() -> Processa as mensagens enviadas para o programa
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  // Vari�veis para manipula��o da parte gr�fica do programa
  HDC hDC = NULL;
  PAINTSTRUCT psPaint;

	// Identificadores da regi�o, bitmap e DC de mem�ria
	static HRGN hRgn = NULL;
	static HBITMAP hBmp = NULL;
	static HDC hMemDC = NULL;
  
  // Verifica qual foi a mensagem enviada
  switch(uMsg)
  {
    case WM_CREATE: // Janela foi criada
    {
			// Cria uma regi�o el�ptica
			hRgn = CreateEllipticRgn(80, 50, 230, 200);

      // Cria DC de mem�ria (hMemDC � global)
      hDC = GetDC(hWnd);
      hMemDC = CreateCompatibleDC(hDC);

			// Cria DIB Section, carregando bitmap do arquivo "frutas_24bit.bmp"
			hBmp = (HBITMAP)LoadImage(NULL, "frutas_24bit.bmp", IMAGE_BITMAP, 320, 240, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

			// Seleciona bitmap no DC de mem�ria (configura DC de mem�ria)
      SelectObject(hMemDC, hBmp);

			// Libera DC de v�deo
			ReleaseDC(hWnd, hDC);

      // Retorna 0, significando que a mensagem foi processada corretamente
      return(0);
    } break;

    case WM_PAINT: // Janela (ou parte dela) precisa ser atualizada
    {
      // Obt�m DC de v�deo
      hDC = BeginPaint(hWnd, &psPaint);

			// Seleciona regi�o no DC, definindo regi�o de corte
			SelectObject(hDC, hRgn);

			// Faz transfer�ncia de bits entre os DC's de mem�ria e v�deo
			BitBlt(hDC, 0, 0, 320, 240, hMemDC, 0, 0, SRCCOPY);

      // Libera DC de v�deo
      EndPaint(hWnd, &psPaint);

      return(0);
    } break;

    case WM_CLOSE: // Janela foi fechada
    {
      // Deleta regi�o
      DeleteObject(SelectObject(hDC, hRgn));

			// Deleta bitmap
      DeleteObject(SelectObject(hMemDC, hBmp));

      // Deleta DC de mem�ria
      DeleteDC(hMemDC);

			// Destr�i a janela
      DestroyWindow(hWnd);

      return(0);
    } break;

    case WM_DESTROY: // Janela foi destru�da
    {
      // Envia mensagem WM_QUIT para o loop de mensagens
      PostQuitMessage(0);

      return(0);
    } break;

		default: // Outra mensagem
    {
      /* Deixa o Windows processar as mensagens que n�o foram verificadas na fun��o */
			return(DefWindowProc(hWnd, uMsg, wParam, lParam));
    } break;

  }
}