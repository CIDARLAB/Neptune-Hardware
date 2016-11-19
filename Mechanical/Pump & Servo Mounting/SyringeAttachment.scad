//Change to suit your system

///// 1cc DONE
//syringe_thickness_in = 1.6;
//syringe_diameter_in = 8.5;
//syringe_slots_diameter = 4;

///// 3cc DONE
syringe_thickness_in = 1.45;
syringe_diameter_in = 12.65;
syringe_slots_diameter = 9;

///// 10cc DONE
//syringe_thickness_in = 1.85;
//syringe_diameter_in = 16.65;
//syringe_slots_diameter = 11;

///// 60cc DONE
//syringe_thickness_in = 2.6;
//syringe_diameter_in = 30.5;
//syringe_slots_diameter = 19;



// Do not change
syringe_thickness = syringe_thickness_in;
syringe_diameter = syringe_diameter_in + 0.5;
hole_r = 2.64/2 + .6;
padding = 3;
syringe_lip = 1;
piston_rod_slot_l = 10;
piston_rod_slot_w = 6;

body_w = syringe_diameter+padding;
body_l = syringe_lip+syringe_thickness+padding+piston_rod_slot_l;
body_h = syringe_diameter+padding;


difference(){
    //Main body
    translate([0,-body_l/2,0])
    cube([body_w,body_l,body_h],center=true);
    
    // Piston_rod attachment slot
    translate([0,-body_l+piston_rod_slot_l/2,0])
    cube([piston_rod_slot_w,piston_rod_slot_l,body_h*2],center=true);
    
    // Piston_rod screw hole
    translate([-body_w,-body_l+3,0])
    rotate([90,0,90])
    cylinder(h=body_w*2,r=hole_r);
    
    
    // Inner circle
    translate([0,-syringe_lip,0])
    rotate([90,0,0])
    cylinder(h=syringe_thickness, r=syringe_diameter/2);
    
    // Inner Square extrusion
    translate([-syringe_diameter/2,-syringe_thickness-syringe_lip,0])
    cube([syringe_diameter,syringe_thickness,body_h/2]); 
    
    
    // Outer circle
    translate([0,0,0])
    rotate([90,0,0])
    cylinder(h=syringe_lip, r=syringe_slots_diameter/2);
    
    // Outer Square extrusion
    translate([-syringe_slots_diameter/2,-syringe_lip,0])
    cube([syringe_slots_diameter,syringe_lip,body_h/2]);     
}
