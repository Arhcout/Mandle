#include <complex.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define MAX_ITER 500

#define SCALE_STEP 1.0

#define SPEED 20

#define LUM_SIZE (255 - 232)

int f(complex double c) {
  complex double z = 0;
  for (int i = 0; i < MAX_ITER; i++) {
    if (cabs(z) >= 2) {
      return i;
    }
    z = z * z + c;
  }
  return 0;
}

int main() {
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);

  double scale = 16;

  double ox = 0, oy = 0;

  struct termios t, oldT;
  tcgetattr(STDIN_FILENO, &t);
  oldT = t;
  t.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &t);

  bool running = true;
  while (running) {
    for (int y = 0; y < w.ws_row; y++) {
      for (int x = 0; x < w.ws_col; x++) {
        complex double c = ((x - (double)w.ws_col / 2) / scale + ox) +
                           ((y - (double)w.ws_row / 2) / scale * 2 + oy) * I;

        double lum = f(c);

        int normLum = round(
            (lum - 0) / (MAX_ITER - 1 - 0) * ((double)LUM_SIZE - 1 - 0) + 0);

        printf("\033[38;5;%dm█", 232 + normLum);
      }
    }
    char c = getchar();
    if (c == '\033') {
      getchar();
      c = getchar();
      switch (c) {
      case 'A':
        oy -= SPEED / scale * 2;
        break;
      case 'B':
        oy += SPEED / scale * 2;
        break;
      case 'C':
        ox += SPEED / scale;
        break;
      case 'D':
        ox -= SPEED / scale;
        break;
      }
    }
    switch (c) {
    case '=':
      scale += scale;
      break;
    case '-':
      scale -= scale / 2;
      if (scale <= 0) {
        scale = 0.001;
      }
      break;
    case 'q':
      running = false;
      break;
    }
    printf("\033[2J");
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &oldT);
}
