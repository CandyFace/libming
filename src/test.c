/*
  warning:
  this is not a concise demonstration of the c interface to ming.
  this is a shell for new functions so that I can easily load them into gdb
  and a quick way to check for linkage errors.
  I make no guarantee that this represents the latest incarnation of the ming
  api, even if it does compile..
*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "ming.h"
#include "libming.h"

#define WIDTH 320
#define HEIGHT 240


/* {{{ testCubic */

void testCubic(SWFMovie movie)
{
  int ax = rand()%WIDTH, ay = rand()%HEIGHT;
  int bx = rand()%WIDTH, by = rand()%HEIGHT;
  int cx = rand()%WIDTH, cy = rand()%HEIGHT;
  int dx = rand()%WIDTH, dy = rand()%HEIGHT;

  SWFShape shape = newSWFShape();
  SWFShape outline = newSWFShape();

  SWFShape_setLine(shape, 20, 0, 0, 0, 0xff);

  SWFShape_movePenTo(shape, ax, ay);
  SWFShape_drawCubic(shape, bx, by, cx, cy, dx, dy);

  SWFMovie_add(movie, (SWFCharacter)shape);

  SWFShape_movePenTo(outline, ax, ay);
  SWFShape_setLine(outline, 20, 0xff, 0, 0, 0x9f);
  SWFShape_drawLineTo(outline, bx, by);
  SWFShape_drawLineTo(outline, cx, cy);
  SWFShape_drawLineTo(outline, dx, dy);
  SWFShape_movePenTo(outline, ax, ay);
  SWFShape_setLine(outline, 100, 0xff, 0, 0, 0x9f);
  SWFShape_drawLine(outline, 1, 0);
  SWFShape_movePenTo(outline, bx, by);
  SWFShape_drawLine(outline, 1, 0);
  SWFShape_movePenTo(outline, cx, cy);
  SWFShape_drawLine(outline, 1, 0);
  SWFShape_movePenTo(outline, dx, dy);
  SWFShape_drawLine(outline, 1, 0);

  SWFMovie_add(movie, (SWFCharacter)outline);
}

/* }}} */
/* {{{ testDrawCharacter */

void testDrawCharacter(SWFMovie movie)
{
  SWFShape shape = newSWFShape();
  SWFFont f = loadSWFFontFromFile(fopen("test.fdb","rb"));
  SWFDisplayItem i;

  SWFFill f1, f2, f3, f4;

  if(f == NULL)
  {
    puts("test.fdb not found!");
    exit(1);
  }

  f1 = SWFShape_addSolidFill(shape, 0xff, 0, 0, 0xff);
  SWFShape_setRightFill(shape, f1);
  SWFShape_setLine(shape, 20, 0x7f, 0, 0, 0xff);
  SWFShape_drawFontGlyph(shape, f, '&');
  SWFShape_movePen(shape, SWFFont_getStringWidth(f, "&"), 0);

  f2 = SWFShape_addSolidFill(shape, 0xff, 0xff, 0, 0xff);
  SWFShape_setRightFill(shape, f2);
  SWFShape_setLine(shape, 20, 0x7f, 0x7f, 0, 0xff);
  SWFShape_drawFontGlyph(shape, f, 'a');
  SWFShape_movePen(shape, SWFFont_getStringWidth(f, "a"), 0);

  f3 = SWFShape_addSolidFill(shape, 0, 0xff, 0, 0xff);
  SWFShape_setRightFill(shape, f3);
  SWFShape_setLine(shape, 20, 0, 0x7f, 0, 0xff);
  SWFShape_drawFontGlyph(shape, f, 'b');
  SWFShape_movePen(shape, SWFFont_getStringWidth(f, "b"), 0);

  f4 = SWFShape_addSolidFill(shape, 0, 0, 0xff, 0xff);
  SWFShape_setRightFill(shape, f4);
  SWFShape_setLine(shape, 20, 0, 0, 0x7f, 0xff);
  SWFShape_drawFontGlyph(shape, f, 'c');

  SWFShape_end(shape);

  i = SWFMovie_add(movie, (SWFCharacter)shape);

  SWFDisplayItem_moveTo(i, 100, 1600);
}

/* }}} */
/* {{{ testDBL */

void testDBL(SWFMovie movie)
{
  SWFFill fill;
  SWFBitmap dbl;
  SWFShape shape;
  SWFDisplayItem i;
  FILE *file = fopen("test.dbl", "rb");

  if(!file)
  {
    printf("Couldn't find test.dbl file!\n");
    exit(1);
  }

  dbl = newSWFDBLBitmap(file);
  shape = newSWFShape();

  fill = SWFShape_addBitmapFill(shape, dbl, SWFFILL_CLIPPED_BITMAP);
  SWFShape_setRightFill(shape, fill);
  SWFShape_drawLine(shape, 640, 0);
  SWFShape_drawLine(shape, 0, 640);
  SWFShape_drawLine(shape, -640, 0);
  SWFShape_drawLine(shape, 0, -640);

  i = SWFMovie_add(movie, (SWFCharacter)shape);
}

/* }}} */
/* {{{ testMorph */

void testMorph(SWFMovie movie)
{
  SWFMorph morph = newSWFMorphShape();
  SWFShape shape1 = SWFMorph_getShape1(morph);
  SWFShape shape2 = SWFMorph_getShape2(morph);
  SWFDisplayItem d;
  int i;

  SWFShape_movePenTo(shape1, 0, 0);
  SWFShape_movePenTo(shape2, 0, 0);

  SWFShape_setLineStyle(shape1, 20, 0, 0, 0, 0xff);
  SWFShape_setLineStyle(shape2, 20, 0, 0, 0, 0xff);

  for(i=0; i<20; ++i)
  {
    SWFShape_drawLineTo(shape1, rand()%2000-1000, rand()%2000-1000);
    SWFShape_drawLineTo(shape2, rand()%2000-1000, rand()%2000-1000);
  }

  d = SWFMovie_add(movie, (SWFCharacter)morph);

  for(i=0; i<10; ++i)
  {
    SWFDisplayItem_setRatio(d, (i*65535)/10);
    SWFMovie_nextFrame(movie);
  }
  for(; i>0; --i)
  {
    SWFDisplayItem_setRatio(d, (i*65535)/10);
    SWFMovie_nextFrame(movie);
  }
}

/* }}} */
/* {{{ testJpeg */

void testJpeg(SWFMovie movie)
{
  SWFFill fill;
  SWFBitmap jpeg;
  SWFShape shape = newSWFShape();
  SWFDisplayItem i;
  SWFInput input;
  FILE *file = fopen("test.jpg", "rb");

  if(!file)
  {
    printf("Couldn't find test.jpg file!\n");
    exit(1);
  }

  input = newSWFInput_file(file);
  jpeg = newSWFBitmap_fromInput(input);

  i = SWFMovie_add(movie, jpeg);
  SWFDisplayItem_moveTo(i,100,100);
  SWFDisplayItem_rotate(i,45.0);
}

/* }}} */
/* {{{ testJpegAlpha */

void testJpegAlpha(SWFMovie movie)
{
  SWFFill fill;
  SWFBitmap jpeg;
  SWFShape shape = newSWFShape();
  SWFDisplayItem i;
  FILE *file = fopen("test.jpg", "rb");
  FILE *alpha = fopen("test.msk", "rb");

  if(!file)
  {
    printf("Couldn't find test.jpg file!\n");
    exit(1);
  }
  if(!alpha)
  {
    printf("Couldn't find test.msk file!\n");
    exit(1);
  }

  jpeg = newSWFJpegWithAlpha(file, alpha);
  fill = SWFShape_addBitmapFill(shape, jpeg, SWFFILL_TILED_BITMAP);
  SWFShape_setRightFill(shape, fill);
  SWFShape_drawCharacterBounds(shape, (SWFCharacter)jpeg);

  i = SWFMovie_add(movie, (SWFCharacter)shape);
  SWFDisplayItem_scale(i, 20.0, 20.0);
}

/* }}} */
/* {{{ testText */

void testText(SWFMovie movie)
{
  SWFFont f = loadSWFFontFromFile(fopen("test.fdb","rb"));
  SWFText t = newSWFText();
  SWFDisplayItem d;

  if(f == NULL)
  {
    printf("font def file not found!\n");
    exit(1);
  }

  SWFText_setFont(t, f);
  SWFText_setHeight(t, 24);
  SWFText_setColor(t, 0, 0, 0, 0xff);
  SWFText_setSpacing(t, 0);
  SWFText_addString(t, "blargghghgghghgh", NULL);

  SWFText_setXY(t, -1, 48);
  SWFText_setHeight(t, 48);
  SWFText_setSpacing(t, 10);
  SWFText_addString(t, "blargghghgghghgh", NULL);

  d = SWFMovie_add(movie, (SWFCharacter)t);
  SWFDisplayItem_move(d, 10, 100);
}

/* }}} */
/* {{{ testLoadSWF */

/* to test renumbering, some other tags must be present, e.g. text */
void testLoadSWF(SWFMovie movie)
{
  SWFFont f = loadSWFFontFromFile(fopen("test.fdb","rb"));
  SWFText t = newSWFText();
  SWFDisplayItem d;

  SWFPrebuiltClip clip;

  if(f == NULL)
  {
    printf("font def file not found!\n");
    exit(1);
  }

  SWFText_setFont(t, f);
  SWFText_setHeight(t, 24);
  SWFText_setColor(t, 0, 0, 0, 0xff);
  SWFText_setSpacing(t, 0);
  SWFText_addString(t, "blargghghgghghgh", NULL);

  d = SWFMovie_add(movie, (SWFCharacter)t);
  SWFDisplayItem_move(d, 10, 100);

  clip = newSWFPrebuiltClip_fromFile("import.swf");
  d = SWFMovie_add(movie, (SWFCharacter)clip);
  SWFDisplayItem_move(d, 10, 100);
}

/* }}} */
/* {{{ testShape */

void testShape(SWFMovie movie)
{
  int i;
  SWFDisplayItem d;
  SWFShape s = newSWFShape();

  SWFFill f1 = SWFShape_addSolidFill(s, 0xff, 0, 0, 0xff);
  SWFFill f2 = SWFShape_addSolidFill(s, 0, 0, 0xff, 0xff);

  SWFShape_movePenTo(s, -1200, -1200);
  SWFShape_setLineStyle(s, 20, 0, 0, 0, 0);
  SWFShape_setRightFill(s, f1);
  SWFShape_drawLine(s, 2400, 0);
  SWFShape_setRightFill(s, f2);
  SWFShape_drawLine(s, 0, 2400);
  SWFShape_setRightFill(s, f1);
  SWFShape_drawLine(s, -2400, 0);
  SWFShape_setRightFill(s, f2);
  SWFShape_drawLine(s, 0, -2400);
  SWFShape_end(s);

  d = SWFMovie_add(movie, (SWFCharacter)s);

  for(i=0; i<45; ++i)
  {
    SWFDisplayItem_rotate(d, 360/45);
    SWFDisplayItem_move(d, 160*20, 120*20);
    SWFMovie_nextFrame(movie);
    SWFDisplayItem_move(d, -160*20, -120*20);
  }
}

/* }}} */
/* {{{ testImport */

void testImport(SWFMovie movie)
{
  int i;
  SWFDisplayItem d;
  SWFCharacter s = SWFMovie_importCharacter(movie, "library.swf", "square");

  d = SWFMovie_add(movie, s);

/*  for(i=0; i<45; ++i)
  {
    SWFDisplayItem_rotate(d, 360/45);
    SWFDisplayItem_move(d, 160*20, 120*20);
    SWFMovie_nextFrame(movie);
    SWFDisplayItem_move(d, -160*20, -120*20);
  }*/
}

/* }}} */
/* {{{ testImportFont */

void testImportFont(SWFMovie movie)
{
  SWFCharacter f = SWFMovie_importFont(movie, "emfonts.swf", "Arial");
//  SWFCharacter f = SWFMovie_importFont(movie, "lib.swf", "myfont");
  SWFTextField t = newSWFTextField();
  SWFDisplayItem d;

  SWFTextField_setFlags(t, SWFTEXTFIELD_DRAWBOX|SWFTEXTFIELD_HTML|SWFTEXTFIELD_USEFONT);
  SWFTextField_setFont(t, f);
  SWFTextField_setHeight(t, 24);
  SWFTextField_setColor(t, 0, 0, 0, 0xff);
  SWFTextField_setBounds(t, 200, 24);
//  SWFTextField_addString(t, "<font face=\"AmericanUncD\">test</font>");

  d = SWFMovie_add(movie, (SWFCharacter)t);
  SWFDisplayItem_move(d, 10, 100);
}
/* {{{ testMp3 */

void testMp3(SWFMovie movie)
{
  FILE *f;
  SWFSoundStream sound;
  int n;

  if(!(f = fopen("/mnt/progs/ming-examples/common/distortobass.mp3", "rb")))
  {
    printf("Couldn't find test.mp3 file\n");
    exit(1);
  }

  sound = newSWFSoundStream(f);
  SWFMovie_setSoundStream(movie, sound);
  n = SWFSoundStream_getFrames(sound);
printf ("frames %d\n", n);
n = 160;
  SWFMovie_setNumberOfFrames(movie, n);
}

/* }}} */
/* {{{ testSound1 */

void testSound1(SWFMovie movie)
{
  FILE *f;
  SWFSound sound;

  if(!(f = fopen("/tmp/sounds/rooster.au", "rb")))
  {
    printf("Couldn't find test.mp3 file\n");
    exit(1);
  }

  sound = newSWFSound(f, SWF_SOUND_11KHZ);

  SWFMovie_startSound(movie, sound);
  SWFMovie_setNumberOfFrames(movie, 40);
}

/* }}} */
/* {{{ testSound2 */

void testSound2(SWFMovie movie)
{
  FILE *f;
  SWFSound sound;
  SWFShape shape;
  SWFButton button;
  SWFFill f1;

  shape = newSWFShape();
  f1 = SWFShape_addSolidFill(shape, 0xff, 0, 0, 0xff);
  SWFShape_setRightFill(shape, f1);
  SWFShape_drawLine(shape, 100, 0);
  SWFShape_drawLine(shape, -50, 80);
  SWFShape_drawLine(shape, -50, -80);
  button = newSWFButton();
  SWFButton_addShape(button, shape, SWFBUTTON_UP|SWFBUTTON_DOWN|SWFBUTTON_OVER|SWFBUTTON_HIT);

  if(!(f = fopen("/tmp/sounds/rooster.au", "rb")))
  {
    printf("Couldn't find test.mp3 file\n");
    exit(1);
  }

  sound = newSWFSound(f, SWF_SOUND_11KHZ);
  SWFButton_addSound(button, sound, 2);
  SWFMovie_add(movie, button);
}

/* }}} */

int main(int argc, char *argv[])
{
  FILE *f;
  SWFMovie m = newSWFMovieWithVersion(6);
  int length;

  Ming_init();

  srand(time(NULL));

  f = fopen("test.swf", "wb");

  SWFMovie_setRate(m, 15.0);
  SWFMovie_setDimension(m, WIDTH, HEIGHT);
  SWFMovie_setNumberOfFrames(m, 1);

  //testCubic(m);
  //testDrawCharacter(m);
  //testMorph(m);
  //testShape(m);
  //testText(m);
  //testSound2(m);
  //testDBL(m);
  //testJpeg(m);
  //testJpegAlpha(m);
  //testMp3(m);
  //testImport(m);
  //testImportFont(m);
  testLoadSWF(m);

  length = SWFMovie_output(m, fileOutputMethod, f);
  destroySWFMovie(m);

  printf("%i bytes written\n", length);

  exit(0);
}
