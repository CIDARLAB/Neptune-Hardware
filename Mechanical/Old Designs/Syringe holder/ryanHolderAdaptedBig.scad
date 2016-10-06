syringediameter = 30.9;
syringeradius = syringediameter/2;
widthofpart= 10;
sideflapwidth = 60;
holeradius = 2.5;
syringeflap = 2.5;
bigger =5;

difference(){

union(){
translate([0,-bigger/2,20])
    linear_extrude(
    height = 40, 
    center = true, 
    convexity = 10, 
    twist = 0)
    square([60,widthofpart+bigger]);
    
//Top box
translate([0,widthofpart,2.5])
    linear_extrude(
    height = 5, 
    center = true, 
    convexity = 10, 
    twist = 0)
    square([sideflapwidth,25]);
    
//Bottom box
translate([0,-widthofpart-15,2.5])
    linear_extrude(
    height = 5, 
    center = true, 
    convexity = 10, 
    twist = 0)
    square([sideflapwidth,25]);
    
}

//Top hole
translate([30, -20 + 2*1.2, 0])
cylinder (h=widthofpart*2, r=holeradius);

//Bottom hole
translate([30, widthofpart+ 20-2*1.2, 0])
cylinder (h=widthofpart*2, r=holeradius);


//Syringe cut

translate([30, widthofpart+bigger, 30])
rotate([90,0,0])
cylinder (h=widthofpart*2 + bigger, r=syringeradius);

// Square with syringe
translate([30-syringeradius, widthofpart+bigger, 30])
rotate([90,0,0])
    linear_extrude(
    height = syringediameter*2, 
    center = true, 
    convexity = 10, 
    twist = 0)
    square([syringediameter,syringediameter]);



//Flap hole
translate([0,syringeflap, 40])
    linear_extrude(
    height = syringediameter*2, 
    center = true, 
    convexity = 10, 
    twist = 0)
    square([60,syringeflap ]);


}