#!/usr/bin/php
<?php
$m = new SWFMovie();


/* SWF_SETBACKGROUNDCOLOR */
$m->setBackground(0x00, 0xff, 0x00);

/* SWF_SHOWFRAME */
$m->nextFrame(); /* end of frame 1 */

$m->save("test02.swf");
?>