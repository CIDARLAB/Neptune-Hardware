//********************
syringediameter= 6.54;

//Large and medium
//sideflapwidth= 90- syringediameter - 45;
//Small
sideflapwidth= 90- syringediameter - 50;

//***************
syringeradius= syringediameter/2;
widthofpart=20;
//sideflapwidth= 15;




difference(){
translate([0,0,20])
    linear_extrude(
    height = 40, 
    center = true, 
    convexity = 10, 
    twist = 0)
    square([90,widthofpart]);
    
//Origin box    
translate([0,0,25])
    linear_extrude(
    height = 30, 
    center = true, 
    convexity = 10, 
    twist = 0)
    square([sideflapwidth,widthofpart]);
    
//Right side box
translate([90- sideflapwidth,0,25])
    linear_extrude(
    height = 30, 
    center = true, 
    convexity = 10, 
    twist = 0)
    square([sideflapwidth,widthofpart]);
    
//Origin woodscrew

     

//center hole square  
translate([6-2, widthofpart/2 - 7.5, 0])
    cube([4,15, widthofpart]); 



//Right woodscrew
 //center hole square  
translate([90-6-2, widthofpart/2 - 7.5, 0])
    cube([4,15, widthofpart]); 


//Top origin screw
translate([sideflapwidth + 5, widthofpart, 32])
rotate([90,0,0])
cylinder (h=widthofpart*2, r=2.5);

//Top right screw
translate([90-sideflapwidth-5, widthofpart, 32])
rotate([90,0,0])
cylinder (h=widthofpart*2, r=2.5);


hull(){
//Center hole circle
translate([45, widthofpart, syringeradius+2])
rotate([90,0,0])
cylinder (h=widthofpart*3, r=syringeradius);
     

//center hole square  
translate([45-syringeradius,widthofpart, -10])
    cube([syringeradius*2,10, syringeradius+10]); 
}

}