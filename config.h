/*  ▄▄▄        ██████  ██░ ██  ██▓ ███▄    █  */
/* ▒████▄    ▒██    ▒ ▓██░ ██▒▓██▒ ██ ▀█   █  */
/* ▒██  ▀█▄  ░ ▓██▄   ▒██▀▀██░▒██▒▓██  ▀█ ██▒ */
/* ░██▄▄▄▄██   ▒   ██▒░▓█ ░██ ░██░▓██▒  ▐▌██▒ */
/*  ▓█   ▓██▒▒██████▒▒░▓█▒░██▓░██░▒██░   ▓██░ */
/*  ▒▒   ▓▒█░▒ ▒▓▒ ▒ ░ ▒ ░░▒░▒░▓  ░ ▒░   ▒ ▒  */
/*   ▒   ▒▒ ░░ ░▒  ░ ░ ▒ ░▒░ ░ ▒ ░░ ░░   ░ ▒░ */
/*   ░   ▒   ░  ░  ░   ░  ░░ ░ ▒ ░   ░   ░ ░  */
/*       ░  ░      ░   ░  ░  ░ ░           ░  */

/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
#define PrintScreenDWM 0x0000ff61
/* appearance */
static const unsigned int borderpx = 2.0; /* border pixel of windows */
static const unsigned int gappx = 3;      /* gaps between windows */
static const unsigned int snap = 32;      /* snap pixel */
static const int showbar = 1;             /* 0 means no bar */
static const int topbar = 1;              /* 0 means bottom bar */
#define ICONSIZE 20                       /* icon size */
#define ICONSPACING 5                     /* space between icon and title */
static const char *fonts[] = {"JetBrains Mono Nerd Font:size=10"};
static const char dmenufont[] = "JetBrains Mono Nerd Font:size=10";
/* Colorscheme */

static const char norm_fg[] = "#d9d3d7";
static const char norm_bg[] = "#0a0906";
static const char norm_border[] = "#979396";

static const char sel_fg[] = "#d9d3d7";
static const char sel_bg[] = "#8F6536";
static const char sel_border[] = "#d9d3d7";

static const char urg_fg[] = "#d9d3d7";
static const char urg_bg[] = "#69625C";
static const char urg_border[] = "#69625C";

static const char *colors[][3] = {
    /*               fg           bg         border                         */
    [SchemeNorm] = {norm_fg, norm_bg, norm_border}, // unfocused wins
    [SchemeSel] = {sel_fg, sel_bg, sel_border},     // the focused win
    [SchemeUrg] = {urg_fg, urg_bg, urg_border},
};

/* tagging */
static const char *tags[] = {"", "", "", "", "", "", "", "", ""};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {"kitty", NULL, NULL, 1, 0, -1},          {"firefox", NULL, NULL, 2, 0, -1},
    {"pcmanfm", NULL, NULL, 3, 0, -1},     {"yad", NULL, NULL, 0, 1, -1},
    {"pavucontrol", NULL, NULL, 0, 1, -1},
};

/* layout(s) */
static const float mfact = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */

#include "grid.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
    {"HHH", grid},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {".config/rofi/bin/launcher_colorful", NULL};
static const char *termcmd[] = {"kitty", NULL};
/* Aplications */
static const char *browser[] = {"firefox", NULL};
static const char *file[] = {"pcmanfm", NULL};
static const char *editor[] = {"emacsclient", "-c", "-a", "emacs", NULL};
/* Scripts */
static const char *logout[] = {".config/rofi/bin/menu_powermenu", NULL};
static const char *apps[] = {".config/rofi/bin/menu_apps", NULL};
static const char *calc[] = {".local/bin/calc_rofi", NULL};
static const char *music[] = {".local/bin/dm-sounds", NULL};
static const char *wifi[] = {".local/bin/dm-wifi", NULL};
static const char *config[] = {".local/bin/dm-confedit", NULL};
static const char *programs[] = {".local/bin/dm-kill", NULL};
static const char *record[] = {".local/bin/dm-record", NULL};
static const char *browse[] = {".local/bin/dm-browser", NULL};
static const char *todo[] = {".local/bin/todo", NULL};
/* Volume */
static const char *upvol[] = {"/usr/bin/pactl", "set-sink-volume", "0", "+5%",
                              NULL};
static const char *downvol[] = {"/usr/bin/pactl", "set-sink-volume", "0", "-5%",
                                NULL};
static const char *mutevol[] = {"/usr/bin/pactl", "set-sink-mute", "0",
                                "toggle", NULL};
/* Brightnes */
static const char *brightness_up[] = {"xbacklight", "-inc", "2", NULL};
static const char *brightness_down[] = {"xbacklight", "-dec", "2", NULL};

static const char *screenshot[] = {"scrot", NULL};

static Key keys[] = {
    /* modifier                     key        function        argument */

    /* dmenu scripts and stuff */

    {MODKEY, XK_d, spawn, {.v = dmenucmd}},
    {MODKEY, XK_a, spawn, {.v = apps}},
    {MODKEY, XK_x, spawn, {.v = logout}},
    {MODKEY, XK_m, spawn, {.v = music}},
    {MODKEY, XK_b, spawn, {.v = browse}},
    {MODKEY, XK_o, spawn, {.v = todo}},
    {MODKEY, XK_c, spawn, {.v = config}},
    {MODKEY, XK_p, spawn, {.v = programs}},
    {MODKEY, XK_r, spawn, {.v = record}},
    {MODKEY | ShiftMask, XK_c, spawn, {.v = calc}},
    {MODKEY | ShiftMask, XK_w, spawn, {.v = wifi}},
    {0, PrintScreenDWM, spawn, {.v = screenshot}},

    /* application I use */

    {MODKEY, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_w, spawn, {.v = browser}},
    {MODKEY, XK_e, spawn, {.v = editor}},
    {MODKEY, XK_n, spawn, {.v = file}},

    /* volume keys */

    {0, XF86XK_AudioLowerVolume, spawn, {.v = downvol}},
    {0, XF86XK_AudioMute, spawn, {.v = mutevol}},
    {0, XF86XK_AudioRaiseVolume, spawn, {.v = upvol}},

    /* volume keys */

    {0, XF86XK_MonBrightnessUp, spawn, {.v = brightness_up}},
    {0, XF86XK_MonBrightnessDown, spawn, {.v = brightness_down}},
    /* all the layout and stuff   */

    {MODKEY, XK_semicolon, spawn, SHCMD("skippy-xd")},
    {MODKEY | ShiftMask, XK_b, togglebar, {0}},
    {MODKEY | ShiftMask, XK_j, rotatestack, {.i = +1}},
    {MODKEY | ShiftMask, XK_k, rotatestack, {.i = -1}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY | ShiftMask, XK_l, incnmaster, {.i = +1}},
    {MODKEY | ShiftMask, XK_h, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY, XK_q, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_g, setlayout, {.v = &layouts[3]}},
    {MODKEY, XK_Tab, setlayout, {0}},
    {MODKEY, XK_space, cyclelayout, {.i = -1}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8){MODKEY | ShiftMask, XK_q, quit, {0}},
    {MODKEY | ControlMask | ShiftMask, XK_q, quit, {1}},
    {MODKEY | ShiftMask, XK_r, quit, {1}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function argument
     */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
