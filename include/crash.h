/*
 * crash
 *
  >x%|.                         .-:::|   . . ... ...   .            :xOU[: -|%]>
yaU&%:.                      __axx]|]||=_ --:|_||||.|._- . .        .|3WWWya_|?"
OO3%]-                  -:|x400W33%||]%>|--:||%xdOUA]||-|::-..      .|]O33OOU3]|
___._                 ._|xU#RHY]||]3]>>xux; :||-|"*33x]||.--... .     _______|.:
HXH9]:-             _jmmZ3]33go||xu%]|]xOx;.---:|-:-::|~|:. . .. --.-|*HHHHHHX9]
"""~-             _|udWRA%|dO4nu>x3x]>|x3x; ---:-:::::-::-- .    -.. --"~~~~"~~-
yyyuu_.         .xqmW0X"=|gdWH3%xxx%]>||||:..------::::::---..  .-...|umWmmmmyux
#ZZX3-.        _|mWWRY?-||UZZ]]]%%x]]xx||--  .  . :--:----. -   .--. |3WW0WWWKCx
OOO3%-.       _|%UWW%):_|34O4%xx|3x3qm*+-  .  .  .. ----  .__||, -. -|xW00WW4OC]
44O3%:.      -%4WWWWUUU42%O4O%):3dO4{|   -||||| .  ---  -|%RRUUk%|...|%W00WZ4O3%
44O2%:.    ._|37?~]3U#WUZZUUUmWW4Y?-`  ._||:||||:_   .  x4WX<- -||;..|xW00WW4OC]
44O3%:.    ||Ox]|%3ORRUBRRRWW0WO:.   -]x%O333%3|=|- ..  34O|=:.-||:- |xW0WWZ4O3%
44O2%-.    |dZRURRRRRUBWBWUZH?+-   :--~3ZWWUBRU3c|-..   %dU]=: -||:- |xW00WW4OC]
44O3%:.  -|UWWBWBWWR#BH*^"-  . |||u_.  |3WWUWRW]>|_..   %qm|>-._|]:..|xW00WZ4OC%
44O3%:.  _|U#RRWWWZH??"- ... .-->xmu=_.jgWWRWWZ|||-- .  ]34gi- -||:- |x000WW4OCx
44O3%:.   ~~~~~~~-      __]||___x%%3XXUXXHX*3)||:----- .-"?X3xx=-. ..|%WW0WZ4OCx
44O3%:.                ._|xxxuuuu%%]]]*]?]]|||||:-  :-:.  -?~"~` ....|xW00WZUOCx
ZZZmu:.               .>x3|~--->33Oxoxx|x||||=-. -_|x|=-:..   . --.. |u000WWWm2x
33%%]-                -%33%=-||%333UA%333x>|-.. -|%44|=----.   ..:.. |]OOOO3%%]>
___._                 .|x3333xd3O33%??~||]]|  ::||]=|--:. .  .  :-.-  .||||___._
WWWB3:.                -||33%U23x=---|>||%>|.--::::-:---.   ..-|::..-|4000000WZO
~~"~-                    |xx3XX%__|x%]]33Oc|  ::-:---.    ...----::- -~~~~~"""~-
wwyyx:.                   ---~|]3%]||x%d3?~-.--        .. ----::::-.._uwwwwww]=|
RRR83-.                       -~~||>]|???-- .     . ....._--:-::::.- |3W00#@*~--
~~*3%:.                             . .  --- ..-----------::-:--::...|%#D*|__|?"
 */

#ifndef CRASH_H
#define CRASH_H

#include <flash.h>

#define CRASH_NCHARS 256

typedef struct crash_font_s {
	int width, height; /* per character */
	flash_image_t *chars[CRASH_NCHARS];
} crash_font_t;

/*extern crash_font_t *crash_loadfont(char *);*/
extern crash_font_t *crash_loadrawfont(char *, int, int, int);
extern void crash_printf(int, int, crash_font_t *, char *, ...);
extern void crash_sprintf(flash_image_t *, int, int, crash_font_t *,
                          char *, ...);
extern void crash_delete(crash_font_t *);

#endif

/* EOF crash.h */
