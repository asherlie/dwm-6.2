#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/param.h>

#include <time.h>

/* to determine number of monitors */
#include <X11/extensions/Xrandr.h>
#include <X11/Xlib.h>

#define br_fn "/sys/class/backlight/intel_backlight/brightness"
#define mon_br_fn "/home/asher/mon_brightness"
#define redshift_fn "/home/asher/rs_state"

#define RED 3500
#define BLU 6500

#define MAX_BR 4648
#define MIN_BR 20

/* sets *br to current brightness and returns an open FILE* */
FILE* get_br(int* br){
    FILE* fp = fopen(br_fn, "r+");
    if(!fp)return NULL;
    if(fscanf(fp, "%i", br) != 1){
        fclose(fp);
        return NULL;
    }
    fseek(fp, 0L, SEEK_SET);
    return fp;
}

FILE* get_br_mon(float* br){
    FILE* fp = fopen(mon_br_fn, "r+");
    if(!fp)return NULL;
    if(fscanf(fp, "%f", br) != 1){
        fclose(fp);
        return NULL;
    }
    fseek(fp, 0L, SEEK_SET);
    return fp;
}

FILE* get_rs(_Bool* red, _Bool close){
    char c;
    FILE* fp = fopen(redshift_fn, "r+");
    if(!fp)return NULL;
    if((c = fgetc(fp)) == EOF){
        fclose(fp);
        return NULL;
    }
    *red = (tolower(c) == 'r');
    fseek(fp, 0L, SEEK_SET);
    if(close){
        fclose(fp);
        return (FILE*)0x1;
    }
    return fp;
}

/* if !dec, increase by amt - otherwise, decrease */
_Bool br_set(_Bool dec, int amt){
    int br;
    FILE* fp;
    if(!(fp = get_br(&br)))return 0;

    int adj = br + ((dec) ? -amt : amt);

    _Bool ret = 0;
    /*printf("adj: %i\n", adj);*/
    if(adj < MIN_BR)adj = MIN_BR;
    else if(adj > MAX_BR)adj = MAX_BR;

    /*printf("br: %i, %i\n", br);*/
        ret = fprintf(fp, "%i\n", adj) == 1;
    fclose(fp);
    return ret;
}

_Bool br_set_mon(_Bool dec, float amt){
    float plus = (dec ? -1 : 1)*amt, cur_br, target;
    _Bool ret, red;
    char cmd[50] = {0};
    FILE* fp = get_br_mon(&cur_br);
    if(!fp)return 0;

    target = MAX(MIN(cur_br+plus, 1.0), 0.1);
    /*printf("cur: %f, target %f\n", cur_br, target);*/
    ret = fprintf(fp, "%f\n", target) == 1;
    fclose(fp);
    if(!get_rs(&red, 1))return 0;
    snprintf(cmd, 50, "redshift -O %i -P -b %f", red ? RED : BLU, target);
    system(cmd);
    return ret;
}

void rs_set(_Bool red){
    _Bool cur_red;
    float cur_br;
    char cmd[50];
    FILE* fp = get_rs(&cur_red, 0), * fp_br;
    if(!fp || !(fp_br = get_br_mon(&cur_br)))return;
    if(cur_red == red)return;
    fclose(fp_br);
    snprintf(cmd, 50, "redshift -O %i -P -b %f", red ? RED : BLU, cur_br);
    puts(cmd);
    system(cmd);
    fputc(red ? 'r' : 'b', fp);
    fclose(fp);
}

int nmons(){
    int nc = 0;
    Display* dpy = XOpenDisplay(NULL);
    XRROutputInfo* oi;
    XRRScreenResources* screen;

    if(!dpy){
        return 0;
    }

    screen = XRRGetScreenResources(dpy, DefaultRootWindow(dpy));

    for(int i = 0; i < screen->noutput; ++i){
        oi = XRRGetOutputInfo(dpy, screen, screen->outputs[i]);
        if(!oi->connection)++nc;
    }
    return nc;
}

int hour(){
    struct tm;
    time_t t;
    time(&t);
    return localtime(&t)->tm_hour;
}

int br_set_agnostic(_Bool dec, float amt, int factor){
    int hr, adj_factor = factor;
    if(nmons() > 1){
        return br_set_mon(dec, amt);
    }
    /* we want less granularity before 11 PM */
    hr = hour();
    if(hr > 9 || hr < 23)adj_factor *= 3;
    return br_set(dec, (int)(amt*adj_factor));
}

int main(int a, char** b){
    if(a < 3){
        if(a == 2)
            br_set(*b[1] == 'd', 300);
        return 1;
    }

    /*int amt = atoi(b[2]);*/
    float amt_f = atof(b[2]);
    /*printf("amt: %f\n", amt_f);*/

    switch(*b[1]){
        case 'i':
            /*br_set(0, amt);*/
            // was set to 1000 for a long time, seems like it could be too granular at night
            /*br_set_agnostic(0, amt_f, 1000);*/
            br_set_agnostic(0, amt_f, 600);
            /*br_set_mon(0, amt_f);*/
            break;
        case 'd':
            /*printf("%i\n", br_set(1, amt));*/
            /*printf("%i\n", br_set_mon(1, amt_f));*/
            br_set_agnostic(1, amt_f, 1000);
            break;
        case 'r':
            rs_set(1);
            break;
        case 'b':
            rs_set(0);
            break;
    }
    return 0;
}
