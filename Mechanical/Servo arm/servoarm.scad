length = 90;
radius = 6.3;
circleorigin = 2;
circleend = 1;


difference(){
    
 union(){
translate([radius, 0, 0])
cube(size=[length- 2*radius, 
2*radius, 
5.5]);

translate ([radius, radius, 0])
cylinder(h = 5.5, 
r = radius);

translate ([length-radius, radius, 0])
cylinder(h = 5.5, 
r = radius);
 }




//Bigger hole
translate([radius, radius, -2.5])
cylinder (h= 10.5, r=circleorigin);

//Smaller hole
translate([length-radius, radius, -2.5])
cylinder (h= 10.5, r=circleend);
}
