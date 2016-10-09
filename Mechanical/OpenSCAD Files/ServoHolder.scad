//Variables to change

servo_w = 22.75;               //40    66
servo_h = 12.5;               //20    30
servo_hole_diameter = 2;    //4     6
servo_hole_distance_from_top_of_servo = 6.25;  //5     6
servo_hole_seperation = 0;        //9.5        18


center_of_rotation = 30;



// Do not change variables unless you know what you're doing
servo_hole_r = servo_hole_diameter/2;
screw_hole_r = 2.5;     // automaticly 

// Two conditional statments; if servo_w < 50, make base certain width. Else increase it. That is it.

if (servo_w < 51) {
    // base_w must be divisable by 7.62
    base_w=7.62*12;  //91.44

    base_l = 10;
    base_h = 5;

    servo_module_w = servo_w+servo_hole_diameter*4;
    servo_module_l = base_l;
    servo_module_h = center_of_rotation+(servo_hole_seperation/2)+servo_hole_distance_from_top_of_servo;


    difference(){
        // Main body
        union(){
            //Base
            translate([0,0,base_h/2])
            cube([base_w,base_l,base_h],center=true);
            
            //Servo module
            translate([0,0,servo_module_h/2])
            cube([servo_module_w,servo_module_l,servo_module_h],center=true);
            
            // Sliding bars
            sliding_w=7.62*2;
            translate([base_w/2-sliding_w/2,0,base_h/2])
            cube([sliding_w,7.62*3/2+7.62*3/2,base_h],center=true);
            
            translate([-base_w/2+sliding_w/2,0,base_h/2])
            cube([sliding_w,7.62*3/2+7.62*3/2,base_h],center=true);
            
        }

        // Servo body hole
        translate([0,0,servo_module_h])
        cube([servo_w,500,servo_h*2],center=true);
        
        // Servo mounting holes
            //top left
            translate([-(servo_w/2+servo_hole_diameter),30,servo_module_h-servo_hole_distance_from_top_of_servo])
            rotate([90,0,0])
            cylinder(h=100,r=servo_hole_r);
            //bottom left
            translate([-(servo_w/2+servo_hole_diameter),30,servo_module_h-servo_hole_distance_from_top_of_servo-servo_hole_seperation])
            rotate([90,0,0])
            cylinder(h=100,r=servo_hole_r);
        
            //top right
            translate([(servo_w/2+servo_hole_diameter),30,servo_module_h-servo_hole_distance_from_top_of_servo])
            rotate([90,0,0])
            cylinder(h=100,r=servo_hole_r);
            //bottom right
            translate([(servo_w/2+servo_hole_diameter),30,servo_module_h-servo_hole_distance_from_top_of_servo-servo_hole_seperation])
            rotate([90,0,0])
            cylinder(h=100,r=servo_hole_r);
            
            
        // Screw Holes
        translate([base_w/2-7.62,0,0])
        cylinder(h=100,r=screw_hole_r);
        
        translate([-base_w/2+7.62,0,0])
        cylinder(h=100,r=screw_hole_r);
        
        // Screw RECTANGLES!    
        translate([base_w/2-7.62,0,0])
        cube([2*screw_hole_r,7.62*3/2,100],center=true);
        
        translate([-base_w/2+7.62,0,0])
        cube([2*screw_hole_r,7.62*3/2,100],center=true);
    }
    
}

if (servo_w >= 51) {
    base_w=7.62*16;     //121.92

    base_l = 10;
    base_h = 5;

    servo_module_w = servo_w+servo_hole_diameter*4;
    servo_module_l = base_l;
    servo_module_h = center_of_rotation+(servo_hole_seperation/2)+servo_hole_distance_from_top_of_servo;


    difference(){
        // Main body
        union(){
            //Base
            translate([0,0,base_h/2])
            cube([base_w,base_l,base_h],center=true);
            
            //Servo module
            translate([0,0,servo_module_h/2])
            cube([servo_module_w,servo_module_l,servo_module_h],center=true);
            
            // Sliding bars
            sliding_w=7.62*2;
            translate([base_w/2-sliding_w/2,0,base_h/2])
            cube([sliding_w,7.62*3/2+7.62*3/2,base_h],center=true);
            
            translate([-base_w/2+sliding_w/2,0,base_h/2])
            cube([sliding_w,7.62*3/2+7.62*3/2,base_h],center=true);
            
        }

        // Servo body hole
        translate([0,0,servo_module_h])
        cube([servo_w,500,servo_h*2],center=true);
        
        // Servo mounting holes
            //top left
            translate([-(servo_w/2+servo_hole_diameter),30,servo_module_h-servo_hole_distance_from_top_of_servo])
            rotate([90,0,0])
            cylinder(h=100,r=servo_hole_r);
            //bottom left
            translate([-(servo_w/2+servo_hole_diameter),30,servo_module_h-servo_hole_distance_from_top_of_servo-servo_hole_seperation])
            rotate([90,0,0])
            cylinder(h=100,r=servo_hole_r);
        
            //top right
            translate([(servo_w/2+servo_hole_diameter),30,servo_module_h-servo_hole_distance_from_top_of_servo])
            rotate([90,0,0])
            cylinder(h=100,r=servo_hole_r);
            //bottom right
            translate([(servo_w/2+servo_hole_diameter),30,servo_module_h-servo_hole_distance_from_top_of_servo-servo_hole_seperation])
            rotate([90,0,0])
            cylinder(h=100,r=servo_hole_r);
            
            
        // Screw Holes
        translate([base_w/2-7.62,0,0])
        cylinder(h=100,r=screw_hole_r);
        
        translate([-base_w/2+7.62,0,0])
        cylinder(h=100,r=screw_hole_r);

        // Screw RECTANGLES!    
        translate([base_w/2-7.62,0,0])
        cube([2*screw_hole_r,7.62*3/2,100],center=true);
        
        translate([-base_w/2+7.62,0,0])
        cube([2*screw_hole_r,7.62*3/2,100],center=true);
    }
}



