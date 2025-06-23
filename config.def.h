/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int borbarpx  = 0;        /* border pixel of bar */
static const unsigned int gappx     = 8;        /* gaps between windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int user_bh            = 6;        /* 2 is the default spacing around the bar's font */
static const char *fonts[]          = {"Terminus:size=11:style=Bold", "Siji:style=Regular" };
static const char dmenufont[]       = "Terminus:size=11:style=Bold";
static const char col_text1[]       = "#808080";
static const char col_text2[]       = "#000000";
static const char col_bg1[]         = "#000000";
static const char col_bg2[]         = "#808080";
static const char col_border2[]     = "#808080";
static const char col_border1[]     = "#202020";
static const char col_borderbar[]   = "#000000";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_text1, col_bg1,   col_border1 },
	[SchemeSel]  = { col_text2, col_bg2,   col_border2  },
};

static const char *const autostart[] = {
	"xwallpaper", "--tile", ".local/share/wallpaper/WALL.png", NULL,
	"xset", "r", "rate", "450", "80", NULL,
	"xrdb", "-merge", ".config/X11/xresources", NULL,
    "bash", "-c", "dwm_stats 1", NULL,
	"sh", "-c", "wp-setup", NULL,
	"sxhkd", NULL,
	"picom", NULL,
	"dunst", NULL,
	NULL /* terminate */
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-c", "mu-term", "-g", "120x40", "-e", "ncmpcpp", NULL };
const char *spcmd2[] = {"keepassxc", NULL };
const char *spcmd3[] = {"qpwgraph", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"mu-term",      spcmd1},
	{"keepass",      spcmd2},
	{"qpwgraph",     spcmd3},
};

/* tagging */
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
static const int momentaryalttags = 0; /* 1 means alttags will show only when key is held down*/

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "mu-term",   NULL,       NULL,       SPTAG(0),         1,           -1 },
	{ "KeePassXC", NULL,       NULL,       SPTAG(1),         1,           -1 },
	{ "qpwgraph",  NULL,       NULL,       SPTAG(2),         1,           -1 },
	{ "tabbed",    "st-float", NULL,       0,                1,           -1 },
	{ "mpv",       "mpvk",     "WEBCAM",   0,                1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
	{ "",      monocle },
	{ "",      deck },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-p", "CMD: ", "-m", dmenumon, "-fn", dmenufont, topbar ? NULL : "-b", NULL };
static const char *termcmd[]  = { "tabbed", "-k", "-c", "-r2", "st", "-w", "''", NULL };
static const char *fermcmd[]  = { "tabbed", "-n", "st-float", "-k", "-c", "-r2", "st", "-w", "''", NULL };

static const Key keys[] = {
	/* modifier            key            function            argument */
	{ MODKEY,              XK_r,          spawn,              {.v = dmenucmd } },
	{ MODKEY,              XK_Return,     spawn,              {.v = termcmd } },
	{ MODKEY|Mod1Mask,     XK_Return,     spawn,              {.v = fermcmd } },
	{ MODKEY,              XK_b,          togglebar,          {0} },
	{ Mod1Mask,            XK_Tab,        focusstack,         {.i = +1 } },
	{ Mod1Mask|ShiftMask,  XK_Tab,        focusstack,         {.i = -1 } },
	{ MODKEY|Mod1Mask,     XK_Tab,        toggleall,          {0} },

	{ MODKEY,              XK_k,          focusstack,         {.i = +1 } },
	{ MODKEY,              XK_l,          focusstack,         {.i = -1 } },
	{ MODKEY,              XK_i,          incnmaster,         {.i = +1 } },
	{ MODKEY,              XK_o,          incnmaster,         {.i = -1 } },
	{ MODKEY,              XK_j,          setmfact,           {.f = -0.1} },
	{ MODKEY,              XK_semicolon,  setmfact,           {.f = +0.1} },
	{ MODKEY|Mod1Mask,     XK_k,          inplacerotate,      {.i = +1} },
	{ MODKEY|Mod1Mask,     XK_l,          inplacerotate,      {.i = -1} },
	{ MODKEY|Mod1Mask,     XK_i,          aspectresize,       {.i = +32} },
	{ MODKEY|Mod1Mask,     XK_o,          aspectresize,       {.i = -32} },
	{ MODKEY,                       XK_Down,  moveresize,     {.v = "0x 32y 0w 0h" } },
	{ MODKEY,                       XK_Up,    moveresize,     {.v = "0x -32y 0w 0h" } },
	{ MODKEY,                       XK_Right, moveresize,     {.v = "32x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,  moveresize,     {.v = "-32x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,  moveresize,     {.v = "0x 0y 0w 32h" } },
	{ MODKEY|ShiftMask,             XK_Up,    moveresize,     {.v = "0x 0y 0w -32h" } },
	{ MODKEY|ShiftMask,             XK_Right, moveresize,     {.v = "0x 0y 32w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,  moveresize,     {.v = "0x 0y -32w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,    moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,  moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,  moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right, moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,    moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,  moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,  moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right, moveresizeedge, {.v = "R"} },
	{ MODKEY,              XK_Tab,        view,               {0} },
	{ MODKEY,              XK_Escape,     killclient,         {0} },
	{ MODKEY,              XK_a,          setlayout,          {.v = &layouts[0]} },
	{ MODKEY,              XK_s,          setlayout,          {.v = &layouts[1]} },
	{ MODKEY,              XK_d,          setlayout,          {.v = &layouts[3]} },
	{ MODKEY,              XK_f,          setlayout,          {.v = &layouts[2]} },
/*	{ MODKEY,              XK_space,      setlayout,          {0} },*/
	{ MODKEY,              XK_space,      togglefloating,     {0} },
	{ MODKEY,              XK_z,          zoom,               {0} },
	{ MODKEY,              XK_x,          togglefullscr,      {0} },
	{ MODKEY,              XK_c,          movecenter,         {0} },
	{ MODKEY|ShiftMask,    XK_space,      togglealwaysontop,  {0} },
	{ MODKEY,              XK_0,          view,               {.ui = ~0 } },
	{ MODKEY|ShiftMask,    XK_0,          tag,                {.ui = ~0 } },
	{ MODKEY,              XK_comma,      focusmon,           {.i = -1 } },
	{ MODKEY,              XK_period,     focusmon,           {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_comma,      tagmon,             {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_period,     tagmon,             {.i = +1 } },
	{ MODKEY,              XK_n,          togglealttag,       {0} },
	{ MODKEY|Mod1Mask,     XK_1,          togglescratch,      {.ui = 0 } },
	{ MODKEY|Mod1Mask,     XK_2,          togglescratch,      {.ui = 1 } },
	{ MODKEY|Mod1Mask,     XK_3,          togglescratch,      {.ui = 2 } },
	TAGKEYS(               XK_1,                               0)
	TAGKEYS(               XK_2,                               1)
	TAGKEYS(               XK_3,                               2)
	TAGKEYS(               XK_4,                               3)
	TAGKEYS(               XK_5,                               4)
	TAGKEYS(               XK_6,                               5)
	TAGKEYS(               XK_7,                               6)
	TAGKEYS(               XK_8,                               7)
	TAGKEYS(               XK_9,                               8)
	{ MODKEY|ControlMask,  XK_Escape,     quit,               {0} },
	{ MODKEY|Mod1Mask,     XK_Escape,     quit,               {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

