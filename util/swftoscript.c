
/* convert everything to objects, resolve dependencies, etc.. */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

//open()
#include <fcntl.h>

//fstat()
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//decompression
#include <zlib.h>

#include "blocktypes.h"
#include "action.h"
#include "read.h"
#include "decompile.h"

#include "swftoscript.h"

#ifndef M_PI
  #define M_PI 3.14159265358979f
#endif

char *filename;
char tmp_name[PATH_MAX];
FILE *tempfile;

void skipBytes(FILE *f, int length);
void silentSkipBytes(FILE *f, int length);
//char *blockName(int);
void printSoundInstance(FILE *f, int id, int soundid);

static char *modNamePrefix()
{
#ifdef SWFPERL
	return "SWF::";
#endif
#ifdef SWFPHP
	return "SWF";
#endif
}

#ifdef SWFPERL
#define COMMSTART "#"
#define COMMEND   ""
#endif
#ifdef SWFPHP
#define COMMSTART "/* "
#define COMMEND   "*/"
#endif

static int m_version = {0};
void decompileAction(FILE *f, int length, int indent)
{	if(m_version >= 5)
		decompile5Action(f, length, indent);
	else if(m_version > 3)
		decompile4Action(f, length, indent);
	else
		while(--length >= 0)
			readUInt8(f);
}

void readMatrix(FILE *f, struct Matrix *s)
{
  int nBits;

  byteAlign();

  if(readBits(f, 1)) /* has scale */
  {
    nBits = readBits(f, 5);
    s->xScale = (float)readSBits(f, nBits)/0x10000;
    s->yScale = (float)readSBits(f, nBits)/0x10000;
  }
  else
    s->xScale = s->yScale = 1.0;

  if(readBits(f, 1)) /* has rotate */
  {
    nBits = readBits(f, 5);
    s->rot0 = (float)readSBits(f, nBits)/0x10000;
    s->rot1 = (float)readSBits(f, nBits)/0x10000;
  }
  else
    s->rot0 = s->rot1 = 0;

  nBits = readBits(f, 5);
  s->x = readSBits(f, nBits);
  s->y = readSBits(f, nBits);
}

void readCXForm(FILE *f, struct CXForm *s, boolean hasAlpha)
{
  int hasAdd, hasMult, nBits;

  byteAlign();

  hasAdd = readBits(f, 1);
  hasMult = readBits(f, 1);
  nBits = readBits(f, 4);

  if(hasMult)
  {
    s->rMult = readSBits(f, nBits);
    s->gMult = readSBits(f, nBits);
    s->bMult = readSBits(f, nBits);

    if(hasAlpha)
      s->aMult = readSBits(f, nBits);
    else
      s->aMult = 0x100;
  }
  else
  {
    s->aMult = 0x100;
    s->rMult = 0x100;
    s->gMult = 0x100;
    s->bMult = 0x100;
  }

  if(hasAdd)
  {
    s->rAdd = readSBits(f, nBits);
    s->gAdd = readSBits(f, nBits);
    s->bAdd = readSBits(f, nBits);

    if(hasAlpha)
      s->aAdd = readSBits(f, nBits);
    else
      s->aAdd = 0;
  }
  else
  {
    s->aAdd = 0;
    s->rAdd = 0;
    s->gAdd = 0;
    s->bAdd = 0;
  }
}

void readRect(FILE *f, struct Rect *s)
{
  int nBits;

  byteAlign();

  nBits = readBits(f, 5);
  s->xMin = readSBits(f, nBits);
  s->xMax = readSBits(f, nBits);
  s->yMin = readSBits(f, nBits);
  s->yMax = readSBits(f, nBits);
}

void readRGB(FILE *f, struct RGBA *s)
{
  s->r = readUInt8(f);
  s->g = readUInt8(f);
  s->b = readUInt8(f);
  s->a = 0xff;
}

void readRGBA(FILE *f, struct RGBA *s)
{
  s->r = readUInt8(f);
  s->g = readUInt8(f);
  s->b = readUInt8(f);
  s->a = readUInt8(f);
}

void printRGBA(struct RGBA *s)
{
  printf("0x%02x, 0x%02x, 0x%02x", s->r, s->g, s->b);

  if(s->a != 0xff)
    printf(", 0x%02x", s->a);
}

void readGradient(FILE *f, struct Gradient *s, Blocktype shapeType)
{
  int i;
  int numGrads = readUInt8(f);

  s->nGradients = numGrads;
  s->colors = malloc(sizeof(struct RGBA)*numGrads);
  s->ratio = malloc(sizeof(int)*numGrads);

  for(i=0; i<numGrads; ++i)
  {
    s->ratio[i] = readUInt8(f);

    if(shapeType==DEFINESHAPE3)
      readRGBA(f, &(s->colors[i]));
    else
      readRGB(f, &(s->colors[i]));
  }
}

void printGradient(struct Gradient *g)
{
  int i;

  printf("\t$g = new %sGradient();\n",modNamePrefix());

  for(i=0; i<g->nGradients; ++i)
  {
    printf("\t$g->addEntry(%f, ", g->ratio[i]/255.0);
    printRGBA(&(g->colors[i]));
    printf(");\n");
  }
}

void readLineStyleArray(FILE *f, struct Shape *shape, int isMorph)
{
  struct LineStyles *s;
  struct LineStyle *l;
  int count, i;
  int start = shape->lines.nLines;

  shape->lineOffset = start;

  count = readUInt8(f);

  if(count==255)
    count = readUInt16(f);

  s = &(shape->lines);

  s->nLines += count;
  s->line = realloc(s->line, s->nLines * sizeof(struct LineStyle));

  for(i=0; i<count; ++i)
  {
    l = &(s->line[start+i]);
    l->width = readUInt16(f);

    if(isMorph)
      l->width2 = readUInt16(f);

    if(shape->shapeType == DEFINESHAPE3)
      readRGBA(f, &(l->color));
    else
      readRGB(f, &(l->color));

    if(isMorph)
      readRGBA(f, &(l->color2));
  }
}

void readFillStyle(FILE *f, struct FillStyle *s, Blocktype shapeType, int isMorph)
{
  int type;

  type = readUInt8(f);
  s->type = type;

  if(type==0) /* solid fill */
  {
    if(shapeType==DEFINESHAPE3)
      readRGBA(f, &(s->fill.color));
    else
      readRGB(f, &(s->fill.color));

    if(isMorph)
      readRGBA(f, &(s->fill2.color));
  }
  else if(type==0x10 || type==0x12)
  {
    /* linear (0x10) or radial (0x10) gradient */

    readMatrix(f, &(s->matrix));

    if(isMorph)
      readMatrix(f, &(s->matrix2));

    readGradient(f, &(s->fill.gradient), shapeType);
  }
  else if(type==0x40 || type==0x41)
  {
    /* tiled bitmap (0x40) or clipped bitmap (0x41) fill */

    s->fill.bitmap = readUInt16(f);
    readMatrix(f, &(s->matrix));

    if(isMorph)
      readMatrix(f, &(s->matrix2));
  }
  else
    error("Unknown fill type: %i\n", type);
}

void printTransform(struct Matrix *m, char ch, int num)
{
  float a = m->xScale, b = m->rot0, c = m->rot1, d = m->yScale;

  float angle;
  float xScale = sqrt(a*a+c*c);
  float yScale = (a*d-b*c)/xScale;
  float skew = (a*b+c*d)/(a*a+c*c);

  if(a==0)
  {
    if(c<0)
      angle = -90;
    else
      angle = 90;
  }
  else
  {
    angle = atan(c/a)*180/M_PI;

    if(a<0)
    {
      if(angle < 0)
	angle += 180;
      else
	angle -= 180;
    }
  }

#define TOLERANCE 0.02

  if(skew < -TOLERANCE || skew > TOLERANCE)
    printf("\t$%c%i->skewXTo(%f);\n", ch, num, skew);

  if(xScale > 1.0-TOLERANCE && xScale < 1.0+TOLERANCE)
    xScale = 1.0;

  if(yScale > 1.0-TOLERANCE && yScale < 1.0+TOLERANCE)
    yScale = 1.0;

  if(xScale != 1.0 || yScale != 1.0)
  {
    if(xScale == yScale)
      printf("\t$%c%i->scaleTo(%f);\n", ch, num, xScale);
    else
      printf("\t$%c%i->scaleTo(%f, %f);\n", ch, num, xScale, yScale);
  }

  if(angle < -TOLERANCE || angle > TOLERANCE)
    printf("\t$%c%i->rotateTo(%f);\n", ch, num, angle);

  if(m->x != 0 || m->y != 0)
    printf("\t$%c%i->moveTo(%i, %i);\n", ch, num, m->x, m->y);
}

void printFillStyle(struct FillStyle *s, int id, int num, int isMorph)
{
  switch(s->type)
  {
    case 0:
      return;

    case 0x10:
      printGradient(&(s->fill.gradient));
      printf("\t$f%i = $s%i->addFill($g, SWFFILL_LINEAR_GRADIENT);\n", num, id);
      break;

    case 0x12:
      printGradient(&(s->fill.gradient));
      printf("\t$f%i = $s%i->addFill($g, SWFFILL_RADIAL_GRADIENT);\n", num, id);
      break;

    case 0x40:
      printf("\t$f%i = $s%i->addFill($b%i, SWFFILL_TILED_BITMAP);\n", num, id, s->fill.bitmap);
      break;

    case 0x41:
      printf("\t$f%i = $s%i->addFill($b%i, SWFFILL_CLIPPED_BITMAP);\n", num, id, s->fill.bitmap);
      break;

    default:
      error("Unknown fill type: 0x%02x!", s->type);
  }

  if(isMorph)
  {
    if(s->type == 0x40 || s->type == 0x41)
      printTransform(&(s->matrix2), 'f', num);
  }
  else
    printTransform(&(s->matrix), 'f', num);
}

void readFillStyleArray(FILE *f, struct Shape *shape, int isMorph)
{
  struct FillStyles *s;
  int count, i, start;

  start = shape->fills.nFills;

  shape->fillOffset = start;

  count = readUInt8(f);

  if(count==255)
    count = readUInt16(f);

  s = &(shape->fills);

  s->nFills += count;
  s->fill = realloc(s->fill, s->nFills * sizeof(struct FillStyle));

  for(i=0; i<count; ++i)
    readFillStyle(f, &(s->fill[start+i]), shape->shapeType, isMorph);
}

int readShapeRec(FILE *f, struct Shape *shape)
{
  struct ShapeRecord *s;
  int type;

  shape->shapes.shape = realloc(shape->shapes.shape,
				(shape->shapes.nShapes+1) *
				sizeof(struct ShapeRecord));

  s = &(shape->shapes.shape[shape->shapes.nShapes]);

  ++shape->shapes.nShapes;

  type = readBits(f, 1);

  if(type==0) /* state change */
  {
    int newStyles = readBits(f, 1);
    int lineStyle = readBits(f, 1);
    int fillStyle1 = readBits(f, 1);
    int fillStyle0 = readBits(f, 1);
    int moveTo = readBits(f, 1);

    if(newStyles==0 && lineStyle==0 && fillStyle1==0 &&
       fillStyle0==0 && moveTo==0)
    {
      s->type = SHAPERECORD_END;
      return 0;
    }

    s->type = SHAPERECORD_STATECHANGE;

    if(moveTo==1)
    {
      int moveBits = readBits(f, 5);
      s->data.change.movetox = readSBits(f, moveBits);
      s->data.change.movetoy = readSBits(f, moveBits);
    }
    else
    {
      s->data.change.movetox = 0;
      s->data.change.movetoy = 0;
    }

    if(fillStyle0==1)
    {
      int num = readBits(f, shape->fillBits);
      s->data.change.fill0 = (num==0) ? 0 : num + shape->fillOffset;
    }
    else
      s->data.change.fill0 = -1;

    if(fillStyle1==1)
    {
      int num = readBits(f, shape->fillBits);
      s->data.change.fill1 = (num==0) ? 0 : num + shape->fillOffset;
    }
    else
      s->data.change.fill1 = -1;

    if(lineStyle==1)
    {
      int num = readBits(f, shape->lineBits);
      s->data.change.line = (num==0) ? 0 : num + shape->lineOffset;
    }
    else
      s->data.change.line = -1;

    if(newStyles==1)
    {
      readFillStyleArray(f, shape, 0);
      readLineStyleArray(f, shape, 0);
      shape->fillBits = readBits(f, 4);
      shape->lineBits = readBits(f, 4);
    }
  }
  else /* it's an edge record */
  {
    int straight = readBits(f, 1);
    int numBits = readBits(f, 4)+2;

    if(straight==1)
    {
      s->type = SHAPERECORD_LINE;

      if(readBits(f, 1)) /* general line */
      {
	s->data.line.x = readSBits(f, numBits);
	s->data.line.y = readSBits(f, numBits);
      }
      else
	if(readBits(f, 1)) /* vert = 1 */
	{
	  s->data.line.x = 0;
	  s->data.line.y = readSBits(f, numBits);
	}
	else
	{
	  s->data.line.x = readSBits(f, numBits);
	  s->data.line.y = 0;
	}
    }
    else
    {
      s->type = SHAPERECORD_CURVE;

      s->data.curve.controlx = readSBits(f, numBits);
      s->data.curve.controly = readSBits(f, numBits);
      s->data.curve.anchorx = readSBits(f, numBits);
      s->data.curve.anchory = readSBits(f, numBits);
    }
  }

  return 1;
}

void printFillChange(int id, struct Shape *shape, int fillNum, int side)
{
  if(fillNum == 0)
    /*no fill*/
    printf("\t$s%i->set%sFill();\n", id, side==0 ? "Left" : "Right");

  else if(shape->fills.fill[fillNum-1].type == 0)
  {
    printf("\t$s%i->set%sFill(", id, side==0 ? "Left" : "Right");
    printRGBA(&(shape->fills.fill[fillNum-1].fill.color));
    printf(");\n");
  }

  else {
    printf("\t$s%i->set%sFill($f%i);\n", id, side==0 ? "Left" : "Right",
	   fillNum);
  }
}

void printShapeRec(struct Shape *shape, struct ShapeRecord *s, int id){
	switch(s->type){
		
		case SHAPERECORD_END:
			return;
		
		case SHAPERECORD_STATECHANGE:
			if(s->data.change.movetox != 0 || s->data.change.movetoy != 0){
				printf("\t$s%i->movePenTo(%i, %i);\n", id, \
					s->data.change.movetox, \
					s->data.change.movetoy);
			}
      if(s->data.change.fill0 != -1){
				printFillChange(id, shape, s->data.change.fill0, 0);
			}
      if(s->data.change.fill1 != -1){
				printFillChange(id, shape, s->data.change.fill1, 1);
			}
      if(s->data.change.line != -1){
				if(s->data.change.line == 0){
					printf("\t$s%i->setLine(0);\n", id);
				}
				else{
					struct LineStyle *line = &(shape->lines.line[s->data.change.line-1]);
					printf("\t$s%i->setLine(%i, ", id, line->width);
					printRGBA(&(line->color));
					printf(");\n");
				}
			}
			break;

		case SHAPERECORD_LINE:
			printf("\t$s%i->drawLine(%i, %i);\n", id, s->data.line.x, s->data.line.y);
			break;

		case SHAPERECORD_CURVE:
			printf("\t$s%i->drawCurve(%i, %i, %i, %i);\n", id,
				s->data.curve.controlx, s->data.curve.controly,
				s->data.curve.anchorx, s->data.curve.anchory);
			break;

		default:
			error("Bad shape type: %i!\n", s->type);
	}
}

void printDefineShape(struct Shape *shape, int id, int isMorph)
{
  int i;

  for(i=0; i<shape->fills.nFills; ++i)
    printFillStyle(&(shape->fills.fill[i]), id, i+1, isMorph);

  for(i=0; i<shape->shapes.nShapes; ++i)
    printShapeRec(shape, &(shape->shapes.shape[i]), id);
}

void printShape(FILE *f, int length, int shapeType)
{
  struct Shape shape;
  int id, start = fileOffset;

  memset(&shape, 0, sizeof(struct Shape));

  id = readUInt16(f);

  shape.shapeType = shapeType;

  readRect(f, &(shape.bounds)); /* bounds */

  readFillStyleArray(f, &shape, 0);
  readLineStyleArray(f, &shape, 0);

  byteAlign();

  shape.fillBits = readBits(f,4);
  shape.lineBits = readBits(f,4);

  while(fileOffset < length+start &&
	readShapeRec(f, &shape)) ;

  /* go for end tag..
  if(fileOffset == length+start && bufbits > 5)
    printShapeRec(f, &lineBits, &fillBits, 2);
  */

  printf("\n\t" COMMSTART " Shape %i " COMMEND "\n", id);
  printf("\t$s%i = new %sShape();\n", id, modNamePrefix());

  printDefineShape(&shape, id, 0);
}

void printMorphShape(FILE *f, int length)
{
  int offset, start = fileOffset, here;
  struct Rect r;
  struct Shape shape1, shape2;
  int id = readUInt16(f);

  printf("\n\t" COMMSTART " Morph %i " COMMEND "\n", id);
  printf("\t$s%i = new %sMorph();\n", id, modNamePrefix());

  readRect(f, &r); /* bounds 1 */
  readRect(f, &r); /* bounds 2 */

  offset = readUInt32(f);

  here = fileOffset+1;

  memset(&shape1, 0, sizeof(struct Shape));
  memset(&shape2, 0, sizeof(struct Shape));

  shape1.shapeType = shape2.shapeType = DEFINESHAPE3;

  readFillStyleArray(f, &shape1, 1);
  readLineStyleArray(f, &shape1, 1);

  byteAlign();

  shape1.fillBits = shape2.fillBits = readBits(f,4);
  shape1.lineBits = shape2.lineBits = readBits(f,4);

  while(fileOffset < here+offset &&
	readShapeRec(f, &shape1)) ;

  printf("\n\t$s%i_1 = $s%i->getShape1();\n", id, id);

  printDefineShape(&shape1, id, 0);

  byteAlign();

  readUInt8(f); /* redundant fill/line bits */

  while(fileOffset < start+length &&
	readShapeRec(f, &shape2)) ;

  printf("\n\t$s%i_2 = $s%i->getShape2();\n", id, id);

  printDefineShape(&shape1, id, 1);
}

/* JPEG stream markers: */
#define JPEG_MARKER 0xFF

/* Start of Image, End of Image */
#define JPEG_SOI  0xD8
#define JPEG_EOI  0xD9

#define JPEG_JFIF 0xE0

/* encoding markers, quantization tables and Huffman tables */
#define JPEG_QUANT 0xDB
#define JPEG_HUFF  0xC4

/* image markers, start of frame and start of scan */
#define JPEG_SOF0 0xC0
#define JPEG_SOF1 0xC1
#define JPEG_SOF2 0xC2
#define JPEG_SOS  0xDA
void printJpegStream(FILE *f, int length)
{
  int end = fileOffset+length;
  int c, l;

  while(fileOffset < end)
  {
    if(readUInt8(f) != JPEG_MARKER)
    {
      printf("%sJpeg marker not found!\n", indent());
      break;
    }

    switch(c=readUInt8(f))
    {
      case JPEG_SOI:   printf("%sSOI\n", indent()); break;
      case JPEG_EOI:   printf("%sEOI\n", indent()); break;
      case JPEG_JFIF:  printf("%sJFIF\n", indent()); break;
      case JPEG_QUANT: printf("%sQuantization table\n", indent()); break;
      case JPEG_HUFF:  printf("%sHuffman table\n", indent()); break;
      case JPEG_SOF0:  printf("%sStart of frame 0\n", indent()); break;
      case JPEG_SOF1:  printf("%sStart of frame 1\n", indent()); break;
      case JPEG_SOF2:  printf("%sStart of frame 2\n", indent()); break;
      case JPEG_SOS:   printf("%sStart of scan\n", indent()); break;
      default:         printf("%sUnknown JPEG block: %02x\n", indent(), c);
    }

    if(c==JPEG_SOS)
      break;

    if(c != JPEG_SOI && c != JPEG_EOI)
    {
      l = (readUInt8(f)<<8) + readUInt8(f);
      skipBytes(f, l-2);
    }
  }

  skipBytes(f, end-fileOffset);
}

void printDefineBitsJpeg(FILE *f, int length)
{
  printf("%sBitmap id: %i\n", indent(), readUInt16(f));
  printJpegStream(f, length-2);
}

void mkBitmap(FILE *f, int length, int id, int format, int width, int height, int csize)
{
  char buf[1024];
  FILE *b;
  int s;

  /* WARNING! length seems to be the uncompressed length, not the compressed length */
  sprintf(buf,"bitmap%d.gz",id);
  b=fopen(buf,"w");
  while(length>0) {
	  s=fread(buf,1,1024,f);
	  fwrite(buf,1,s,b);
	  length-=s;
  }
  fclose(b);
}

void printDefineBitsLossless(FILE *f, int length)
{
  int id, format, width, height, colorTableSize;

  id = readUInt16(f);length-=2;
  printf("\t$b%i = new %sBitmap(fopen(\"bitmap%d\",\"r\") );\n",id, modNamePrefix(),id);
  format = readUInt8(f);length-=1;   /*	 format = 3, 4, 5 -> 2^format bpp */
  width = readUInt16(f);length-=2;
  height = readUInt16(f);length-=2;
  if(format == 3)
    colorTableSize = readUInt8(f)-1;length-=1;
  mkBitmap(f, length, id, format, width, height, colorTableSize);
}

/*
int printActionRecord(FILE *f)
{
  int length = 0, type = readUInt8(f);

  if((type&0x80) == 0x80)
    length = readUInt16(f);

  switch(type)
  {
    case ACTION_ADD:
      printf("%sAdd\n", indent());
      break;
    case ACTION_SUBTRACT:
      printf("%sSubtract\n", indent());
      break;
    case ACTION_MULTIPLY:
      printf("%sMultiply\n", indent());
      break;
    case ACTION_DIVIDE:
      printf("%sDivide\n", indent());
      break;
    case ACTION_EQUAL:
      printf("%sEquals\n", indent());
      break;
    case ACTION_LESSTHAN:
      printf("%sLess Than\n", indent());
      break;
    case ACTION_LOGICALAND:
      printf("%sAnd\n", indent());
      break;
    case ACTION_LOGICALOR:
      printf("%sOr\n", indent());
      break;
    case ACTION_LOGICALNOT:
      printf("%sNot\n", indent());
      break;
    case ACTION_STRINGEQ:
      printf("%sString eq\n", indent());
      break;
    case ACTION_STRINGLENGTH:
      printf("%sString Length\n", indent());
      break;
    case ACTION_SUBSTRING:
      printf("%sSubstring\n", indent());
      break;
    case ACTION_INT:
      printf("%sInt\n", indent());
      break;
    case ACTION_GETVARIABLE:
      printf("%sGet Variable\n", indent());
      break;
    case ACTION_SETVARIABLE:
      printf("%sSet Variable\n", indent());
      break;
    case ACTION_SETTARGETEXPRESSION:
      printf("%sSet Target Expression\n", indent());
      break;
    case ACTION_STRINGCONCAT:
      printf("%sString Concat\n", indent());
      break;
    case ACTION_GETPROPERTY:
      printf("%sGet Property\n", indent());
      break;
    case ACTION_SETPROPERTY:
      printf("%sSet Property\n", indent());
      break;
    case ACTION_DUPLICATECLIP:
      printf("%sDuplicate Clip\n", indent());
      break;
    case ACTION_REMOVECLIP:
      printf("%sRemove Clip\n", indent());
      break;
    case ACTION_TRACE:
      printf("%sTrace\n", indent());
      break;
    case ACTION_STARTDRAGMOVIE:
      printf("%sStart Drag Movie\n", indent());
      break;
    case ACTION_STOPDRAGMOVIE:
      printf("%sStop Drag Movie\n", indent());
      break;
    case ACTION_STRINGCOMPARE:
      printf("%sString Compare\n", indent());
      break;
    case ACTION_RANDOM:
      printf("%sRandom\n", indent());
      break;
    case ACTION_MBLENGTH:
      printf("%sString MB Length\n", indent());
      break;
    case ACTION_ORD:
      printf("%sOrd\n", indent());
      break;
    case ACTION_CHR:
      printf("%sChr\n", indent());
      break;
    case ACTION_GETTIMER:
      printf("%sGet Timer\n", indent());
      break;
    case ACTION_MBSUBSTRING:
      printf("%sMB Substring\n", indent());
      break;
    case ACTION_MBORD:
      printf("%sMB Ord\n", indent());
      break;
    case ACTION_MBCHR:
      printf("%sMB Chr\n", indent());
      break;
    case ACTION_NEXTFRAME:
      printf("%sNext Frame\n", indent());
      break;
    case ACTION_PREVFRAME:
      printf("%sPrevious Frame\n", indent());
      break;
    case ACTION_PLAY:
      printf("%sPlay\n", indent());
      break;
    case ACTION_STOP:
      printf("%sStop\n", indent());
      break;
    case ACTION_TOGGLEQUALITY:
      printf("%sToggle Quality\n", indent());
      break;
    case ACTION_STOPSOUNDS:
      printf("%sStop Sounds\n", indent());
      break;

*/    /* ops with args */ /*
    case ACTION_PUSHDATA:
    {
      int type = readUInt8(f);
      if(type==0)
        printf("%sPush String: %s\n", indent(), readString(f));
      else
      {
	readUInt16(f);
        printf("%sPush Property: %04x\n", indent(), readUInt16(f));
      }
      break;
    }
    case ACTION_GOTOFRAME:
      printf("%sGoto Frame %i\n", indent(), readUInt16(f));
      break;
    case ACTION_GETURL:
    {
      char *url = readString(f);
      printf("%sGet URL \"%s\" target \"%s\"\n", indent(), url, readString(f));
      break;
    }
    case ACTION_WAITFORFRAMEEXPRESSION:
      printf("%sWait For Frame Expression, skip %i\n", indent(), readUInt8(f));
      break;
    case ACTION_BRANCHALWAYS:
      printf("%sBranch Always %i\n", indent(), readSInt16(f));
      break;
    case ACTION_GETURL2:
      switch(readUInt8(f))
      {
        case 0: printf("%sGet URL2 (Don't send)\n", indent()); break;
        case 1: printf("%sGet URL2 (GET)\n", indent()); break;
        case 2: printf("%sGet URL2 (POST)\n", indent()); break;
      }
      break;
    case ACTION_BRANCHIFTRUE:
      printf("%sBranch If True %i\n", indent(), readSInt16(f));
      break;
    case ACTION_CALLFRAME:
      printf("%sCall Frame\n", indent());
      dumpBytes(f, length);
      break;
    case ACTION_GOTOEXPRESSION:
      printf("%sAdd\n", indent());
      dumpBytes(f, length);
      break;
    case ACTION_WAITFORFRAME:
    {
      int frame = readUInt16(f);
      printf("%sWait for frame %i else skip %i\n", indent(), frame, readUInt8(f));
      break;
    }
    case ACTION_SETTARGET:
      printf("%sSet Target %s\n", indent(), readString(f));
      break;
    case ACTION_GOTOLABEL:
      printf("%sGoto Label %s\n", indent(), readString(f));
      break;
    case ACTION_END:
      return 0;
      break;
    default:
      printf("%sUnknown Action: %02X\n", indent(), type);
      dumpBytes(f, length);
  }
  return 1;
}
*/

int printButtonRecord(FILE *f, int recordType, int id)
{
  int character, layer;
  int flags = readUInt8(f);
  struct Matrix m;
  int notFirst = 0;

  if(flags == 0)
    return 0;

  character = readUInt16(f);
  layer = readUInt16(f);

  printf("\t$s%i->addShape($s%i, ", id, character);
  readMatrix(f, &m);

  if(flags & 0x08)
  {
    printf("SWFBUTTON_HIT");
    notFirst = 1;
  }
  if(flags & 0x04)
  {
    printf("%sSWFBUTTON_DOWN", (notFirst?" | ":""));
    notFirst = 1;
  }
  if(flags & 0x02)
  {
    printf("%sSWFBUTTON_OVER", (notFirst?" | ":""));
    notFirst = 1;
  }
  if(flags & 0x01)
  {
    printf("%sSWFBUTTON_UP", (notFirst?" | ":""));
    notFirst = 1;
  }
  printf(");\n");

  printf("\t# Button character transforms not implemented, sorry.\n");

  if(recordType == 2)
  {
    struct CXForm s;
    readCXForm(f, &s, true);
    printf("\t# Button cxforms not implemented, either.. \n");
  }

  return 1;
}


void printDefineButtonSound(FILE *f, int length){
	int i;
	int id = readUInt16(f);

	for(i=0;i<4;i++){
		int soundid = readUInt16(f);
		if (soundid != 0){
			printf("\t$si%i_%i = $s%i->addSound($snd%i,",id,i,id,soundid);
			switch(i){
				case 0:	 printf("SWFBUTTON_UP);\n"); break;
				case 1:	 printf("SWFBUTTON_OVER);\n"); break;
				case 2:	 printf("SWFBUTTON_DOWN);\n"); break;
				default: printf("SWFBUTTON_HIT);\n");
			}	
			printSoundInstance(f,id,soundid);		
		}
	}
}

#define SWFSOUNDINFO_SYNCSTOPSOUND  (1<<5)
#define SWFSOUNDINFO_SYNCNOMULTIPLE (1<<4)
#define SWFSOUNDINFO_HASENVELOPE    (1<<3)
#define SWFSOUNDINFO_HASLOOPS       (1<<2)
#define SWFSOUNDINFO_HASOUTPOINT    (1<<1)
#define SWFSOUNDINFO_HASINPOINT     (1<<0)

void printSoundInstance(FILE *f, int id, int soundid){
  int e,point,left,right;
	int flags = readUInt8(f);

	if (flags & SWFSOUNDINFO_SYNCSTOPSOUND){
		printf("\t# stopsound??");
	}
	if (flags & SWFSOUNDINFO_SYNCNOMULTIPLE){
		printf("\t$si->setNoMultiple();\n");
	}
	if(flags & SWFSOUNDINFO_HASINPOINT){
		int inpoint = readUInt32(f);
		printf("\t$si->setInPoint(%i);\n", inpoint);
	}
	if(flags & SWFSOUNDINFO_HASOUTPOINT){
		int outpoint = readUInt32(f);
		printf("\t$si->setOutPoint(%i);\n", outpoint);               
	}
	if(flags & SWFSOUNDINFO_HASLOOPS){
		int loopcount = readUInt16(f);
		printf("\t$si->setLoops(%i);\n", loopcount);
	}
	if(flags & SWFSOUNDINFO_HASENVELOPE) {
		int envpoints = readUInt8(f);
		for (e=0; e<envpoints;e++){
			point = readUInt32(f);
			left  = readUInt16(f);
			left  = readUInt16(f);
			printf("\t$si->addEnvelopePoint(%i,%i,%i);\n", point,left,right);
		}
	}
} 

void printDefineButton(FILE *f, int length)
{
  int offset = fileOffset;
  int id = readUInt16(f);

  printf("\n\t" COMMSTART " Button %i " COMMEND "\n", id);
  printf("\t$s%i = new %sButton();\n", id, modNamePrefix());

  while(printButtonRecord(f, 1, id)) ;

  printf("\t$a = new %sAction(\"\n", modNamePrefix());
  decompileAction(f, length-(fileOffset-offset), 0);
  printf("\");\n");
  printf("\t$s%i->setAction($a);\n", id);
}

int printButton2ActionCondition(FILE *f, int end, int id)
{
  int offset = readUInt16(f);
  int condition = readUInt16(f);
  int notFirst = 0;

  printf("\t$a = new %sAction(\"\n", modNamePrefix());

  if(offset == 0)
    {decompileAction(f, end-fileOffset, 0);}
  else
    {decompileAction(f, offset-4, 0);}

  printf("\");\n");
  printf("\t$s%i->addAction($a, ", id);

  if(condition & 0xfe00)
  {
    printf("SWFBUTTON_KEYPRESS(%c)", (condition&0xfe00)>>9);
    notFirst = 1;
  }
  if(condition & 0x100 || condition & 0x10)
  {
    printf("%sSWFBUTTON_DRAGOUT", (notFirst?" | ":""));
    notFirst = 1;
  }
  if(condition & 0x80 || condition & 0x20)
  {
    printf("%sSWFBUTTON_DRAGIN", (notFirst?" | ":""));
    notFirst = 1;
  }
  if(condition & 0x40)
  {
    printf("%sSWFBUTTON_MOUSEUPOUTSIDE", (notFirst?" | ":""));
    notFirst = 1;
  }
  if(condition & 0x08)
  {
    printf("%sSWFBUTTON_MOUSEUP", (notFirst?" | ":""));
    notFirst = 1;
  }
  if(condition & 0x04)
  {
    printf("%sSWFBUTTON_MOUSEDOWN", (notFirst?" | ":""));
    notFirst = 1;
  }
  if(condition & 0x02)
  {
    printf("%sSWFBUTTON_MOUSEOUT", (notFirst?" | ":""));
    notFirst = 1;
  }
  if(condition & 0x01)
  {
    printf("%sSWFBUTTON_MOUSEOVER", (notFirst?" | ":""));
    notFirst = 1;
  }

  printf(");\n");

  return offset;
}

void printDefineButton2(FILE *f, int length)
{
  int offset, end = fileOffset+length;
  int id = readUInt16(f);
  int flags = readUInt8(f);

  printf("\n\t" COMMSTART " Button2 %i " COMMEND "\n", id);
  printf("\t$s%i = new %sButton();\n", id, modNamePrefix() );

  if(flags){printf("\t$s%i->setMenu();\n",id);}

  offset = readUInt16(f);

  while(printButtonRecord(f, 2, id)) ;

  if(offset>0)
    while(printButton2ActionCondition(f, end, id)) ;
}

void printCXForm(struct CXForm *c, char ch, int num)
{
  if(c->aMult != 0x100)
    printf("\t$%c%i->multColor(%f, %f, %f, %f);\n", ch, num,
	   c->rMult/256.0, c->gMult/256.0, c->bMult/256.0, c->aMult/256.0);
  else
    printf("\t$%c%i->multColor(%f, %f, %f);\n", ch, num,
	   c->rMult/256.0, c->gMult/256.0, c->bMult/256.0);

  if(c->aAdd != 0)
    printf("\t$%c%i->addColor(0x%02x, 0x%02x, 0x%02x, 0x%02x);\n", ch, num,
	   c->rAdd, c->gAdd, c->bAdd, c->aAdd);
  else
    printf("\t$%c%i->addColor(0x%02x, 0x%02x, 0x%02x);\n", ch, num,
	   c->rAdd, c->gAdd, c->bAdd);
}

void printPlaceObject(FILE *f, int length)
{
  int start = fileOffset;
  struct Matrix m;
  int id = readUInt16(f);
  int depth = readUInt16(f);

  readMatrix(f, &m);

  if(movieclip == 0)
    printf("\t$i%i = $m->add($s%i);\n", depth, id);
  else
    printf("\t$j%i = $s%i->add($s%i);\n", depth, movieclip, id);

  printTransform(&m, (movieclip==0)?'i':'j', depth);

  if(fileOffset < start+length)
  {
    struct CXForm c;
    readCXForm(f, &c, false);
    printCXForm(&c, (movieclip==0)?'i':'j', depth);
  }
}

#define PLACE_HAS_ACTION    (1<<7)
#define PLACE_HAS_MASK      (1<<6)
#define PLACE_HAS_NAME      (1<<5)
#define PLACE_HAS_RATIO     (1<<4)
#define PLACE_HAS_CXFORM    (1<<3)
#define PLACE_HAS_MATRIX    (1<<2)
#define PLACE_HAS_CHARACTER (1<<1)
#define PLACE_HAS_MOVE      (1<<0)

void printPlaceObject2(FILE *f){
	int flags = readUInt8(f);
	int depth = readUInt16(f);
	int l;

	if(flags & PLACE_HAS_CHARACTER){
		if(movieclip == 0){
			printf("\t$i%i = $m->add($s%i);\n", depth, readUInt16(f));
		}
		else {
			printf("\t$j%i = $s%i->add($s%i);\n", depth, movieclip, readUInt16(f));
		}
	}

	if(flags & PLACE_HAS_MATRIX){
		struct Matrix m;
		readMatrix(f, &m);
		printTransform(&m, (movieclip==0)?'i':'j', depth);
	}

	if(flags & PLACE_HAS_CXFORM){
		struct CXForm c;
		readCXForm(f, &c, true);
		printCXForm(&c, (movieclip==0)?'i':'j', depth);
	}

	if(flags & PLACE_HAS_RATIO){
		printf("\t$%c%i->setRatio(%f);\n",(movieclip==0)?'i':'j', depth, (float)readUInt16(f)/(1<<16));
	}

	if(flags & PLACE_HAS_NAME){
		printf("\t$%c%i->setName('%s');\n",(movieclip==0)?'i':'j', depth, readString(f));
	}

	if(flags & PLACE_HAS_MASK){
		//printf("\t# clipDepth (%i) not implemented \n", readUInt16(f));
		/*added by Peter*/
		printf("\t$%c%i->setMaskLevel(%i);\n", (movieclip==0)?'i':'j', depth, readUInt16(f));
	}

	if(flags & PLACE_HAS_ACTION){
		printf(COMMSTART "Mystery number: %04x" COMMEND "\n", readUInt16(f));

		flags = readUInt16(f);
		//printf("#Clip flags: %04x\n", flags);
		printf("\t$%c%i->addAction(new %sAction(\"\n", (movieclip==0)?'i':'j', depth, modNamePrefix() );

#define PLACEACTION_ONLOAD      (1<<0)
#define PLACEACTION_ENTERFRAME  (1<<1)
#define PLACEACTION_UNLOAD      (1<<2)
#define PLACEACTION_MOUSEMOVE   (1<<3)
#define PLACEACTION_MOUSEDOWN   (1<<4)
#define PLACEACTION_MOUSEUP     (1<<5)
#define PLACEACTION_KEYDOWN     (1<<6)
#define PLACEACTION_KEYUP       (1<<7)
#define PLACEACTION_DATA        (1<<8)

		while((flags = readUInt16(f)) != 0){
			//printf("//flags: %04x\n", flags);

			switch(flags){
				case PLACEACTION_ONLOAD: printf("onClipEvent(load){\n");  break;
				case PLACEACTION_ENTERFRAME:printf("onClipEvent(enterFrame){\n"); break;
				case PLACEACTION_UNLOAD: printf("onClipEvent(unload){\n"); break;
				case PLACEACTION_MOUSEMOVE: printf("onClipEvent(mouseMove){\n"); break;
				case PLACEACTION_MOUSEDOWN: printf("onClipEvent(mouseDown){\n"); break;
				case PLACEACTION_MOUSEUP: printf("onClipEvent(mouseUp){\n"); break;
				case PLACEACTION_KEYDOWN: printf("onClipEvent(keyDown){\n"); break;
				case PLACEACTION_KEYUP: printf("onClipEvent(keyUp){\n"); break;
				case PLACEACTION_DATA: printf("onClipEvent(data){\n"); break;
				default: printf("// unknown clipEvent: %04x\n", flags);
			}
			l = readUInt32(f);
			decompileAction(f, l, 0);
			printf("}\n");
		}
		printf("\"));\n");
	}
}


void printRemoveObject(FILE *f)
{
  /* may be cases where id matters, not sure. */

  readUInt16(f); /* id */

  if(movieclip == 0)
    printf("\t$m->remove($i%i);\n", readUInt16(f));
  else
    printf("\t$s%i->remove($j%i);\n", movieclip, readUInt16(f));
}

void printRemoveObject2(FILE *f)
{
  if(movieclip == 0)
    printf("\t$m->remove($i%i);\n", readUInt16(f));
  else
    printf("\t$s%i->remove($j%i);\n", movieclip, readUInt16(f));
}

void printSetBackgroundColor(FILE *f)
{
  struct RGBA s;
  readRGB(f, &s);

  printf("\t$m->setBackground(0x%02x, 0x%02x, 0x%02x);\n", s.r, s.g, s.b);
}

void printFrameLabel(FILE *f)
{
  if(movieclip == 0)
    printf("\t$m->labelFrame('%s');\n", readString(f));
  else
    printf("\t$s%i->labelFrame('%s');\n", movieclip, readString(f));
}

#define FONTINFO2_HASLAYOUT		(1<<7)
#define FONTINFO2_SHIFTJIS		(1<<6)
#define FONTINFO2_UNICODE		(1<<5)
#define FONTINFO2_ANSI			(1<<4)
#define FONTINFO2_WIDEOFFSETS	(1<<3)
#define FONTINFO2_WIDECODES		(1<<2)
#define FONTINFO2_ITALIC		(1<<1)
#define FONTINFO2_BOLD			(1<<0)

void printDefineFont(FILE *f, int length) { 
	printf("\t# DefineFont, %i bytes \n", length);
	silentSkipBytes(f, length);
}

void printDefineFont2(FILE *f, int length)
{
  int flags, namelen, i, reserved;
  char name[264];
  FILE *out;
  int wide = 0;

  int id = readUInt16(f); /* font id */

  flags = readUInt8(f);

  if(flags & FONTINFO2_WIDECODES) {
#if 1
    wide = 1;
    warning("Sorry, wide code fonts not supported.");
#else
    error("Sorry, wide code fonts not supported.");
#endif
  }

  if(!(flags & FONTINFO2_HASLAYOUT)){ 
	//error("Hrm.  I was expecting some layout info.");  
	fprintf(stderr,"Hrm. I was expecting some layout info!\n"); 
  }
  
  reserved = readUInt8(f); /* "reserved" */

  namelen = readUInt8(f);
  for(i=0; i<namelen; ++i){ 
  	name[i] = (unsigned char)readUInt8(f);
  	if (name[i] == ' ') {name[i] = '_';}
  }

  if(flags & FONTINFO2_BOLD)
  {
    name[i++] = '-';
    name[i++] = 'B';
  }
  if(flags & FONTINFO2_ITALIC)
  {
    name[i++] = '-';
    name[i++] = 'I';    
  }

  name[i++] = '.';
  name[i++] = 'f';
  name[i++] = 'd';
  name[i++] = 'b';
  name[i] = '\0';
  printf("\t$f%i = new %sFont(\"%s\" );  # This file should exist.  \n", id, modNamePrefix() , name);

  /* writing fontdatabase */
  fprintf(stderr,"Writing %s, %i bytes\n", name, length+2);
  if((out=fopen(name,"wb"))==NULL) {error("Couldn't open output file!"); }

  fputc('f', out);
  fputc('d', out);
  fputc('b', out);
  fputc('0', out);
  fputc(flags, out);
  fputc(reserved, out);
  fputc(namelen, out);

  for(i=0; i<namelen; ++i) {fputc(name[i], out); }
  length -= namelen + 5;
  /*for(; length>0; --length) { fputc(fgetc(f), out);  }*/
  for(; length>0; --length) { fputc(readUInt8(f), out); }
  fclose(out);

  if(wide)
  {
    printf("Removing %s\n", name);
    unlink(name);
  }
}

#define FONTINFO_RESERVED	(1<<6 | 1<<7)
#define FONTINFO_UNICODE	(1<<5)
#define FONTINFO_SHIFTJIS	(1<<4)
#define FONTINFO_ANSI		(1<<3)
#define FONTINFO_ITALIC		(1<<2)
#define FONTINFO_BOLD		(1<<1)
#define FONTINFO_WIDE		(1<<0)

#define TEXTRECORD_STATECHANGE	(1<<7)
#define TEXTRECORD_RESERVED		(1<<6 | 1<<5 | 1<<4)
#define TEXTRECORD_HASFONT		(1<<3)
#define TEXTRECORD_HASCOLOR		(1<<2)
#define TEXTRECORD_HASYOFF		(1<<1)
#define TEXTRECORD_HASXOFF		(1<<0)
#define TEXTRECORD_NUMGLYPHS	0x7f

int printTextRecord(FILE *f, int glyphBits, int advanceBits, int type)
{
  int i, numGlyphs, fontid;
  int flags = readUInt8(f);
  struct RGBA rgba;

  if(flags == 0) {return 0; }
  if(flags & TEXTRECORD_STATECHANGE)
  {
    if(flags & TEXTRECORD_HASFONT) {fontid = readUInt16(f); /* fontid */ }

    if(flags & TEXTRECORD_HASCOLOR)
    {
      if(type == DEFINETEXT2) {readRGBA(f, &rgba);}
      else {readRGB(f, &rgba);}
    }

    if(flags & TEXTRECORD_HASXOFF) {readSInt16(f); /* x offset */ }
    if(flags & TEXTRECORD_HASYOFF) {readSInt16(f); /* y offset */ }
    if(flags & TEXTRECORD_HASFONT) {readUInt16(f); /* font height */ }
  }
  else /* it's a text entry */
  {
    numGlyphs = flags & TEXTRECORD_NUMGLYPHS;
    for(i=0; i<numGlyphs; ++i)
    {
      readBits(f, glyphBits); /* glyph index */
      readSBits(f, advanceBits); /* glyph advance */
    }
  }

  return 1;
}

void printDefineText(FILE *f, Blocktype type) /* type 2 allows transparency */
{
  int glyphBits, advanceBits;
  int id = readUInt16(f);
  struct Rect r;
  struct Matrix m;

  printf("\t$s%i = new %sShape();  # Empty shape for text, for now \n", id, modNamePrefix() );

  readRect(f, &r);
  byteAlign();
  readMatrix(f, &m);

  glyphBits = readUInt8(f);
  advanceBits = readUInt8(f);

  while(printTextRecord(f, glyphBits, advanceBits, type)) ;
}

void silentSkipBytes(FILE *f, int length)
{
  for(; length>0; --length)
    readUInt8(f);
}

void printDoAction(FILE *f, int length)
{
  if(movieclip == 0)
  {
    printf("\t$m->add(new %sAction(\"\n", modNamePrefix() );
    decompileAction(f, length, 0);
    printf("\"));\n");
  }
  else
  {
    printf("\t$s%i->add(new %sAction(\"\n", movieclip, modNamePrefix() );
    decompileAction(f, length, 0);
    printf("\"));\n");
  }
}

void printMovieClip(FILE *f, int length)
{
  int start = fileOffset;
  int block, type, l;
  int nFrames, frame = 0;

  movieclip = readUInt16(f);
  nFrames = readUInt16(f);

  // Sprite heisst jetzt MovieClip! names changed
  printf("\n\t" COMMSTART " MovieClip %i " COMMEND "\n", movieclip);
  printf("\t$s%i = new %sMovieClip();  # %i frames\n", movieclip, modNamePrefix(), nFrames);

  while(fileOffset < start+length)
  {
    block = readUInt16(f);
    type = block>>6;

    l = block & ((1<<6)-1);

    if(l == 63) /* it's a long block */
      l = readUInt32(f);

    switch(type)
    {
      case PLACEOBJECT:    printPlaceObject(f, l); break;
      case PLACEOBJECT2:   printPlaceObject2(f); break;
      case REMOVEOBJECT:   printRemoveObject(f); break;
      case REMOVEOBJECT2:  printRemoveObject2(f); break;
      case FRAMELABEL:     printFrameLabel(f); break;
      case DOACTION:       printDoAction(f, l); break;
      case SHOWFRAME:      printf("\t$s%i->nextFrame();  # end of clip frame %i \n\n", movieclip, ++frame); break;
      case END:            putchar('\n'); break;
      /*
      case SOUNDSTREAMHEAD:  printSoundStreamHead(f, 1);     break;
      case SOUNDSTREAMHEAD2: printSoundStreamHead(f, 2);     break;
      case SOUNDSTREAMBLOCK: printSoundStreamBlock(f, l);    break;
      */
      default:
        printf("\t# %s, %i bytes \n", blockName(type), l);
        silentSkipBytes(f, l);
    }
  }

  movieclip = 0;
}
/* peter: Constants with numbers added by peter for testing */
#define SWFTEXTFIELD_HASLENGTH (1<<1)
#define SWFTEXTFIELD_2         (1<<2)
#define SWFTEXTFIELD_NOEDIT    (1<<3)
#define SWFTEXTFIELD_PASSWORD  (1<<4)
#define SWFTEXTFIELD_MULTILINE (1<<5)
#define SWFTEXTFIELD_WORDWRAP  (1<<6)
#define SWFTEXTFIELD_7         (1<<7)
#define SWFTEXTFIELD_8         (1<<8)
#define SWFTEXTFIELD_9         (1<<9)
#define SWFTEXTFIELD_10        (1<<10)
#define SWFTEXTFIELD_DRAWBOX   (1<<11)
#define SWFTEXTFIELD_NOSELECT  (1<<12)

#define SWFTEXTFIELD_ALIGN_LEFT    0
#define SWFTEXTFIELD_ALIGN_RIGHT   1
#define SWFTEXTFIELD_ALIGN_CENTER  2
#define SWFTEXTFIELD_ALIGN_JUSTIFY 3

void printTextField(FILE *f, int length)
{
  int notFirst = 0, flags, num, end = fileOffset+length;
  struct Rect r;
  struct RGBA c;
  char *string;
  int id = readUInt16(f);

  readRect(f, &r);

  flags=readUInt16(f);

  printf("\n\t$s%i = new %sTextField(", id, modNamePrefix() );

  if(flags & SWFTEXTFIELD_HASLENGTH)
  {
    printf("SWFTEXTFIELD_HASLENGTH");
    notFirst = 1;
  }
  if(flags & SWFTEXTFIELD_NOEDIT)
  { 
    if (notFirst) { printf(" | ");}
    printf("SWFTEXTFIELD_NOEDIT");
    notFirst = 1;
  }
  if(flags & SWFTEXTFIELD_PASSWORD)
  {
    if (notFirst) { printf(" | ");}
    printf("SWFTEXTFIELD_PASSWORD");
    notFirst = 1;
  }
  if(flags & SWFTEXTFIELD_MULTILINE)
  {
    if (notFirst) { printf(" | ");}
    printf("SWFTEXTFIELD_MULTILINE");
    notFirst = 1;
  }
  if(flags & SWFTEXTFIELD_WORDWRAP)
  {
    if (notFirst) { printf(" | ");}
    printf("SWFTEXTFIELD_WORDWRAP");
    notFirst = 1;
  }
  if(flags & SWFTEXTFIELD_DRAWBOX)
  {
    if (notFirst) { printf(" | ");}
    printf("SWFTEXTFIELD_DRAWBOX");
    notFirst = 1;
  }
  if(flags & SWFTEXTFIELD_NOSELECT)
  {
    if (notFirst) { printf(" | ");}
    printf("SWFTEXTFIELD_NOSELECT");
    notFirst = 1;
  }
/* peter test */
/*  
  if(flags & SWFTEXTFIELD_2)
  {
    if (notFirst) { printf(" | ");}
    printf("SWFTEXTFIELD_2");
    notFirst = 1;
  }
  if(flags & SWFTEXTFIELD_7)
  {
    if (notFirst) { printf(" | ");}
    printf("SWFTEXTFIELD_7");
    notFirst = 1;
  }  
  if(flags & SWFTEXTFIELD_8)
  {
    if (notFirst) { printf(" | ");}
    printf("SWFTEXTFIELD_8");
    notFirst = 1;
  }
  if(flags & SWFTEXTFIELD_9)
  {
    if (notFirst) { printf(" | ");}
    printf("SWFTEXTFIELD_9");
    notFirst = 1;
  }  
  if(flags & SWFTEXTFIELD_10)
  {
    if (notFirst) { printf(" | ");}
    printf("SWFTEXTFIELD_10");
    notFirst = 1;
  }
*/
/*end peter test */  

  printf(");\n");

  printf("\t$s%i->setBounds(%i, %i);\n", id, r.xMax, r.yMax);

  printf("\t$s%i->setFont($f%i);\n", id, readUInt16(f));
  printf("\t$s%i->setHeight(%i);\n", id, readUInt16(f));

  readRGBA(f, &c);
  printf("\t$s%i->setColor(0x%02x, 0x%02x, 0x%02x, 0x%02x);\n", id, c.r, c.g, c.b, c.a);

  if(flags & SWFTEXTFIELD_HASLENGTH)
    printf("\t$s%i->setLength(%i);\n", id, readUInt16(f));

  printf("\t$s%i->align(", id);

  switch(num = readUInt8(f))
  {
    case SWFTEXTFIELD_ALIGN_LEFT:    printf("SWFTEXTFIELD_ALIGN_LEFT");    break;
    case SWFTEXTFIELD_ALIGN_RIGHT:   printf("SWFTEXTFIELD_ALIGN_RIGHT");   break;
    case SWFTEXTFIELD_ALIGN_CENTER:  printf("SWFTEXTFIELD_ALIGN_CENTER");  break;
    case SWFTEXTFIELD_ALIGN_JUSTIFY: printf("SWFTEXTFIELD_ALIGN_JUSTIFY"); break;
    default: error("unexpected justification: %i\n", num);
  }

  printf(");\n");

  if((num = readUInt16(f)) != 0)
    {printf("\t$s%i->setLeftMargin(%i);\n", id, num);}

  if((num = readUInt16(f)) != 0)
    {printf("\t$s%i->setRightMargin(%i);\n", id, num);}

  if((num = readUInt16(f)) != 0)
    {printf("\t$s%i->setIndentation(%i);\n", id, num);}

  if((num = readUInt16(f)) != 40)
    {printf("\t$s%i->setLineSpacing(%i);\n", id, num);}

  string = readString(f);

  if(string[0] != '\0')
    {printf("\t$s%i->setName('%s');\n", id, string);}

  if(fileOffset<end)
  {
    string = readString(f);

    if(string[0] != '\0')
      {printf("\t$s%i->addString('%s');\n", id, string);}
  }

  if(fileOffset<end)
  {
    printf("# extra garbage (i.e., we goofed in printTextField):\n");
    puts("print STDERR <<'DUMP;'");
    dumpBytes(f, end-fileOffset);
    puts("DUMP\n");
  }
}

void skipBytes(FILE *f, int length)
{
  ++gIndent;
  printf("%s<%i bytes skipped>\n\n", indent(), length);
  --gIndent;

  for(; length>0; --length)
    readUInt8(f);
}

/*
 * Compressed swf-files have a 8 Byte uncompressed header and a
 * zlib-compressed body. 
 */
int
cws2fws(FILE *f, uLong outsize)
{

	struct stat statbuffer;
	int insize;
	int err,tmp_fd;
	Byte *inbuffer,*outbuffer;

	sprintf(tmp_name, "swftoscriptXXXXXX");
	
	tmp_fd = mkstemp(tmp_name);
	if ( tmp_fd == -1 )
	{
		error("Couldn't create tempfile.\n");
	}

	tempfile = fdopen(tmp_fd, "w");
	if ( ! tempfile )
	{
		error("fdopen: %s", strerror(errno));
	}


	if( stat(filename, &statbuffer) == -1 )
	{
		error("stat() failed on input file");
	}
	
	insize = statbuffer.st_size-8;
	inbuffer = malloc(insize);
	if(!inbuffer){ error("malloc() failed"); }
	fread(inbuffer,insize,1,f);
	
	/* We don't trust the value in the swfheader. */
	outbuffer=NULL;
	do{
		outbuffer = realloc(outbuffer, outsize);	
		if (!outbuffer) { error("malloc(%lu) failed",outsize); }
		
		err=uncompress(outbuffer,&outsize,inbuffer,insize);
		switch(err){
			case Z_MEM_ERROR:
				error("Not enough memory.\n");
				break;
			case Z_BUF_ERROR:
				fprintf(stderr,"resizing outbuffer..\n");
				outsize*=2;
				continue;
			case Z_DATA_ERROR:
				error("Data corrupted. Couldn't uncompress.\n");
				break;
			case Z_OK:
				break;
			default:
				error("Unknown returnvalue of uncompress:%i\n",
					err);
				break;
		}
	} while(err == Z_BUF_ERROR);
 
	fwrite(outbuffer, 1, outsize, tempfile);
	rewind(tempfile);
	return (int)outsize;
}


int main(int argc, char *argv[])
{	
	struct Movie m;
	FILE *f;
	char first;
  int block, type, length, frame = 0, noactions = 0;
	int compressed= 0;
	
	if(argc == 3 && strcmp(argv[1], "-a") == 0)
	{	noactions = 1;
		--argc;
		++argv;
	}
	filename= argv[1];
	
	if(argc<2) {
		error("Give me a filename.\n\n\tswftoperl myflash..swf >myflash.pl");
	}

	if(!(f = fopen(filename,"rb"))){
		error("Sorry, can't seem to read that file.\n");
	}
	
	first=readUInt8(f);
	compressed=(first==('C'))?1:0;
	if(!((first=='C' || first=='F') && readUInt8(f)=='W' && readUInt8(f)=='S')){
		error("Doesn't look like a swf file to me..\n");
	}
	m.version = readUInt8(f);
	m.size = readUInt32(f);
	
	if(compressed){
		int unzipped=cws2fws(f, m.size);
		if (m.size!=(unzipped+8)){
			warning("m.size: %i != %i+8  Maybe wrong value in swfheader.\n",m.size, unzipped+8);
		}
		fclose(f);
		f=tempfile;
		rewind(f);
	}	

	readRect(f, &(m.frame));

	m.rate = readUInt8(f)/256.0+readUInt8(f);
	m.nFrames = readUInt16(f);

#ifdef SWFPHP
	printf("<?php\n");
#endif
#ifdef SWFPERL
	printf("#!/usr/bin/perl -w\n");
	printf("# Generated by swftoperl converter included with ming. Have fun. \n\n");
	printf("# Change this to your needs. If you installed perl-ming global you don't need this.\n");
	printf("#use lib(\"/home/peter/mystuff/lib/site_perl\");\n\n");

	printf("# We import all because our converter is not so clever to select only needed. ;-)\n");
	printf("use SWF qw(:ALL);\n"); 
	printf("# Just copy from a sample, needed to use Constants like SWFFILL_RADIAL_GRADIENT\n");
	printf("use SWF::Constants qw(:Text :Button :DisplayItem :Fill);\n\n");
	printf("SWF::setScale(1);\n");
	printf("SWF::setVersion(%i);\n", m.version);
#endif
	printf("\t$m = new %sMovie();\n\n", modNamePrefix() );
	printf("\t$m->setRate(%f);\n", m.rate);
	printf("\t$m->setDimension(%i, %i);\n", m.frame.xMax, m.frame.yMax);
	printf("\t$m->setFrames(%i);\n", m.nFrames);

  if(noactions){ m_version = 0; }
	else { m_version = m.version; }
	
  for(;;)
  {
    block = readUInt16(f);
    type = block>>6;

    length = block & ((1<<6)-1);

    if(length == 63) /* it's a long block. */
      length = readUInt32(f);

    if(type == 0 || fileOffset >= m.size)
      break;

    switch(type)
    {
      case DEFINESHAPE3:
      case DEFINESHAPE2:
      case DEFINESHAPE:       printShape(f, length, type); break;
      case SETBACKGROUNDCOLOR: printSetBackgroundColor(f); break;
      case SHOWFRAME:         printf("\t$m->nextFrame();  # end of frame %i\n", ++frame); break;
      case PLACEOBJECT2:      printPlaceObject2(f); break;
      case REMOVEOBJECT2:     printRemoveObject2(f); break;
      case DOACTION:          printDoAction(f, length); break;
      case DEFINESPRITE:      printMovieClip(f, length); break;
      case DEFINEBUTTON:      printDefineButton(f, length); break;
      case DEFINEBUTTON2:     printDefineButton2(f, length); break;
      case DEFINEBUTTONSOUND: printDefineButtonSound(f, length); break;
      case FRAMELABEL:        printFrameLabel(f); break;
      case DEFINETEXT:
      case DEFINETEXT2:       printDefineText(f, type); break;
      case TEXTFIELD:         printTextField(f, length); break;
      case DEFINEMORPHSHAPE:  printMorphShape(f, length); break;
      case PLACEOBJECT:       printPlaceObject(f, length); break;
      case REMOVEOBJECT:      printRemoveObject(f); break;

      case DEFINEFONT:        printDefineFont(f, length); break;
      case DEFINEFONT2:       printDefineFont2(f, length); break;
/*
      case DEFINEFONTINFO:    printFontInfo(f, length); break;
      case DEFINESOUND:       printDefineSound(f, length); break;
      case SOUNDSTREAMHEAD:   printSoundStreamHead(f, 1); break;
      case SOUNDSTREAMHEAD2:  printSoundStreamHead(f, 2); break;
      case SOUNDSTREAMBLOCK:  printSoundStreamBlock(f, length); break;
 */
      case JPEGTABLES:        printJpegStream(f, length); break;
      case DEFINEBITS:
      case DEFINEBITSJPEG2:  printDefineBitsJpeg(f,length);  break;
      case DEFINELOSSLESS:   printDefineBitsLossless(f,length);  break;

			/*
			case DEFINEBITSJPEG3:
			*/
      default:
	printf("\t" COMMSTART "Top level parser: %s, %i bytes " COMMEND "\n", blockName(type), length);
	silentSkipBytes(f, length);
    }
  }

  putchar('\n');

  if(fileOffset < m.size)
  {
    printf(COMMSTART " extra garbage (i.e., we messed up in main): \n");
    dumpBytes(f, m.size-fileOffset);
    printf("\n" COMMEND "\n");
  }

#ifdef SWFPHP
  printf("\n\theader('Content-type: application/x-shockwave-flash');\n");
  printf("\t$m->output();\n");
#endif
#ifdef SWFPERL
  printf("\t#print('Content-type: application/x-shockwave-flash\\n\\n');\n");
  printf("\t$m->output(%i);\n",(compressed)?'9':'0');
  printf("\t$m->save(\".swf\",%i);\n",(compressed)?'9':'0');
#endif
#ifdef SWFPHP
  printf("?>");
#endif
	
	fclose(f);
	if (compressed){
		unlink(tmp_name);
	}
  exit(0);
}
