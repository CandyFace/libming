# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _mingc

def _swig_setattr(self,class_type,name,value):
    if (name == "this"):
        if isinstance(value, class_type):
            self.__dict__[name] = value.this
            if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
            del value.thisown
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    self.__dict__[name] = value

def _swig_getattr(self,class_type,name):
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types



Ming_init = _mingc.Ming_init

Ming_cleanup = _mingc.Ming_cleanup

Ming_collectGarbage = _mingc.Ming_collectGarbage

Ming_setCubicThreshold = _mingc.Ming_setCubicThreshold

Ming_setScale = _mingc.Ming_setScale

Ming_getScale = _mingc.Ming_getScale

Ming_useSWFVersion = _mingc.Ming_useSWFVersion

Ming_setSWFCompression = _mingc.Ming_setSWFCompression

fileOutputMethod = _mingc.fileOutputMethod

newSWFInput_file = _mingc.newSWFInput_file

newSWFInput_stream = _mingc.newSWFInput_stream

newSWFInput_buffer = _mingc.newSWFInput_buffer

newSWFInput_allocedBuffer = _mingc.newSWFInput_allocedBuffer

destroySWFInput = _mingc.destroySWFInput

SWFInput_length = _mingc.SWFInput_length

SWFInput_rewind = _mingc.SWFInput_rewind

SWFInput_tell = _mingc.SWFInput_tell

SWFInput_seek = _mingc.SWFInput_seek

SWFInput_eof = _mingc.SWFInput_eof

SWFCharacter_getWidth = _mingc.SWFCharacter_getWidth

SWFCharacter_getHeight = _mingc.SWFCharacter_getHeight

newSWFBitmap_fromInput = _mingc.newSWFBitmap_fromInput

destroySWFBitmap = _mingc.destroySWFBitmap

SWFBitmap_getWidth = _mingc.SWFBitmap_getWidth

SWFBitmap_getHeight = _mingc.SWFBitmap_getHeight

newSWFDBLBitmap = _mingc.newSWFDBLBitmap

newSWFDBLBitmap_fromInput = _mingc.newSWFDBLBitmap_fromInput

newSWFJpegBitmap = _mingc.newSWFJpegBitmap

newSWFJpegBitmap_fromInput = _mingc.newSWFJpegBitmap_fromInput

newSWFJpegWithAlpha = _mingc.newSWFJpegWithAlpha

newSWFJpegWithAlpha_fromInput = _mingc.newSWFJpegWithAlpha_fromInput

newSWFGradient = _mingc.newSWFGradient

destroySWFGradient = _mingc.destroySWFGradient

SWFGradient_addEntry = _mingc.SWFGradient_addEntry
SWFFILL_SOLID = _mingc.SWFFILL_SOLID
SWFFILL_GRADIENT = _mingc.SWFFILL_GRADIENT
SWFFILL_LINEAR_GRADIENT = _mingc.SWFFILL_LINEAR_GRADIENT
SWFFILL_RADIAL_GRADIENT = _mingc.SWFFILL_RADIAL_GRADIENT
SWFFILL_BITMAP = _mingc.SWFFILL_BITMAP
SWFFILL_TILED_BITMAP = _mingc.SWFFILL_TILED_BITMAP
SWFFILL_CLIPPED_BITMAP = _mingc.SWFFILL_CLIPPED_BITMAP

newSWFSolidFillStyle = _mingc.newSWFSolidFillStyle

newSWFGradientFillStyle = _mingc.newSWFGradientFillStyle

newSWFBitmapFillStyle = _mingc.newSWFBitmapFillStyle

SWFFillStyle_getMatrix = _mingc.SWFFillStyle_getMatrix

newSWFLineStyle = _mingc.newSWFLineStyle

newSWFShape = _mingc.newSWFShape

newSWFShapeFromBitmap = _mingc.newSWFShapeFromBitmap

destroySWFShape = _mingc.destroySWFShape

SWFShape_end = _mingc.SWFShape_end

SWFShape_movePenTo = _mingc.SWFShape_movePenTo

SWFShape_movePen = _mingc.SWFShape_movePen

SWFShape_getPenX = _mingc.SWFShape_getPenX

SWFShape_getPenY = _mingc.SWFShape_getPenY

SWFShape_getPen = _mingc.SWFShape_getPen

SWFShape_drawLineTo = _mingc.SWFShape_drawLineTo

SWFShape_drawLine = _mingc.SWFShape_drawLine

SWFShape_drawCurveTo = _mingc.SWFShape_drawCurveTo

SWFShape_drawCurve = _mingc.SWFShape_drawCurve

SWFShape_setLineStyle = _mingc.SWFShape_setLineStyle

SWFShape_hideLine = _mingc.SWFShape_hideLine

SWFShape_addSolidFillStyle = _mingc.SWFShape_addSolidFillStyle

SWFShape_addGradientFillStyle = _mingc.SWFShape_addGradientFillStyle

SWFShape_addBitmapFillStyle = _mingc.SWFShape_addBitmapFillStyle

SWFShape_setLeftFillStyle = _mingc.SWFShape_setLeftFillStyle

SWFShape_setRightFillStyle = _mingc.SWFShape_setRightFillStyle

newSWFMorphShape = _mingc.newSWFMorphShape

destroySWFMorph = _mingc.destroySWFMorph

SWFMorph_getShape1 = _mingc.SWFMorph_getShape1

SWFMorph_getShape2 = _mingc.SWFMorph_getShape2

newSWFFont = _mingc.newSWFFont

loadSWFFontFromFile = _mingc.loadSWFFontFromFile

destroySWFFont = _mingc.destroySWFFont

SWFFont_getStringWidth = _mingc.SWFFont_getStringWidth

SWFFont_getUTF8StringWidth = _mingc.SWFFont_getUTF8StringWidth

SWFFont_getWideStringWidth = _mingc.SWFFont_getWideStringWidth

SWFFont_getAscent = _mingc.SWFFont_getAscent

SWFFont_getDescent = _mingc.SWFFont_getDescent

SWFFont_getLeading = _mingc.SWFFont_getLeading

SWFFont_getShape = _mingc.SWFFont_getShape

newSWFText = _mingc.newSWFText

newSWFText2 = _mingc.newSWFText2

destroySWFText = _mingc.destroySWFText

SWFText_setFont = _mingc.SWFText_setFont

SWFText_setHeight = _mingc.SWFText_setHeight

SWFText_setColor = _mingc.SWFText_setColor

SWFText_moveTo = _mingc.SWFText_moveTo

SWFText_addString = _mingc.SWFText_addString

SWFText_addUTF8String = _mingc.SWFText_addUTF8String

SWFText_addWideString = _mingc.SWFText_addWideString

SWFText_setSpacing = _mingc.SWFText_setSpacing

SWFText_getStringWidth = _mingc.SWFText_getStringWidth

SWFText_getUTF8StringWidth = _mingc.SWFText_getUTF8StringWidth

SWFText_getWideStringWidth = _mingc.SWFText_getWideStringWidth

SWFText_getAscent = _mingc.SWFText_getAscent

SWFText_getDescent = _mingc.SWFText_getDescent

SWFText_getLeading = _mingc.SWFText_getLeading

newSWFBrowserFont = _mingc.newSWFBrowserFont

destroySWFBrowserFont = _mingc.destroySWFBrowserFont

SWFMovie_addFont = _mingc.SWFMovie_addFont

SWFFontCharacter_addChars = _mingc.SWFFontCharacter_addChars

SWFFontCharacter_addUTF8Chars = _mingc.SWFFontCharacter_addUTF8Chars

SWFMovie_importFont = _mingc.SWFMovie_importFont
SWFTEXTFIELD_ONMASK = _mingc.SWFTEXTFIELD_ONMASK
SWFTEXTFIELD_OFFMASK = _mingc.SWFTEXTFIELD_OFFMASK
SWFTEXTFIELD_HASFONT = _mingc.SWFTEXTFIELD_HASFONT
SWFTEXTFIELD_HASLENGTH = _mingc.SWFTEXTFIELD_HASLENGTH
SWFTEXTFIELD_HASCOLOR = _mingc.SWFTEXTFIELD_HASCOLOR
SWFTEXTFIELD_NOEDIT = _mingc.SWFTEXTFIELD_NOEDIT
SWFTEXTFIELD_PASSWORD = _mingc.SWFTEXTFIELD_PASSWORD
SWFTEXTFIELD_MULTILINE = _mingc.SWFTEXTFIELD_MULTILINE
SWFTEXTFIELD_WORDWRAP = _mingc.SWFTEXTFIELD_WORDWRAP
SWFTEXTFIELD_HASTEXT = _mingc.SWFTEXTFIELD_HASTEXT
SWFTEXTFIELD_USEFONT = _mingc.SWFTEXTFIELD_USEFONT
SWFTEXTFIELD_HTML = _mingc.SWFTEXTFIELD_HTML
SWFTEXTFIELD_DRAWBOX = _mingc.SWFTEXTFIELD_DRAWBOX
SWFTEXTFIELD_NOSELECT = _mingc.SWFTEXTFIELD_NOSELECT
SWFTEXTFIELD_HASLAYOUT = _mingc.SWFTEXTFIELD_HASLAYOUT
SWFTEXTFIELD_AUTOSIZE = _mingc.SWFTEXTFIELD_AUTOSIZE
SWFTEXTFIELD_ALIGN_LEFT = _mingc.SWFTEXTFIELD_ALIGN_LEFT
SWFTEXTFIELD_ALIGN_RIGHT = _mingc.SWFTEXTFIELD_ALIGN_RIGHT
SWFTEXTFIELD_ALIGN_CENTER = _mingc.SWFTEXTFIELD_ALIGN_CENTER
SWFTEXTFIELD_ALIGN_JUSTIFY = _mingc.SWFTEXTFIELD_ALIGN_JUSTIFY

newSWFTextField = _mingc.newSWFTextField

destroySWFTextField = _mingc.destroySWFTextField

SWFTextField_setFont = _mingc.SWFTextField_setFont

SWFTextField_setBounds = _mingc.SWFTextField_setBounds

SWFTextField_setFlags = _mingc.SWFTextField_setFlags

SWFTextField_setColor = _mingc.SWFTextField_setColor

SWFTextField_setVariableName = _mingc.SWFTextField_setVariableName

SWFTextField_addString = _mingc.SWFTextField_addString

SWFTextField_addUTF8String = _mingc.SWFTextField_addUTF8String

SWFTextField_setHeight = _mingc.SWFTextField_setHeight

SWFTextField_setFieldHeight = _mingc.SWFTextField_setFieldHeight

SWFTextField_setLeftMargin = _mingc.SWFTextField_setLeftMargin

SWFTextField_setRightMargin = _mingc.SWFTextField_setRightMargin

SWFTextField_setIndentation = _mingc.SWFTextField_setIndentation

SWFTextField_setLineSpacing = _mingc.SWFTextField_setLineSpacing

SWFTextField_setPadding = _mingc.SWFTextField_setPadding

SWFTextField_addChars = _mingc.SWFTextField_addChars

SWFTextField_setAlignment = _mingc.SWFTextField_setAlignment

SWFTextField_setLength = _mingc.SWFTextField_setLength

newSWFSoundStream = _mingc.newSWFSoundStream

newSWFSoundStreamFromFileno = _mingc.newSWFSoundStreamFromFileno

newSWFSoundStream_fromInput = _mingc.newSWFSoundStream_fromInput

SWFSoundStream_getFrames = _mingc.SWFSoundStream_getFrames

destroySWFSoundStream = _mingc.destroySWFSoundStream

newSWFSound = _mingc.newSWFSound

newSWFSoundFromFileno = _mingc.newSWFSoundFromFileno

newSWFSound_fromInput = _mingc.newSWFSound_fromInput

destroySWFSound = _mingc.destroySWFSound
SWF_SOUND_COMPRESSION = _mingc.SWF_SOUND_COMPRESSION
SWF_SOUND_NOT_COMPRESSED = _mingc.SWF_SOUND_NOT_COMPRESSED
SWF_SOUND_ADPCM_COMPRESSED = _mingc.SWF_SOUND_ADPCM_COMPRESSED
SWF_SOUND_MP3_COMPRESSED = _mingc.SWF_SOUND_MP3_COMPRESSED
SWF_SOUND_NOT_COMPRESSED_LE = _mingc.SWF_SOUND_NOT_COMPRESSED_LE
SWF_SOUND_NELLY_COMPRESSED = _mingc.SWF_SOUND_NELLY_COMPRESSED
SWF_SOUND_RATE = _mingc.SWF_SOUND_RATE
SWF_SOUND_5KHZ = _mingc.SWF_SOUND_5KHZ
SWF_SOUND_11KHZ = _mingc.SWF_SOUND_11KHZ
SWF_SOUND_22KHZ = _mingc.SWF_SOUND_22KHZ
SWF_SOUND_44KHZ = _mingc.SWF_SOUND_44KHZ
SWF_SOUND_BITS = _mingc.SWF_SOUND_BITS
SWF_SOUND_8BITS = _mingc.SWF_SOUND_8BITS
SWF_SOUND_16BITS = _mingc.SWF_SOUND_16BITS
SWF_SOUND_CHANNELS = _mingc.SWF_SOUND_CHANNELS
SWF_SOUND_MONO = _mingc.SWF_SOUND_MONO
SWF_SOUND_STEREO = _mingc.SWF_SOUND_STEREO

SWFSoundInstance_setLoopInPoint = _mingc.SWFSoundInstance_setLoopInPoint

SWFSoundInstance_setLoopOutPoint = _mingc.SWFSoundInstance_setLoopOutPoint

SWFSoundInstance_setLoopCount = _mingc.SWFSoundInstance_setLoopCount

SWFSoundInstance_setNoMultiple = _mingc.SWFSoundInstance_setNoMultiple

newSWFCXform = _mingc.newSWFCXform

newSWFAddCXform = _mingc.newSWFAddCXform

newSWFMultCXform = _mingc.newSWFMultCXform

destroySWFCXform = _mingc.destroySWFCXform

SWFCXform_setColorAdd = _mingc.SWFCXform_setColorAdd

SWFCXform_setColorMult = _mingc.SWFCXform_setColorMult

compileSWFActionCode = _mingc.compileSWFActionCode

destroySWFAction = _mingc.destroySWFAction
SWFBUTTON_HIT = _mingc.SWFBUTTON_HIT
SWFBUTTON_DOWN = _mingc.SWFBUTTON_DOWN
SWFBUTTON_OVER = _mingc.SWFBUTTON_OVER
SWFBUTTON_UP = _mingc.SWFBUTTON_UP
SWFBUTTONRECORD_HITSTATE = _mingc.SWFBUTTONRECORD_HITSTATE
SWFBUTTONRECORD_DOWNSTATE = _mingc.SWFBUTTONRECORD_DOWNSTATE
SWFBUTTONRECORD_OVERSTATE = _mingc.SWFBUTTONRECORD_OVERSTATE
SWFBUTTONRECORD_UPSTATE = _mingc.SWFBUTTONRECORD_UPSTATE
SWFBUTTON_OVERDOWNTOIDLE = _mingc.SWFBUTTON_OVERDOWNTOIDLE
SWFBUTTON_IDLETOOVERDOWN = _mingc.SWFBUTTON_IDLETOOVERDOWN
SWFBUTTON_OUTDOWNTOIDLE = _mingc.SWFBUTTON_OUTDOWNTOIDLE
SWFBUTTON_OUTDOWNTOOVERDOWN = _mingc.SWFBUTTON_OUTDOWNTOOVERDOWN
SWFBUTTON_OVERDOWNTOOUTDOWN = _mingc.SWFBUTTON_OVERDOWNTOOUTDOWN
SWFBUTTON_OVERDOWNTOOVERUP = _mingc.SWFBUTTON_OVERDOWNTOOVERUP
SWFBUTTON_OVERUPTOOVERDOWN = _mingc.SWFBUTTON_OVERUPTOOVERDOWN
SWFBUTTON_OVERUPTOIDLE = _mingc.SWFBUTTON_OVERUPTOIDLE
SWFBUTTON_IDLETOOVERUP = _mingc.SWFBUTTON_IDLETOOVERUP
SWFBUTTON_MOUSEUPOUTSIDE = _mingc.SWFBUTTON_MOUSEUPOUTSIDE
SWFBUTTON_DRAGOVER = _mingc.SWFBUTTON_DRAGOVER
SWFBUTTON_DRAGOUT = _mingc.SWFBUTTON_DRAGOUT
SWFBUTTON_MOUSEUP = _mingc.SWFBUTTON_MOUSEUP
SWFBUTTON_MOUSEDOWN = _mingc.SWFBUTTON_MOUSEDOWN
SWFBUTTON_MOUSEOUT = _mingc.SWFBUTTON_MOUSEOUT
SWFBUTTON_MOUSEOVER = _mingc.SWFBUTTON_MOUSEOVER

newSWFButton = _mingc.newSWFButton

destroySWFButton = _mingc.destroySWFButton

SWFButton_addShape = _mingc.SWFButton_addShape

SWFButton_addAction = _mingc.SWFButton_addAction

SWFButton_addSound = _mingc.SWFButton_addSound

SWFButton_setMenu = _mingc.SWFButton_setMenu

newSWFVideoStream_fromFile = _mingc.newSWFVideoStream_fromFile

newSWFVideoStream_fromInput = _mingc.newSWFVideoStream_fromInput

newSWFVideoStream = _mingc.newSWFVideoStream

SWFVideoStream_setDimension = _mingc.SWFVideoStream_setDimension

SWFVideoStream_getNumFrames = _mingc.SWFVideoStream_getNumFrames

newSWFSprite = _mingc.newSWFSprite

destroySWFSprite = _mingc.destroySWFSprite

SWFSprite_addBlock = _mingc.SWFSprite_addBlock
class _swfPosition(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, _swfPosition, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, _swfPosition, name)
    def __repr__(self):
        return "<C _swfPosition instance at %s>" % (self.this,)
    __swig_setmethods__["x"] = _mingc._swfPosition_x_set
    __swig_getmethods__["x"] = _mingc._swfPosition_x_get
    if _newclass:x = property(_mingc._swfPosition_x_get, _mingc._swfPosition_x_set)
    __swig_setmethods__["y"] = _mingc._swfPosition_y_set
    __swig_getmethods__["y"] = _mingc._swfPosition_y_get
    if _newclass:y = property(_mingc._swfPosition_y_get, _mingc._swfPosition_y_set)
    __swig_setmethods__["xScale"] = _mingc._swfPosition_xScale_set
    __swig_getmethods__["xScale"] = _mingc._swfPosition_xScale_get
    if _newclass:xScale = property(_mingc._swfPosition_xScale_get, _mingc._swfPosition_xScale_set)
    __swig_setmethods__["yScale"] = _mingc._swfPosition_yScale_set
    __swig_getmethods__["yScale"] = _mingc._swfPosition_yScale_get
    if _newclass:yScale = property(_mingc._swfPosition_yScale_get, _mingc._swfPosition_yScale_set)
    __swig_setmethods__["xSkew"] = _mingc._swfPosition_xSkew_set
    __swig_getmethods__["xSkew"] = _mingc._swfPosition_xSkew_get
    if _newclass:xSkew = property(_mingc._swfPosition_xSkew_get, _mingc._swfPosition_xSkew_set)
    __swig_setmethods__["ySkew"] = _mingc._swfPosition_ySkew_set
    __swig_getmethods__["ySkew"] = _mingc._swfPosition_ySkew_get
    if _newclass:ySkew = property(_mingc._swfPosition_ySkew_get, _mingc._swfPosition_ySkew_set)
    __swig_setmethods__["rot"] = _mingc._swfPosition_rot_set
    __swig_getmethods__["rot"] = _mingc._swfPosition_rot_get
    if _newclass:rot = property(_mingc._swfPosition_rot_get, _mingc._swfPosition_rot_set)
    __swig_setmethods__["matrix"] = _mingc._swfPosition_matrix_set
    __swig_getmethods__["matrix"] = _mingc._swfPosition_matrix_get
    if _newclass:matrix = property(_mingc._swfPosition_matrix_get, _mingc._swfPosition_matrix_set)
    def __init__(self, *args):
        _swig_setattr(self, _swfPosition, 'this', _mingc.new__swfPosition(*args))
        _swig_setattr(self, _swfPosition, 'thisown', 1)
    def __del__(self, destroy=_mingc.delete__swfPosition):
        try:
            if self.thisown: destroy(self)
        except: pass

class _swfPositionPtr(_swfPosition):
    def __init__(self, this):
        _swig_setattr(self, _swfPosition, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, _swfPosition, 'thisown', 0)
        _swig_setattr(self, _swfPosition,self.__class__,_swfPosition)
_mingc._swfPosition_swigregister(_swfPositionPtr)


newSWFPosition = _mingc.newSWFPosition

destroySWFPosition = _mingc.destroySWFPosition

SWFPosition_skewX = _mingc.SWFPosition_skewX

SWFPosition_skewXTo = _mingc.SWFPosition_skewXTo

SWFPosition_skewY = _mingc.SWFPosition_skewY

SWFPosition_skewYTo = _mingc.SWFPosition_skewYTo

SWFPosition_scaleX = _mingc.SWFPosition_scaleX

SWFPosition_scaleXTo = _mingc.SWFPosition_scaleXTo

SWFPosition_scaleY = _mingc.SWFPosition_scaleY

SWFPosition_scaleYTo = _mingc.SWFPosition_scaleYTo

SWFPosition_scaleXY = _mingc.SWFPosition_scaleXY

SWFPosition_scaleXYTo = _mingc.SWFPosition_scaleXYTo

SWFPosition_setMatrix = _mingc.SWFPosition_setMatrix

SWFPosition_rotate = _mingc.SWFPosition_rotate

SWFPosition_rotateTo = _mingc.SWFPosition_rotateTo

SWFPosition_move = _mingc.SWFPosition_move

SWFPosition_moveTo = _mingc.SWFPosition_moveTo

SWFDisplayItem_move = _mingc.SWFDisplayItem_move

SWFDisplayItem_moveTo = _mingc.SWFDisplayItem_moveTo

SWFDisplayItem_rotate = _mingc.SWFDisplayItem_rotate

SWFDisplayItem_rotateTo = _mingc.SWFDisplayItem_rotateTo

SWFDisplayItem_scale = _mingc.SWFDisplayItem_scale

SWFDisplayItem_scaleTo = _mingc.SWFDisplayItem_scaleTo

SWFDisplayItem_skewX = _mingc.SWFDisplayItem_skewX

SWFDisplayItem_skewXTo = _mingc.SWFDisplayItem_skewXTo

SWFDisplayItem_skewY = _mingc.SWFDisplayItem_skewY

SWFDisplayItem_skewYTo = _mingc.SWFDisplayItem_skewYTo

SWFDisplayItem_getPosition = _mingc.SWFDisplayItem_getPosition

SWFDisplayItem_getRotation = _mingc.SWFDisplayItem_getRotation

SWFDisplayItem_getScale = _mingc.SWFDisplayItem_getScale

SWFDisplayItem_getSkew = _mingc.SWFDisplayItem_getSkew

SWFDisplayItem_setMatrix = _mingc.SWFDisplayItem_setMatrix

SWFDisplayItem_getDepth = _mingc.SWFDisplayItem_getDepth

SWFDisplayItem_setDepth = _mingc.SWFDisplayItem_setDepth

SWFDisplayItem_remove = _mingc.SWFDisplayItem_remove

SWFDisplayItem_setName = _mingc.SWFDisplayItem_setName

SWFDisplayItem_setMaskLevel = _mingc.SWFDisplayItem_setMaskLevel

SWFDisplayItem_setRatio = _mingc.SWFDisplayItem_setRatio

SWFDisplayItem_setCXform = _mingc.SWFDisplayItem_setCXform

SWFDisplayItem_setColorAdd = _mingc.SWFDisplayItem_setColorAdd

SWFDisplayItem_setColorMult = _mingc.SWFDisplayItem_setColorMult
SWFACTION_ONLOAD = _mingc.SWFACTION_ONLOAD
SWFACTION_ENTERFRAME = _mingc.SWFACTION_ENTERFRAME
SWFACTION_UNLOAD = _mingc.SWFACTION_UNLOAD
SWFACTION_MOUSEMOVE = _mingc.SWFACTION_MOUSEMOVE
SWFACTION_MOUSEDOWN = _mingc.SWFACTION_MOUSEDOWN
SWFACTION_MOUSEUP = _mingc.SWFACTION_MOUSEUP
SWFACTION_KEYDOWN = _mingc.SWFACTION_KEYDOWN
SWFACTION_KEYUP = _mingc.SWFACTION_KEYUP
SWFACTION_DATA = _mingc.SWFACTION_DATA

SWFDisplayItem_addAction = _mingc.SWFDisplayItem_addAction

newSWFFill = _mingc.newSWFFill

destroySWFFill = _mingc.destroySWFFill

SWFFill_skewX = _mingc.SWFFill_skewX

SWFFill_skewXTo = _mingc.SWFFill_skewXTo

SWFFill_skewY = _mingc.SWFFill_skewY

SWFFill_skewYTo = _mingc.SWFFill_skewYTo

SWFFill_scaleX = _mingc.SWFFill_scaleX

SWFFill_scaleXTo = _mingc.SWFFill_scaleXTo

SWFFill_scaleY = _mingc.SWFFill_scaleY

SWFFill_scaleYTo = _mingc.SWFFill_scaleYTo

SWFFill_scaleXY = _mingc.SWFFill_scaleXY

SWFFill_scaleXYTo = _mingc.SWFFill_scaleXYTo

SWFFill_rotate = _mingc.SWFFill_rotate

SWFFill_rotateTo = _mingc.SWFFill_rotateTo

SWFFill_move = _mingc.SWFFill_move

SWFFill_moveTo = _mingc.SWFFill_moveTo

SWFFill_setMatrix = _mingc.SWFFill_setMatrix

SWFShape_setLine = _mingc.SWFShape_setLine

SWFShape_addSolidFill = _mingc.SWFShape_addSolidFill

SWFShape_addGradientFill = _mingc.SWFShape_addGradientFill

SWFShape_addBitmapFill = _mingc.SWFShape_addBitmapFill

SWFShape_setLeftFill = _mingc.SWFShape_setLeftFill

SWFShape_setRightFill = _mingc.SWFShape_setRightFill

SWFShape_drawArc = _mingc.SWFShape_drawArc

SWFShape_drawCircle = _mingc.SWFShape_drawCircle

SWFShape_drawGlyph = _mingc.SWFShape_drawGlyph

SWFShape_drawSizedGlyph = _mingc.SWFShape_drawSizedGlyph

SWFShape_drawCubic = _mingc.SWFShape_drawCubic

SWFShape_drawCubicTo = _mingc.SWFShape_drawCubicTo

SWFShape_drawCharacterBounds = _mingc.SWFShape_drawCharacterBounds

newSWFMovieClip = _mingc.newSWFMovieClip

destroySWFMovieClip = _mingc.destroySWFMovieClip

SWFMovieClip_setNumberOfFrames = _mingc.SWFMovieClip_setNumberOfFrames

SWFMovieClip_nextFrame = _mingc.SWFMovieClip_nextFrame

SWFMovieClip_labelFrame = _mingc.SWFMovieClip_labelFrame

SWFMovieClip_add = _mingc.SWFMovieClip_add

SWFMovieClip_remove = _mingc.SWFMovieClip_remove

SWFMovieClip_setSoundStream = _mingc.SWFMovieClip_setSoundStream

SWFMovie_setSoundStreamAt = _mingc.SWFMovie_setSoundStreamAt

SWFMovieClip_startSound = _mingc.SWFMovieClip_startSound

SWFMovieClip_stopSound = _mingc.SWFMovieClip_stopSound

newSWFPrebuiltClip_fromFile = _mingc.newSWFPrebuiltClip_fromFile

newSWFPrebuiltClip_fromInput = _mingc.newSWFPrebuiltClip_fromInput

newSWFMovie = _mingc.newSWFMovie

newSWFMovieWithVersion = _mingc.newSWFMovieWithVersion

destroySWFMovie = _mingc.destroySWFMovie

SWFMovie_setRate = _mingc.SWFMovie_setRate

SWFMovie_setDimension = _mingc.SWFMovie_setDimension

SWFMovie_setNumberOfFrames = _mingc.SWFMovie_setNumberOfFrames

SWFMovie_addExport = _mingc.SWFMovie_addExport

SWFMovie_setBackground = _mingc.SWFMovie_setBackground

SWFMovie_setSoundStream = _mingc.SWFMovie_setSoundStream

SWFMovie_startSound = _mingc.SWFMovie_startSound

SWFMovie_stopSound = _mingc.SWFMovie_stopSound

SWFMovie_add = _mingc.SWFMovie_add

SWFMovie_remove = _mingc.SWFMovie_remove

SWFMovie_nextFrame = _mingc.SWFMovie_nextFrame

SWFMovie_labelFrame = _mingc.SWFMovie_labelFrame

SWFMovie_output = _mingc.SWFMovie_output

SWFMovie_save = _mingc.SWFMovie_save

SWFMovie_importCharacter = _mingc.SWFMovie_importCharacter

