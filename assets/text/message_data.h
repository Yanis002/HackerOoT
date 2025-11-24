#include "text/message_data.h"
#include "config.h"

#if ENABLE_MM_TITLE_CARDS

DEFINE_MESSAGE(0x8000, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_VARIABLE,
MSG(/* MISSING */)
,
MSG("Example Scene")
,
MSG(/* MISSING */)
,
MSG(/* MISSING */)
)

DEFINE_MESSAGE(0x9000, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_VARIABLE,
MSG(/* MISSING */)
,
MSG("Undefined Scene")
,
MSG(/* MISSING */)
,
MSG(/* MISSING */)
)

#endif

DEFINE_MESSAGE(0x9001, TEXTBOX_TYPE_BLUE, TEXTBOX_POS_MIDDLE,
MSG(/* MISSING */)
,
MSG(
"Vous êtes reconnu coupable!"
FADE(40)
)
,
MSG(/* MISSING */)
,
MSG(/* MISSING */)
)

DEFINE_MESSAGE(0x9002, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
MSG(/* MISSING */)
,
MSG(
"En prison, il n'y a rien à voir,\n"
"et rien à faire."
FADE(40)
)
,
MSG(/* MISSING */)
,
MSG(/* MISSING */)
)

DEFINE_MESSAGE(0x9003, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
MSG(/* MISSING */)
,
MSG(
"J'oublie le silence qui n'existe pas à\n"
"la Santé où il y a beaucoup à entendre."
FADE(40)
)
,
MSG(/* MISSING */)
,
MSG(/* MISSING */)
)

DEFINE_MESSAGE(0x9004, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
MSG(/* MISSING */)
,
MSG(
SHIFT(20) "Le bruit y est hélas constant.\n"
FADE(40)
)
,
MSG(/* MISSING */)
,
MSG(/* MISSING */)
)

DEFINE_MESSAGE(0x9005, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
MSG(/* MISSING */)
,
MSG(
"A l'image du désert, la vie intérieure\n"
"se fortifie en prison."
FADE(40)
)
,
MSG(/* MISSING */)
,
MSG(/* MISSING */)
)

DEFINE_MESSAGE(0x9006, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
MSG(/* MISSING */)
,
MSG(
SHIFT(50) "Quelle indignité...\n"
FADE(120)
)
,
MSG(/* MISSING */)
,
MSG(/* MISSING */)
)

/*
 * The following two messages should be kept last and in this order.
 * Message 0xFFFD must be last to not break the message debugger (see R_MESSAGE_DEBUGGER_TEXTID).
 * Message 0xFFFC must be immediately before message 0xFFFD to not break Font_LoadOrderedFont.
 */
DEFINE_MESSAGE_FFFC(0xFFFC, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_VARIABLE,
MSG(
"０１２３４５６７８９あいうえおかきくけこ\n"
"さしすせそたちつてとなにぬねのはひふへほ\n"
"まみむめもやゆよらりるれろわをんぁぃぅぇ\n"
"ぉっゃゅょがぎぐげござじずぜぞだぢづでど\n"
"ばびぶべぼぱぴぷぺぽアイウエオカキクケコ\n"
"サシスセソタチツテトナニヌネノハヒフヘホ\n"
"マミムメモヤユヨラリルレロワヲンァィゥェ\n"
"ォッャュョガギグゲゴザジズゼゾダヂヅデド\n"
"バビブベボパピプペポヴＡＢＣＤＥＦＧＨＩ\n"
"ＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺａｂｃ\n"
"ｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗ\n"
"ｘｙｚ　┯？！：−（）゛゜，．／"
)
,
MSG(
"0123456789\n"
"ABCDEFGHIJKLMN\n"
"OPQRSTUVWXYZ\n"
"abcdefghijklmn\n"
"opqrstuvwxyz\n"
" -.\n"
)
,
MSG(/* UNUSED */)
,
MSG(/* UNUSED */)
)
DEFINE_MESSAGE(0xFFFD, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_VARIABLE, MSG(), MSG(), MSG(), MSG())
