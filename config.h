/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";

struct ubuttons_t{
    const char* ub_txt;
    const char** uaction;
    _Bool activated;
};

/* ubuttons only appear with external monitors if not set */
const _Bool force_ubuttons = 0;
int cur_ubutton_press = -1;
/* auto laptop/monitor mode */
const _Bool asher_x220 = 1;

/* relevant only for asher's home x220 dock setup */
_Bool docked = 0;

/* colors that match background to play around with */
static const char col_salmon[]      = "#CD928C";
static const char col_deepblue[]    = "#187E94";

static const char *colors[][4]      = {
	/*               fg         bg         border   */
#ifndef BACKMATCH
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
#else
    [SchemeNorm] = { col_deepblue, col_gray1, col_deepblue},
    [SchemeSel]  = { col_gray4, col_cyan,  col_salmon},
#endif
    [SchemeAlt] = { col_gray1, col_gray3, col_gray2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };


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
//static const int resizehints = 1;    [> 1 means respect size hints in tiled resizals <]
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[==]",     equal_size},
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

static const char* br_up_cmd[] = { "/home/asher/ashbin/br/br", "i", "0.1", NULL };
static const char* br_down_cmd[] = { "/home/asher/ashbin/br/br", "d", "0.1", NULL };

static const char* red_cmd[] = { "/home/asher/ashbin/br/br", "r", "_", NULL};
static const char* blue_cmd[] = { "/home/asher/ashbin/br/br", "b", "_", NULL};

static const char* browser_cmd[] = { "chromium", NULL };
static const char* incog_browser_cmd[] = { "chromium", "--incognito", NULL };
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "xfce4-terminal", NULL };

static const char* sendclip[]  = { "/home/asher/sendsel.sh", NULL };
static const char* airpod_con[] = {"bluetoothctl", "connect", "A4:C6:F0:D7:4E:87", NULL};
static const char* airpod_dcon[] = {"bluetoothctl", "disconnect", "A4:C6:F0:D7:4E:87", NULL};
/* TODO: maybe this should just be systemctl suspend - no need to hibernate
 * when docked - fn key is also accessible when docked so waking won't be a problem
 */
/* TODO: should this be complete hibernation? we can already 
 * trigger hybrid-sleep using sleep key - see /etc/systemd/logind.conf
 */
static const char* hibernate[] = {"systemctl", "hybrid-sleep", NULL};
static const char* shuffle_wallpaper[] = {"/home/asher/ashbin/papes/random_pape.sh", NULL};

static const char* vpn_connect[] = {"echo", "vpn_con", ">>", "~/vpn_log", NULL};
static const char* vpn_disconnect[] = {"echo", "vpn_dcon", ">>", "~/vpn_log", NULL};

/* TODO: should these be NULL terminated? */
static const char* set_mons[] = {"/home/asher/ashbin/mon/mon.sh"};
static const char* set_nomons[] = {"/home/asher/ashbin/mon/lap.sh"};

static struct ubuttons_t ubuttons[] = {
    {.ub_txt = "PC",    .uaction = airpod_con,        .activated = 0},
    {.ub_txt = "PD",    .uaction = airpod_dcon,       .activated = 0},
    {.ub_txt = "WP",    .uaction = shuffle_wallpaper, .activated = 0},
    {.ub_txt = "RS",    .uaction = red_cmd,           .activated = 0},
    {.ub_txt = "BS",    .uaction = blue_cmd,          .activated = 0},
    {.ub_txt = "BR-U",  .uaction = br_up_cmd,         .activated = 0},
    {.ub_txt = "BR-D",  .uaction = br_down_cmd,       .activated = 0},
    {.ub_txt = "VPN-C", .uaction = vpn_connect,       .activated = 0},
    {.ub_txt = "VPN-D", .uaction = vpn_disconnect,    .activated = 0},
};

static Key keys[] = {
	/* modifier                     key        function        argument */
    { 0,                            0x1008ff12,spawn,          {.v = mutecmd } },
    { 0,                            0x1008ff11,spawn,          {.v = vol_dwn_cmd } },
    { 0,                            0x1008ff13,spawn,          {.v = vol_up_cmd } },
                                    /* mute mic button */
    { 0,                            0x1008ffb2, spawn,         {.v = airpod_con} },
    { MODKEY,                       0x1008ffb2, spawn,         {.v = airpod_dcon} },
                                    /* thinkvantage button */
    { 0,                            0x1008ff41, spawn,         {.v = hibernate} },
      

    /* brightness */
    { 0,                            0x1008ff02,spawn,          {.v = br_up_cmd} },
    { 0,                            0x1008ff03,spawn,          {.v = br_down_cmd} },

                                    /* f9 */
    { 0,                            0xffc6,    spawn,          {.v = shuffle_wallpaper} },
    /* mon/lap modes must exist */  
    /* on both laptop/keyboard */   /* f1 */
    { 0,                            0xffbe,    spawn,          {.v = set_mons} },
                                    /* f2 */
    { 0,                            0xffbf,    spawn,          {.v = set_nomons} },

    /* redshift, blueshift */
    /* PrtSc,    ScrLk*/
    { 0,                            0xff61,spawn,          {.v = red_cmd} },
    { 0,                            0xff14,spawn,          {.v = blue_cmd} },

    /* shift, alt, ' */
    { MODKEY|ShiftMask,             0x27      ,spawn,          {.v = sendclip} },
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
	{ MODKEY,                       XK_e,      setlayout,      {.v = &layouts[3]} },
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
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
    { ClkUbutton,           0,              Button1,        press_ubutton,  {.v = 0} },

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
