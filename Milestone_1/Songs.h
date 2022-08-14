/*
 * Songs.h
 *
 *  Created on: Oct 13, 2021
 *      Author: Patrick Kelling, Christopher Andrew
 */

#ifndef SONGS_H_
#define SONGS_H_

// buzzer struggles to play these
#define NOTE_CS6 1109
#define NOTE_D6  1175

// Best Section
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

// Basically cant use these
#define NOTE_E8  5274
#define NOTE_F8  5588
#define NOTE_FS8 5920
#define NOTE_G8  6272

// starts working here again but very high pitched
#define NOTE_GS8 6645
#define NOTE_A8  7040
#define NOTE_AS8 7459
#define NOTE_B8  7902
#define NOTE_C9  8372
#define NOTE_CS9 8870
#define NOTE_D9  9397
#define NOTE_DS9 9956
#define NOTE_E9  10548
#define NOTE_F9  11175
#define NOTE_FS9 11840
#define NOTE_G9  12544
#define NOTE_GS9 13290
#define NOTE_A9  14080
#define NOTE_AS9 14917
#define NOTE_B9  15804

#define REST      0


int starwars[] = {
		REST,2, NOTE_D7,8, NOTE_B6,4, NOTE_D7,8, //1
				  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,
				  REST,8, NOTE_A6,8, NOTE_FS7,8, NOTE_E7,4, NOTE_D7,8,
				  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,
				  REST,4, NOTE_D7,8, NOTE_B6,4, NOTE_D7,8,
				  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,

				  REST,8, NOTE_B6,8, NOTE_B6,8, NOTE_G6,4, NOTE_B6,8, //7
				  NOTE_A6,4, NOTE_B6,8, NOTE_A6,4, NOTE_D6,2,
				  REST,4, NOTE_D7,8, NOTE_B6,4, NOTE_D6,8,
				  NOTE_CS6,4, NOTE_D6,8, NOTE_CS6,4, NOTE_A6,2,
				  REST,8, NOTE_A6,8, NOTE_FS7,8, NOTE_E7,4, NOTE_D7,8,
				  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,

				  REST,4, NOTE_D7,8, NOTE_B6,4, NOTE_D7,8, //13
				  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,
				  REST,8, NOTE_B6,8, NOTE_B6,8, NOTE_G6,4, NOTE_B6,8,
				  NOTE_A6,4, NOTE_B6,8, NOTE_A6,4, NOTE_D6,8, NOTE_D6,8, NOTE_FS6,8,
				  NOTE_E6,-1,
				  REST,8, NOTE_D6,8, NOTE_E6,8, NOTE_FS6,-1,

				  REST,8, NOTE_D6,8, NOTE_D6,8, NOTE_FS6,8, NOTE_F6,-1, //20

				  REST,8, NOTE_D6,8, NOTE_F6,8, NOTE_E6,-1, //end 1

				  //repeats from 1

				  REST,2, NOTE_D7,8, NOTE_B6,4, NOTE_D7,8, //1
				  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,
				  REST,8, NOTE_A6,8, NOTE_FS7,8, NOTE_E7,4, NOTE_D7,8,
				  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,
				  REST,4, NOTE_D7,8, NOTE_B6,4, NOTE_D7,8,
				  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,

				  REST,8, NOTE_B6,8, NOTE_B6,8, NOTE_G6,4, NOTE_B6,8, //7
				  NOTE_A6,4, NOTE_B6,8, NOTE_A6,4, NOTE_D6,2,
				  REST,4, NOTE_D6,8, NOTE_B6,4, NOTE_D7,8,
				  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,
				  REST,8, NOTE_A6,8, NOTE_FS7,8, NOTE_E7,4, NOTE_D7,8,
				  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,

				  REST,4, NOTE_D7,8, NOTE_B6,4, NOTE_D7,8, //13
				  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,
				  REST,8, NOTE_B6,8, NOTE_B6,8, NOTE_G6,4, NOTE_B6,8,
				  NOTE_A6,4, NOTE_B6,8, NOTE_A6,4, NOTE_D6,8, NOTE_D6,8, NOTE_FS6,8,
				  NOTE_E6,-1,
				  REST,8, NOTE_D6,8, NOTE_E6,8, NOTE_FS6,-1,

				  REST,8, NOTE_D6,8, NOTE_D6,8, NOTE_FS6,8, NOTE_F6,-1, //20

				  REST,8, NOTE_D6,8, NOTE_F6,8, NOTE_E6,8, //end 2
				  NOTE_E6,-2, NOTE_A6,8, NOTE_CS7,8,
				  NOTE_FS7,8, NOTE_E7,4, NOTE_D7,8, NOTE_A7,-4,
};


int harrypotter[] = {


  // Hedwig's theme fromn the Harry Potter Movies
  // Socre from https://musescore.com/user/3811306/scores/4906610

  REST, 2, NOTE_D6, 4,
  NOTE_G6, -4, NOTE_AS6, 8, NOTE_A6, 4,
  NOTE_D6, 2, NOTE_D7, 4,
  NOTE_C7, -2,
  NOTE_A6, -2,
  NOTE_D6, -4, NOTE_AS6, 8, NOTE_A6, 4,
  NOTE_F6, 2, NOTE_GS6, 4,
  NOTE_D6, -1,
  NOTE_D6, 4,

  NOTE_D6, -4, NOTE_AS6, 8, NOTE_A6, 4, //10
  NOTE_D6, 2, NOTE_D7, 4,
  NOTE_F7, 2, NOTE_E7, 4,
  NOTE_DS7, 2, NOTE_B6, 4,
  NOTE_DS7, -4, NOTE_D7, 8, NOTE_CS7, 4,
  NOTE_CS6, 2, NOTE_B6, 4,
  NOTE_D6, -1,
  NOTE_AS6, 4,

  NOTE_D7, 2, NOTE_AS6, 4,//18
  NOTE_D7, 2, NOTE_AS6, 4,
  NOTE_DS7, 2, NOTE_D7, 4,
  NOTE_CS7, 2, NOTE_A6, 4,
  NOTE_AS6, -4, NOTE_D7, 8, NOTE_CS7, 4,
  NOTE_CS6, 2, NOTE_D6, 4,
  NOTE_D7, -1,
  REST,4, NOTE_AS6,4,

  NOTE_D7, 2, NOTE_AS6, 4,//26
  NOTE_D7, 2, NOTE_AS6, 4,
  NOTE_F7, 2, NOTE_E7, 4,
  NOTE_DS7, 2, NOTE_B6, 4,
  NOTE_DS7, -4, NOTE_D7, 8, NOTE_CS7, 4,
  NOTE_CS6, 2, NOTE_AS6, 4,
  NOTE_D6, -1,

};




int* songs[] = { starwars, harrypotter };
int SongSizes[] = {(sizeof starwars / sizeof starwars[0]),
					(sizeof harrypotter / sizeof harrypotter[0])};

// Hold songs array length
int NumOfSongs = sizeof songs / sizeof songs[0];

#endif /* SONGS_H_ */

/*
 *working form to get it to fit in speaker
 * low pich
REST,2, NOTE_D7,8, NOTE_B6,4, NOTE_D7,8, //1
		  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,
		  REST,8, NOTE_A6,8, NOTE_FS7,8, NOTE_E7,4, NOTE_D7,8,
		  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,
		  REST,4, NOTE_D7,8, NOTE_B6,4, NOTE_D7,8,
		  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,

		  REST,8, NOTE_B6,8, NOTE_B6,8, NOTE_G6,4, NOTE_B6,8, //7
		  NOTE_A6,4, NOTE_B6,8, NOTE_A6,4, NOTE_D6,2,
		  REST,4, NOTE_D7,8, NOTE_B6,4, NOTE_D6,8,
		  NOTE_CS6,4, NOTE_D6,8, NOTE_CS6,4, NOTE_A6,2,
		  REST,8, NOTE_A6,8, NOTE_FS7,8, NOTE_E7,4, NOTE_D7,8,
		  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,

		  REST,4, NOTE_D7,8, NOTE_B6,4, NOTE_D7,8, //13
		  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,
		  REST,8, NOTE_B6,8, NOTE_B6,8, NOTE_G6,4, NOTE_B6,8,
		  NOTE_A6,4, NOTE_B6,8, NOTE_A6,4, NOTE_D6,8, NOTE_D6,8, NOTE_FS6,8,
		  NOTE_E6,-1,
		  REST,8, NOTE_D6,8, NOTE_E6,8, NOTE_FS6,-1,

		  REST,8, NOTE_D6,8, NOTE_D6,8, NOTE_FS6,8, NOTE_F6,-1, //20

		  REST,8, NOTE_D6,8, NOTE_F6,8, NOTE_E6,-1, //end 1

		  //repeats from 1

		  REST,2, NOTE_D7,8, NOTE_B6,4, NOTE_D7,8, //1
		  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,
		  REST,8, NOTE_A6,8, NOTE_FS7,8, NOTE_E7,4, NOTE_D7,8,
		  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,
		  REST,4, NOTE_D7,8, NOTE_B6,4, NOTE_D7,8,
		  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,

		  REST,8, NOTE_B6,8, NOTE_B6,8, NOTE_G6,4, NOTE_B6,8, //7
		  NOTE_A6,4, NOTE_B6,8, NOTE_A6,4, NOTE_D6,2,
		  REST,4, NOTE_D6,8, NOTE_B6,4, NOTE_D7,8,
		  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,
		  REST,8, NOTE_A6,8, NOTE_FS7,8, NOTE_E7,4, NOTE_D7,8,
		  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,

		  REST,4, NOTE_D7,8, NOTE_B6,4, NOTE_D7,8, //13
		  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A6,2,
		  REST,8, NOTE_B6,8, NOTE_B6,8, NOTE_G6,4, NOTE_B6,8,
		  NOTE_A6,4, NOTE_B6,8, NOTE_A6,4, NOTE_D6,8, NOTE_D6,8, NOTE_FS6,8,
		  NOTE_E6,-1,
		  REST,8, NOTE_D6,8, NOTE_E6,8, NOTE_FS6,-1,

		  REST,8, NOTE_D6,8, NOTE_D6,8, NOTE_FS6,8, NOTE_F6,-1, //20

		  REST,8, NOTE_D6,8, NOTE_F6,8, NOTE_E6,8, //end 2
		  NOTE_E6,-2, NOTE_A6,8, NOTE_CS7,8,
		  NOTE_FS7,8, NOTE_E7,4, NOTE_D7,8, NOTE_A7,-4,


high pich


		  // Gren Hill Zone - Sonic the Hedgehog
		  // Score available at https://musescore.com/user/248346/scores/461661
		  // Theme by Masato Nakamura, arranged by Teddy Mason

		  REST,2, NOTE_D8,8, NOTE_B7,4, NOTE_D8,8, //1
		  NOTE_CS8,4, NOTE_D8,8, NOTE_CS8,4, NOTE_A7,2,
		  REST,8, NOTE_A7,8, NOTE_FS8,8, NOTE_E8,4, NOTE_D8,8,
		  NOTE_CS8,4, NOTE_D8,8, NOTE_CS8,4, NOTE_A7,2,
		  REST,4, NOTE_D8,8, NOTE_B7,4, NOTE_D8,8,
		  NOTE_CS8,4, NOTE_D8,8, NOTE_CS8,4, NOTE_A7,2,

		  REST,8, NOTE_B7,8, NOTE_B7,8, NOTE_G7,4, NOTE_B7,8, //7
		  NOTE_A7,4, NOTE_B7,8, NOTE_A7,4, NOTE_D7,2,
		  REST,4, NOTE_D8,8, NOTE_B7,4, NOTE_D7,8,
		  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A7,2,
		  REST,8, NOTE_A7,8, NOTE_FS8,8, NOTE_E8,4, NOTE_D8,8,
		  NOTE_CS8,4, NOTE_D8,8, NOTE_CS8,4, NOTE_A7,2,

		  REST,4, NOTE_D8,8, NOTE_B7,4, NOTE_D8,8, //13
		  NOTE_CS8,4, NOTE_D8,8, NOTE_CS8,4, NOTE_A7,2,
		  REST,8, NOTE_B7,8, NOTE_B8,8, NOTE_G7,4, NOTE_B7,8,
		  NOTE_A7,4, NOTE_B7,8, NOTE_A7,4, NOTE_D7,8, NOTE_D7,8, NOTE_FS7,8,
		  NOTE_E7,-1,
		  REST,8, NOTE_D7,8, NOTE_E7,8, NOTE_FS7,-1,

		  REST,8, NOTE_D7,8, NOTE_D7,8, NOTE_FS7,8, NOTE_F7,-1, //20

		  REST,8, NOTE_D6,8, NOTE_F6,8, NOTE_E6,-1, //end 1

		  //repeats from 1

		  REST,2, NOTE_D8,8, NOTE_B7,4, NOTE_D8,8, //1
  		  NOTE_CS8,4, NOTE_D8,8, NOTE_CS8,4, NOTE_A7,2,
		  REST,8, NOTE_A7,8, NOTE_FS8,8, NOTE_E8,4, NOTE_D8,8,
	 	  NOTE_CS8,4, NOTE_D8,8, NOTE_CS8,4, NOTE_A7,2,
 		  REST,4, NOTE_D8,8, NOTE_B7,4, NOTE_D8,8,
		  NOTE_CS8,4, NOTE_D8,8, NOTE_CS8,4, NOTE_A7,2,

		  REST,8, NOTE_B7,8, NOTE_B7,8, NOTE_G7,4, NOTE_B7,8, //7
		  NOTE_A7,4, NOTE_B7,8, NOTE_A7,4, NOTE_D7,2,
		  REST,4, NOTE_D8,8, NOTE_B7,4, NOTE_D7,8,
	 	  NOTE_CS7,4, NOTE_D7,8, NOTE_CS7,4, NOTE_A7,2,
	 	  REST,8, NOTE_A7,8, NOTE_FS8,8, NOTE_E8,4, NOTE_D8,8,
		  NOTE_CS8,4, NOTE_D8,8, NOTE_CS8,4, NOTE_A7,2,

		  REST,4, NOTE_D8,8, NOTE_B7,4, NOTE_D8,8, //13
		  NOTE_CS8,4, NOTE_D8,8, NOTE_CS8,4, NOTE_A7,2,
		  REST,8, NOTE_B7,8, NOTE_B8,8, NOTE_G7,4, NOTE_B7,8,
		  NOTE_A7,4, NOTE_B7,8, NOTE_A7,4, NOTE_D7,8, NOTE_D7,8, NOTE_FS7,8,
		  NOTE_E7,-1,
		  REST,8, NOTE_D7,8, NOTE_E7,8, NOTE_FS7,-1,

		  REST,8, NOTE_D7,8, NOTE_D7,8, NOTE_FS7,8, NOTE_F7,-1, //20

		  REST,8, NOTE_D7,8, NOTE_F7,8, NOTE_E7,8, //end 2
		  NOTE_E7,-2, NOTE_A7,8, NOTE_CS8,8,
		  NOTE_FS8,8, NOTE_E8,4, NOTE_D8,8, NOTE_A8,-4,
 *
 */

