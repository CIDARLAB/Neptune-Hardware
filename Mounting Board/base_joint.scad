hole_seperation = 7.62;      //0.3 inches
hole_diameter = 3.175;       //.125 inches
part_thickness = 5;          // 5mm thick

n = 0; //multiples
a=0;
difference() {
    // Base
    translate([0,0,part_thickness/2])
    cube([((n+2)*hole_seperation)+2,((n+2)*hole_seperation)+2,part_thickness],center=true);
    
    // Holes
    for(a=[0:n]) {
        echo(a);
        translate([((hole_seperation/2)+hole_seperation/2*a),((hole_seperation/2)+hole_seperation/2*a),0])
        cylinder(h=part_thickness*4, r=hole_diameter/2);

        translate([-((hole_seperation/2)+hole_seperation/2*a),((hole_seperation/2)+hole_seperation/2*a),0])
        cylinder(h=part_thickness*4, r=hole_diameter/2);
      
        translate([((hole_seperation/2)+hole_seperation/2*a),-((hole_seperation/2)+hole_seperation/2*a),0])
        cylinder(h=part_thickness*4, r=hole_diameter/2);

        translate([-((hole_seperation/2)+hole_seperation/2*a),-((hole_seperation/2)+hole_seperation/2*a),0])
        cylinder(h=part_thickness*4, r=hole_diameter/2);  
    }
}     
 



