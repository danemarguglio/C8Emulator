## An in-depth look at the pong binary with some explanations
 Thanks Joel!
```
[200] : 6a02
[202] : 6b0c
[204] : 6c3f
[206] : 6d0c
[208] : a2ea
[20a] : dab6
[20c] : dcd6
[20e] : 6e00
[210] : 22d4
[212] : 6603 // set ball x pos
[214] : 6802 // Set ball x speed

//GAME START
[216] : 6060
[218] : f015
[21a] : f007
[21c] : 3000
[21e] : 121a
[220] : c717 //get rand, start game
[222] : 7708 // set ball y pos?
[224] : 69ff // set ball y speed?
[226] : a2f0
[228] : d671 //draw ball

//NORMAL DRAW CODE
[22a] : a2ea
[22c] : dab6 //draw paddles
[22e] : dcd6
[230] : 6001
[232] : e0a1 //skip if 1 not pressed
[234] : 7bfe // Add fe to reg b (move paddle)
[236] : 6004
[238] : e0a1 //skip if 4 not pressed
[23a] : 7b02 // Move paddle
[23c] : 601f
[23e] : 8b02 // And b and 1f (check out of bounds)
[240] : dab6 //draw left paddle
[242] : 600c //Start move left paddle
[244] : e0a1
[246] : 7dfe
[248] : 600d
[24a] : e0a1
[24c] : 7d02
[24e] : 601f
[250] : 8d02
[252] : dcd6 //End move left paddle

// UNDRAW BALL AND MOVE

//MOVE BALL
[254] : a2f0 
[256] : d671 //UnDraw ball
[258] : 8684 // Add r6 and r8 (move ball x)
[25a] : 8794 // Add r7 and r9 (move ball y)
[25c] : 603f
[25e] : 8602 // 6 is balls x pos, oob check
[260] : 611f
[262] : 8712 //oob y check, 7 is balls y

//CHECK FOR COLLISIONS/update speed
[264] : 4602 // if ball x is not 02, skip
[266] : 1278 //Jump 278
[268] : 463f // if ball x is not 3f skip
[26a] : 1282 // Jump 282

[26c] : 471f // If y is not max
[26e] : 69ff //Reverse y speed
[270] : 4700 // If y is not min
[272] : 6901 // reverse y speed

//REDRAW BALL
[274] : d671 //draw ball
[276] : 122a //Jump 22a (normal code, draw stuff)

//UPDATE SPEED STUFF
//if ball x is 02
[278] : 6802 // set x speed ->
[27a] : 6301 // r3 = 1, scores
[27c] : 8070 // v0 = v7
[27e] : 80b5 // sub paddle pos from ball pos
[280] : 128a // Jump down 
//if ball is 3f
[282] : 68fe //set x speed <-
[284] : 630a // r3 = 0a, scores
[286] : 8070 // v0 = v7, balls y val
[288] : 80d5 // sub paddle pos from ball pos

//both, checks if the subtractions have carrys (how many)

// MISSED PADDLE COMPLETELY (TOO LOW)
[28a] : 3f01 //if f == 1, skip, this is the carry bit of the subtraction!
[28c] : 12a2 //Jump 2a2, update score, reset ball pos

[28e] : 6102 // Set 1 to 2
[290] : 8015 // r1 - r0

//HIT THE BOTTOM OF THE PADDLE
[292] : 3f01 // if f == 1 (again, so both subs threw a 1) skip
[294] : 12ba // Jump to update y speed

[296] : 8015 // Subtract

//HIT THE MIDDLE OF THE PADDLE
[298] : 3f01 // if f ==1 again, skip
[29a] : 12c8 

[29c] : 8015

//HIT THE TOP OF THE PADDLE
[29e] : 3f01 // AGAIN
[2a0] : 12c2 //Jump update y speed


//OTHERWISE BALL DID NOT HIT THE PADDLE, SO WE FAIL AND UPDATE SCORE
//BASED ON WHICH SIDE WE ARE ON

//UPDATE SCORE
[2a2] : 6020 //set 0 to 20
[2a4] : f018 // set sound timer to 20
[2a6] : 22d4 //Call 2d4 (display score), erases
[2a8] : 8e34 // add r3 to re, update score, r3 pop'd based on the side it went out
[2aa] : 22d4 // Display score again
[2ac] : 663e // Set x pos to 3e
[2ae] : 3301 // skip if r3 == 1, only true depending on which side it went out
[2b0] : 6603 // Set x pos to 03
[2b2] : 68fe // Set x speed
[2b4] : 3301 // skip if r3 == 1
[2b6] : 6802 // set x speed opposite
[2b8] : 1216 // Jump 216 (Game restart)

//Update y speed absed on paddle? Hit the bottom
[2ba] : 79ff // Add ff to r9, bals y speed
[2bc] : 49fe // skip if r9 != fe
[2be] : 69ff // set r9 to ff
[2c0] : 12c8 // Play sound

//Update y speed based on paddle? Hit the top
[2c2] : 7901 // add 1 to y speed
[2c4] : 4902 // skip if 9 != 2
[2c6] : 6901 // set y speed

//PLAY SOUND OF HITTING PADDLE
[2c8] : 6004
[2ca] : f018 // Play sound (18)
[2cc] : 7601 // Add 1 to x pos of ball
[2ce] : 4640 // x pos != 40 skip
[2d0] : 76fe // add fe to x pos ball
[2d2] : 126c // Jump back up to collision checks

//DISPLAY SCORES
[2d4] : a2f2
[2d6] : fe33 //split score, score stored in E
[2d8] : f265
[2da] : f129
[2dc] : 6414
[2de] : 6500
[2e0] : d455
[2e2] : 7415
[2e4] : f229
[2e6] : d455
[2e8] : ee


[2ea] : 8080 //PADDLE
[2ec] : 8080 //PADDLE
[2ee] : 8080 //PADDLE
[2f0] : 8000 //BALL
[2f2] : 0
[2f4] : 0
```