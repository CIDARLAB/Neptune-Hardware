//Change to suit your system

// 60cc
//syringe_plung_thickness = 2.6;
//syringe_outer_diameter = 30.7;

// 10cc
//syringe_plung_thickness = 1.9;
//syringe_outer_diameter = 16.5;

// 3cc
syringe_plung_thickness = 1.7;
syringe_outer_diameter = 10.65;

// 1cc
//syringe_plung_thickness = 1.6;
//syringe_outer_diameter = 6.2;

center_of_rotation = 30;    // just an example




// Don't change unless you know what you are doing
percent_above_center_of_rotation = .5;
screw_hole_r = 2.5;     // automaticly 10mm away from edge of part
baseplate_w = 50;   // redefined in if
baseplate_h = 5;
baseplate_l = 7.62*8;   //MUST be divisable by 7.62 to fit on MEC board

upperslate_l = 12;  // redefined in if
upperslate_h = center_of_rotation+syringe_outer_diameter*percent_above_center_of_rotation; //simply the center of rotation plus any additional height added


if (syringe_outer_diameter <= 15) {
    baseplate_w = 25;
    upperslate_l = 12;

    difference() {
        // Main body
        union(){
            
            // Base Plate
            translate([0,0,2.5])
            cube([baseplate_w,baseplate_l,baseplate_h],center=true);
            
            // Upper Slate
            translate([0,0,upperslate_h/2])
            cube([baseplate_w,upperslate_l,upperslate_h],center=true);
        }
        
        // Screw holes
        translate([0,baseplate_l/2-7.62,0])
        cylinder (h=300,r=screw_hole_r);
        
        translate([0,-(baseplate_l/2-7.62),0])
        cylinder (h=300,r=screw_hole_r);
        
        
        // Slot
        translate([0,0,upperslate_h])
        cube([baseplate_w,syringe_plung_thickness,(syringe_outer_diameter+(syringe_outer_diameter+percent_above_center_of_rotation+15))],center=true);

        
        // Center Cylinder
            translate([0,0,upperslate_h-percent_above_center_of_rotation*syringe_outer_diameter])
            rotate([90,0,0])
        cylinder(h=300,r=syringe_outer_diameter/2);
        
            translate([0,0,upperslate_h-percent_above_center_of_rotation*syringe_outer_diameter])
            rotate([-90,0,0])
        cylinder(h=300,r=syringe_outer_diameter/2);
        
     
         // Center Square
            translate([0,0,upperslate_h])
            cube([syringe_outer_diameter,upperslate_l*2,syringe_outer_diameter*(percent_above_center_of_rotation*2)],center=true);
    }
}

if (syringe_outer_diameter > 15) {
    baseplate_w = 50;
    upperslate_l = 16;

    difference() {
        // Main body
        union(){
            
            // Base Plate
            translate([0,0,2.5])
            cube([baseplate_w,baseplate_l,baseplate_h],center=true);
            
            // Upper Slate
            translate([0,0,upperslate_h/2])
            cube([baseplate_w,upperslate_l,upperslate_h],center=true);
        }
        
        // Screw holes
        translate([0,baseplate_l/2-7.62,0])
        cylinder (h=300,r=screw_hole_r);
        
        translate([0,-(baseplate_l/2-7.62),0])
        cylinder (h=300,r=screw_hole_r);
        
        
        // Slot
        translate([0,0,upperslate_h])
        cube([baseplate_w,syringe_plung_thickness,(syringe_outer_diameter+(syringe_outer_diameter+percent_above_center_of_rotation+15))],center=true);

        
        // Center Cylinder
            translate([0,0,upperslate_h-percent_above_center_of_rotation*syringe_outer_diameter])
            rotate([90,0,0])
        cylinder(h=300,r=syringe_outer_diameter/2);
        
            translate([0,0,upperslate_h-percent_above_center_of_rotation*syringe_outer_diameter])
            rotate([-90,0,0])
        cylinder(h=300,r=syringe_outer_diameter/2);
        
     
         // Center Square
            translate([0,0,upperslate_h])
            cube([syringe_outer_diameter,upperslate_l*2,syringe_outer_diameter*(percent_above_center_of_rotation*2)],center=true);
    }
}
