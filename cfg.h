/* See LICENSE file for copyright and license details. */
/* appearance */
//#include <X11/XF86keysym.h>
//#define ROMANNUM
static const char *fonts[] = {
	"monospace:size=11"
};
static const unsigned int gappx = 8;
static const char dmenufont[]       = "monospace:size=10";
static const char normbordercolor[] = "#444444";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#005577";
static const char selbgcolor[]      = "#005577";
static const char selfgcolor[]      = "#eeeeee";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

/* tagging */
#ifdef ROMANNUM
static const char *tags[] = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX" };
#else
//static const char *tags[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
#endif

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
      { "|||",      col  },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char* mutecmd[] = { "/home/asher/bin/alsa_t/alsa_t", NULL };
static const char* vol_up_cmd[]  = { "amixer", "-q", "sset", "Master", "10%+", NULL };
static const char* vol_dwn_cmd[] = { "amixer", "-q", "sset", "Master", "10%-", NULL };
//static const char* rs_toggle_cmd[] = { "/home/asher/bin/rs_t/rs_t", "1500", "6500", "t", NULL }; // the lower the first arg, the redder
//static const char* rs_inc_cmd[] = {"/home/asher/bin/rs_t/rs_t", "1500", "6500", "i", NULL };
//static const char* rs_dec_cmd[] = {"/home/asher/bin/rs_t/rs_t", "1500", "6500", "d", NULL };
static const char* mpc_toggle_cmd[] = { "mpc", "toggle", NULL };
static const char* mpc_prev_cmd[] = { "mpc", "prev", NULL };
static const char* mpc_next_cmd[] = { "mpc", "next", NULL };
//static const char* reboot_cmd[] = { "reboot", "null" };
static const char* browser_cmd[] = { "chromium", NULL };
static const char* incog_browser_cmd[] = { "chromium", "--incognito", NULL };
//static const char* rshift_mr[] = {"/home/asher/bin/"};
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "xfce4-terminal", NULL };
static const char *find_str[]  = { "Chromium", NULL };
//static const char *sendclip[]  = { "/home/asher/sendclip/sc", "\"$(xsel)\"", NULL };
static const char *sendclip[]  = { "/home/asher/sendsel.sh", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
      { 0,                            0x1008ff12,spawn,          {.v = mutecmd } },
      { 0,                            0x1008ff11,spawn,          {.v = vol_dwn_cmd } },
      { 0,                            0x1008ff13,spawn,          {.v = vol_up_cmd } },
      //{ 0,                            0xffc5    ,spawn,          {.v = sendclip} },
      // 22 if with shift, idk
      /* shift, alt, ' */
      { MODKEY|ShiftMask,             0x27      ,spawn,          {.v = sendclip} },
      /*
       *{ 0,                            0xffbe,    spawn,          {.v = rs_toggle_cmd } },
       *{ 0,                            XK_F11,    spawn,          {.v = rs_inc_cmd } },
       *{ 0,                            XK_F9,     spawn,          {.v = rs_dec_cmd } },
       *{ 0,                            XK_F10,    spawn,          {.v = rs_toggle_cmd } },
       */
      //{ 0,                            0x1008ff2d,spawn,          {.v = reboot_cmd } },
      { 0,                            0x1008ff41,spawn,          {.v = mpc_toggle_cmd } },
      { 0,                            0x1008ff16,spawn,          {.v = mpc_prev_cmd } },
      { 0,                            0x1008ff17,spawn,          {.v = mpc_next_cmd } },
      { 0,                            0x1008ff14,spawn,          {.v = mpc_toggle_cmd } },
      //{ 0,                            0x1008ffb2,quit,           {0} },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             0x5c,      spawn,          {.v = browser_cmd } },
	{ MODKEY|ShiftMask,             0x5d,      spawn,          {.v = incog_browser_cmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_s,      iter,           {.v = find_str} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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

