#include <stdio.h>
#include <tonc.h>

u8 txt_scrolly = 8;

const char *names[] =
{	"C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#", "A ", "A#", "B "	};

// Play a note and show which one was played
void note_play(int note, int octave) {

	// Clear next top and current rows
	SBB_CLEAR_ROW(31, (txt_scrolly/8-2)&31);
	SBB_CLEAR_ROW(31, txt_scrolly/8);	

	// Display note and scroll
	tte_printf("#{P:16,%d;cx:0}%-2s%+2d", txt_scrolly, names[note], octave);

	txt_scrolly -= 8;
	REG_BG0VOFS= txt_scrolly-8;

	// Play the actual note
	REG_SND1FREQ = SFREQ_RESET | SND_RATE(note, octave);
}

int main() {
	// from tonc-sn1
    REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;

	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	tte_init_se(0, BG_CBB(0) | BG_SBB(31), 0, CLR_ORANGE, 0, NULL, NULL);
	tte_init_con();
	pal_bg_mem[0x11] = CLR_GREEN;

	// turn sound on
	REG_SNDSTAT    = SSTAT_ENABLE;
	// snd1 on left/right ; both full volume
	REG_SNDDMGCNT  = SDMG_BUILD_LR(SDMG_SQR1, 7);
	// DMG ratio to 100%
	REG_SNDDSCNT   = SDS_DMG100;

	// no sweep
	REG_SND1SWEEP  = SSW_OFF;
	// envelope: vol=12, decay, max step time (7) ; 50% duty
	REG_SND1CNT    = SSQR_ENV_BUILD(12, 0, 7) | SSQR_DUTY1_2;
	REG_SND1FREQ   = 0;
    
    // just tries to play never gonna give you up on the gameboy
    const int notes[] = {	
    NOTE_B, NOTE_CIS, NOTE_D, NOTE_D, NOTE_E, NOTE_CIS, NOTE_B, NOTE_A, 
    NOTE_B, NOTE_B, NOTE_CIS, NOTE_D, NOTE_B, NOTE_A, NOTE_A, NOTE_A, NOTE_E,
    NOTE_B, NOTE_B, NOTE_CIS, NOTE_D, NOTE_B, NOTE_D, NOTE_E, NOTE_CIS, NOTE_B, NOTE_CIS, NOTE_B, NOTE_A,
    NOTE_B, NOTE_B, NOTE_CIS, NOTE_D, NOTE_B, NOTE_A, NOTE_E, NOTE_E, NOTE_E, NOTE_FIS, NOTE_E,
    NOTE_D, NOTE_E, NOTE_FIS, NOTE_D, NOTE_E, NOTE_E, NOTE_E, NOTE_FIS, NOTE_E, NOTE_A,
    NOTE_B, NOTE_CIS, NOTE_D, NOTE_B, NOTE_E, NOTE_FIS, NOTE_E,
    NOTE_A, NOTE_B, NOTE_D, NOTE_B, NOTE_FIS, NOTE_FIS, NOTE_E,
    NOTE_A, NOTE_B, NOTE_D, NOTE_B, NOTE_E, NOTE_E, NOTE_D, NOTE_CIS, NOTE_B,
    NOTE_A, NOTE_B, NOTE_D, NOTE_B, NOTE_D, NOTE_E, NOTE_CIS, NOTE_A, NOTE_A, NOTE_E, NOTE_D,
    NOTE_A, NOTE_B, NOTE_D, NOTE_B, NOTE_FIS, NOTE_FIS, NOTE_E,
    NOTE_A, NOTE_B, NOTE_D, NOTE_B, NOTE_A, NOTE_CIS, NOTE_D, NOTE_CIS, NOTE_B,
    NOTE_A, NOTE_B, NOTE_D, NOTE_B, NOTE_D, NOTE_E, NOTE_CIS, NOTE_A, NOTE_A, NOTE_E, NOTE_D	};

    int i;
    // loop through all the notes and play them
    for(i=0; i < 111; i++) {
        note_play(notes[i], notes[i]);
        VBlankIntrDelay(30);
    }
    // don't continue onto anything
    return 0;
}
