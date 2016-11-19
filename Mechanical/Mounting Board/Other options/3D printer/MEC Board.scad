cube_side = 106.68;     //4.2 inches
cube_thickness = 12.7/2;  //0.5 inches

spacing = 7.62;         //0.3 inches
diameter = 3.175;       //1/8 inch

difference() {
    // Main Cube
    translate([spacing/2,spacing/2,0])
    cube([cube_side,cube_side,cube_thickness]);
    
    for (a = [1:14]) {
        for (b = [1:14]) {
            translate([spacing*a,spacing*b,0])
            cylinder(h=15,r=diameter/2);
            }
   }
}
