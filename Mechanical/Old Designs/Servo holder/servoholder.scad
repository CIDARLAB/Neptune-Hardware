servowidth = 40;
servoheight = 20;
shdiameter= 4;
distanceholes = 9.5;
shradius= shdiameter/2;


difference(){

union(){
linear_extrude(height = 10, 
    center = true, 
    convexity = 10, 
    twist = 0)
polygon(points= [
    [0,0],
    [0,10],
    [8.75,10],
    [52.5-servowidth/2 -10.25, 10], 
    [52.5-servowidth/2 -10.25,20+ servoheight],
    [52.5-servowidth/2,20+ servoheight],
    [52.5-servowidth/2,20],
    [52.5+servowidth/2,20],
    [52.5+servowidth/2,20+ servoheight],
    [62.75+servowidth/2,20+ servoheight],
    [62.75 + servowidth/2,10], 
    [105,10], 
    [105,0]
    ]); 
    
    //two extrusions on sides
    cube([12,10,15]);
  
  translate([105-12,0,0])
  cube([12,10,15]);  
} 

//Servo screw holes
//bottom origin
translate([52.5-servowidth/2 -5.125, 20- distanceholes/2 + servoheight/2, -5])
cylinder (h=10, r=shradius);
   
//top origin
translate([52.5-servowidth/2 -5.125, 20+ distanceholes/2 + servoheight/2, -5])
cylinder (h=10, r=shradius);

//top
translate([52.5+servowidth/2 +5.125, 20+ distanceholes/2 + servoheight/2, -5])
cylinder (h=10, r=shradius);

//bottom
translate([52.5+servowidth/2 +5.125, 20 - distanceholes/2 + servoheight/2, -5])
cylinder (h=10, r=shradius);


//Wood screw holes
translate([3, 0, 0])
cube ([6,10,10]);

translate([105-6-3, 0, 0])
cube ([6,10,10]);

}